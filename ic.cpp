#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <chrono> //do timera
using namespace std;

//FUNKCJE:
//funkcja wczytuje parametr N z pliku i zapisuje go do wektora
vector <int> wczytaj_N(string sciezka) { 
	//wczytanie wartosci z pliku:
	ifstream plik(sciezka);	//sciezka do pliku wejsciowego
	string str;
	vector <int> x;
	
	if(plik.is_open())
	{	
		while (std::getline(plik, str))
		{
			istringstream iss(str);
			string N;
			string M;
			
			iss >> N >> M;
			x.push_back(stoi(N));
		}
	}
	else
	{
		cout<<"Blad przy otwarciu pliku!"<<endl;
	}
	plik.close();
	return x;
}

//funkcja wczytuje parametr M z pliku i zapisuje go do wektora
vector <int> wczytaj_M(string sciezka) { 
	//wczytanie wartosci z pliku:
	ifstream plik(sciezka);	//sciezka do pliku wejsciowego
	string str;
	vector <int> x;
	
	if(plik.is_open())
	{	
		while (std::getline(plik, str))
		{
			istringstream iss(str);
			string N;
			string M;
			
			iss >> N >> M;
			x.push_back(stoi(M));
		}
	}
	else
	{
		cout<<"Blad przy otwarciu pliku!"<<endl;
	}
	plik.close();
	return x;
}


int main ()
{
	srand (time(NULL));
	
	//parametry modelu IC:
	int No = 0;
	int q = 0;
	int p = 0;
	
	//podanie wartosci powyzszych parametrow przez uzytkownika:
	cout<<"Podaj ponizsze wartosci: "<<endl;
	cout<<"Liczba cytowan wlasnych na krok czasowy - p: ";
	cin>>p;
	cout<<"Liczba cytowan obcych na krok czasowy  - q: ";
	cin>>q;
	
	//wczytanie z pliku parametrow N i M:
	vector <int> N;
	N = wczytaj_N("appa_do_symulacji.txt");
	vector <int> M;
	M = wczytaj_M("appa_do_symulacji.txt");
	
	//rozmiar segmentu poczatkowego:
	No = p + q;
		
	for(int i = 0; i < N.size(); i++) //czyli dla każdej osoby
	{
		//pliki do zapisu wynikow:
		//wszystkie cytowania
		string name="APPA_los_wszystkie_" + to_string(i+1) + ".txt"; 
		ofstream plik_wynikowy(name);
			
		//cytowania wlasne
		string name_auto="APPA_los_auto_" + to_string(i+1) + ".txt";
		ofstream plik_wynikowy_auto(name_auto);
			
		//cytowania obce
		string name_obce="APPA_los_obce_" + to_string(i+1) + ".txt";
		ofstream plik_wynikowy_obce(name_obce);
			
		//czas
		string name_czas="APPA_los_czas_" + to_string(i+1) + ".txt"; 
		ofstream plik_wynikowy_czas(name_czas);
		
		for (int pp = 0; pp < 100; pp++)
		{
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); //poczatek symulacji:
				
			//wektory
			vector <int> wekt_N; //do przechowywania publikacji - losowy przydzial autocytowan
			vector <int> tablica; //wszystkie cytowania i publikacje - cala symulacja
			vector <int> cyt_wlasne; //osobny wektor do zapisu cytowan wlasnych...
			vector <int> cyt_obce; //...i obcych
			
			if((N[i] >= No) && (M[i] != 0))  //program wchodzi w procedure dodawania publikacji i cytowan
			{
				//segment poczatkowy:
				for (int ii = 0; ii < No; ii++)
				{
					tablica.push_back(ii + 1);
					wekt_N.push_back(ii + 1); //gdy autocytowania przydzielane LOSOWO
				} 
				
				//uzupelnianie tablicy cytowaniami i publikacjami:
				if ((p + q)*(N[i] - No) >= M[i]) //Przypadek 1 - wyczerpanie zbioru cytowan
				{
					//cout<<"Przypadek 1 - wyczerpanie zbioru cytowań"<<endl;
					if((int (M[i]/(p + q))) != 0) //czy powstanie segment
					{
						for (int j = 0; j < int (M[i]/(p + q)); j++)
						{
							for (int k = 0; k < (p + q); k++) //obreb segmentu
							{
								if(k < p) //pierwsze p rozdanych cytowan traktujemy jako cytowania wlasne
								{
									tablica.push_back(wekt_N[rand() % (wekt_N.size())]);	// autocytowania przydzielane LOSOWO
									//tablica.push_back(tablica[rand() % (tablica.size() - k)]); // autocytowania przydzielane PREFERENCYJNIE
									//- k, by nie wybieralo z przed chwila dodanych autocytowan
									cyt_wlasne.push_back(tablica[tablica.size()- 1]); //wpisuje do tablicy te sama wartosc, co dodana przed chwila do tablicy
								}
								else //cytowania za p pierwszymi cytowaniami (wlasnymi) to cytowania obce
								{
									tablica.push_back(tablica[rand() % (tablica.size() - k)]);
									cyt_obce.push_back(tablica[tablica.size()- 1]);
								}
							}
							tablica.push_back(No + (j + 1)); //na koniec segmentu do wektora dodawana jest kolejna publikacja
							wekt_N.push_back(No + (j + 1));
						}
					}	
					//ostatni segment - tak jak wyzej, pierwsze p cytowan z niepelnego segmentu to autocytowania
					for (int k = 0; k < int (M[i]%(p + q)); k++)
					{
						if(k < p)
						{
							tablica.push_back(wekt_N[rand() % (wekt_N.size())]); // autocytowania przydzielane LOSOWO
							//tablica.push_back(tablica[rand() % (tablica.size() - k)]); // autocytowania przydzielane PREFERENCYJNIE
							cyt_wlasne.push_back(tablica[tablica.size()- 1]);
						}	
						else
						{
							tablica.push_back(tablica[rand() % (tablica.size() - k)]);
							cyt_obce.push_back(tablica[tablica.size()- 1]);
						}				
					}
				}
				else if ((p + q)*(N[i] - No) < M[i]) //Przypadek 2 - wyczerpanie zbioru publikacji
				{
					//cout<<"Przypadek 2 - wyczerpanie zbioru publikacji"<<endl;
					if((N[i] - No) != 0) //czy powstanie segment
					{
						for (int j = 0; j < (N[i] - No); j++)
						{
							for (int k = 0; k < (p + q); k++)
							{
								if(k < p) 
								{
									tablica.push_back(wekt_N[rand() % (wekt_N.size())]);	// autocytowania przydzielane LOSOWO
									//tablica.push_back(tablica[rand() % (tablica.size() - k)]); // autocytowania przydzielane PREFERENCYJNIE
									cyt_wlasne.push_back(tablica[tablica.size()- 1]);
								}
								else
								{
									tablica.push_back(tablica[rand() % (tablica.size() - k)]);	
									cyt_obce.push_back(tablica[tablica.size()- 1]);
								}	
							}
							tablica.push_back(No + (j + 1)); 
							wekt_N.push_back(No + (j + 1));
						}
								
					}
					//dodawane cytowania obce:
					for (int j = 0; j < int ((M[i] - (N[i] - No)*(p + q))/q); j++)
					{
						for (int k = 0; k < q; k++)
						{
							tablica.push_back(tablica[rand() % (tablica.size() - k)]);
							cyt_obce.push_back(tablica[tablica.size()- 1]);
						}		
					}
					//ostatni segment:
					for (int j = 0; j < int ((M[i] - (N[i] - No)*(p + q))%q); j++)
					{
						tablica.push_back(tablica[rand() % (tablica.size() - j)]);
						cyt_obce.push_back(tablica[tablica.size()- 1]);
					}
				}
				
				//wyznaczenie wektora cytowan
				int Nmax = *std::max_element(tablica.begin(), tablica.end()); //najwiekszy element w wektorze -> liczba publikacji 
				int cytowania[Nmax]; 
				for(int w = 0; w < Nmax; w++)
				{
					cytowania[w] = count(tablica.begin(), tablica.end(), w+1);		
				}
				//odjecie liczby 1 od kazdej z wartosci:
				for(int ww = 0; ww < Nmax; ww++) 
				{
					cytowania[ww] = cytowania[ww] - 1;
				}
				
				//wyznaczenie wektora autocytowan
				int autocytowania[Nmax];
				for(int wa = 0; wa < Nmax; wa++)
				{
					autocytowania[wa] = count(cyt_wlasne.begin(), cyt_wlasne.end(), wa+1);		
				}
				
				//wyznaczenie wektora cytowan obcych
				int obce_cytowania[Nmax];
				for(int wo = 0; wo < Nmax; wo++)
				{
					obce_cytowania[wo] = count(cyt_obce.begin(), cyt_obce.end(), wo+1);		
				}
				
				
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); //koniec symulacji

				//zapis wektorow cytowan do plikow:
				if (plik_wynikowy.is_open() && plik_wynikowy_auto.is_open() && plik_wynikowy_obce.is_open())
				{
					for(int z = 0; z < Nmax; z++ ) {
						plik_wynikowy << cytowania[z] << " ";
						plik_wynikowy_auto << autocytowania[z] << " ";
						plik_wynikowy_obce << obce_cytowania[z] << " ";
					}
					plik_wynikowy << "\n";
					plik_wynikowy_auto << "\n";
					plik_wynikowy_obce << "\n";
				}
				else
				{
					cout<<"Blad przy zapisie do pliku 1!"<<endl;
				}
				
				//zapis czasu wykonywania symulacji do pliku:
				if (plik_wynikowy_czas.is_open())
				{
					plik_wynikowy_czas << (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count())<<std::endl;
				}
				else
				{
					cout<<"Blad przy zapisie do pliku 2!"<<endl;
				}	
			}
			else //gdy, z uwagi na parametry, w ogole nie weszlo w wykonywanie procedury
			{
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); 
				//zapis wektorow cytowan do plikow:
				if (plik_wynikowy.is_open() && plik_wynikowy_auto.is_open() && plik_wynikowy_obce.is_open())
				{
					for(int z = 0; z < N[i]; z++ ) {
						plik_wynikowy << 0 << " ";
						plik_wynikowy_auto << 0 << " ";
						plik_wynikowy_obce << 0 << " ";
					}
					plik_wynikowy << "\n";
					plik_wynikowy_auto << "\n";
					plik_wynikowy_obce << "\n";
				}
				else
				{
					cout<<"Blad przy zapisie do pliku 1!"<<endl;
				}
				
				//zapis czasu wykonywania symulacji do pliku:
				if (plik_wynikowy_czas.is_open())
				{
					plik_wynikowy_czas << (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count())<<std::endl;
				}
				else
				{
					cout<<"Blad przy zapisie do pliku 2!"<<endl;
				}	
			}
			wekt_N.clear();
			tablica.clear();
			cyt_wlasne.clear();
			cyt_obce.clear();			
		}
		plik_wynikowy.close();
		plik_wynikowy_auto.close();
		plik_wynikowy_obce.close();
		plik_wynikowy_czas.close();
	}
	return 0;
}
