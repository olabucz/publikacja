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
//funkcja wczytujaca wszystkie wektory z pliku i zwracajaca vector vector-ow -> wynik[numer wektora][kolejne elementy wektora] - wynik przypisac do wekt_N, wekt_C..
vector<vector<int>> wczytaj_tablice_wektorow(string nazwa_pliku){
	ifstream inFile(nazwa_pliku); //otworzenie pliku
	
	string line;
	vector<vector<int>> tabl_wekt;
	
	if(inFile)
	{
		while(getline(inFile, line)){
		  istringstream is(line);
		  tabl_wekt.push_back(vector<int>(istream_iterator<int>(is),istream_iterator<int>() ) );
		}
	}
	else 
    {
        cout<<"Blad przy otwieraniu pliku!"<<endl;
    }
	inFile.close(); //zamkniecie pliku
	return(tabl_wekt);
}

int main() {
	srand (time(NULL));
	
    //wczytanie wektorow z plikow:
    vector<vector<int>> N_t = wczytaj_tablice_wektorow("N_t.txt");
    vector<vector<int>> C_auto_t = wczytaj_tablice_wektorow("Cauto_t.txt"); 
    vector<vector<int>> C_obce_t = wczytaj_tablice_wektorow("Cobce_t.txt"); 
           
    if((N_t.size() != C_obce_t.size()) || (N_t.size() != C_auto_t.size()) || (C_obce_t.size() != C_auto_t.size()) ){
		 cout<<"Rozna dlugosc plikow!"<<endl; 
		 return 1;
	}
    
    else{
		for(int i = 0; i < N_t.size(); i++) //czyli dla kazdej osoby
		{
			//pliki do zapisu wynikow:
			//wszystkie cytowania
			string name="PRE_auto3_par_wszystkie_" + to_string(i+1) + ".txt"; 
			ofstream plik_wynikowy(name);
			
			//cytowania wlasne
			string name_auto="PRE_auto3_par_auto_" + to_string(i+1) + ".txt";
			ofstream plik_wynikowy_auto(name_auto);
			
			//cytowania obce
			string name_obce="PRE_auto3_par_obce_" + to_string(i+1) + ".txt";
			ofstream plik_wynikowy_obce(name_obce);
			
			//czas
			string name_czas="PRE_auto3_par_czas_" + to_string(i+1) + ".txt"; 
			ofstream plik_wynikowy_czas(name_czas);

			for (int p = 0; p < 100; p++)
			{ 
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); //poczatek symulacji:
				
				//wektory
				vector <int> N; //wektor publikacji - do losowego przydzialu cytowan 
				vector <int> tablica; //wektor wszystkich: publikacji i cytowan - symulacja
				vector <int> cyt_wlasne; //osobny wektor na same cytowania wlasne...
				vector <int> cyt_obce;  //...i obce

				//N_t[i] - i-ty wektor/osoba, N_t[i][j] - liczba cytowan/publikacji w j-tym roku
				for (int j = 0; j < C_obce_t[i].size(); j++){ //wstawianie wartosci - N, Cobce, Cauto - z wektorow,
					//iteracja od dlugosci wektora cytowan (a nie publikacji) bo na koniec mogly byc lata bez publikacji, a z cytowaniami (wekt cytowan moze byc dluzszy od wekt publikacji)
					//pierwszy rok - poczatek symulacji - publikacje i cytowania
					if(j == 0){
						//najpierw publikacje
						for(int k = 0; k < N_t[i][j]; k++){
							N.push_back(k+1); //wstawienie numeru publikacji 
							tablica.push_back(N[N.size()- 1]); //wstawienie numeru publikacji do analizowanej tablicy - to co przed chwila zostało dodane do N
						}
						//potem ewentualnie cytowania, jesli sa
						//cytowania wlasne:
						for(int l = 0; l < C_auto_t[i][j]; l++){
							//tablica.push_back(N[rand() % (N.size())] ); //autocytowania LOSOWO
							tablica.push_back(tablica[rand() % (tablica.size() - l)]); //autocytowania PREFERENCYJNIE
							
							cyt_wlasne.push_back(tablica[tablica.size() - 1]);
						}
						//cytowania obce:	
						for(int m = 0; m < C_obce_t[i][j]; m ++){
							tablica.push_back(tablica[rand() % (tablica.size() - C_auto_t[i][j] - m)] ); //cytowania obce PREFERENCYJNIE
							// - C_auto_t[i][j] - by nie losowalo z dodanych przed chwila autocytowan
							
							cyt_obce.push_back(tablica[tablica.size() - 1]);
						}
					}
									
					//kolejne etapy symulacji - najpierw cytowania, potem publikacje
					else{
						//najpierw cytowania:
						//wlasne:
						for(int l = 0; l < C_auto_t[i][j]; l++){
							//tablica.push_back(N[rand() % (N.size())] ); //autocytowania LOSOWO
							tablica.push_back(tablica[rand() % (tablica.size() - l)]); //autocytowania PREFERENCYJNIE
							
							cyt_wlasne.push_back(tablica[tablica.size() - 1]);
						}
						//obce:
						for(int m = 0; m < C_obce_t[i][j]; m ++){ 
							tablica.push_back(tablica[rand() % (tablica.size() - C_auto_t[i][j] - m)] ); //cytowania obce PREFERENCYJNIE
							// - C_auto_t[i][j] - by nie losowało z dodanych przed chwilą autocytowań
							
							cyt_obce.push_back(tablica[tablica.size() - 1]);
						}
						//kolejne publikacje:
						if(j < N_t[i].size()){ //dodaje publikacje, a jak publikacje sie skoncza (cytowania moga jeszcze byc) to nie dodaje juz nic
							for(int k = 0; k < N_t[i][j]; k++){
								N.push_back(N.back() + 1);
								tablica.push_back(N[N.size()- 1]); 
							}
						}
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
					
								
				N.clear();
				tablica.clear();	
				cyt_wlasne.clear();
				cyt_obce.clear();	
			}
			plik_wynikowy.close();
			plik_wynikowy_czas.close();	
			plik_wynikowy_auto.close();
			plik_wynikowy_obce.close();
		}		
	}
	return 0;
}








/*    
    for(int i = 0; i < N_t.size(); i++){
		for(int j = 0; j < N_t[i].size(); j++){
			cout<<N_t[i][j]<<" ";
		}
		cout<<endl;
		
		for(int j = 0; j < C_auto_t[i].size(); j++){
			cout<<C_auto_t[i][j]<<" ";
		}
		cout<<endl;
		
		for(int j = 0; j < C_obce_t[i].size(); j++){
			cout<<C_obce_t[i][j]<<" ";
		}
		cout<<endl;
	}
	
	cout<<endl;*/
