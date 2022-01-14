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
    vector<vector<int>> N_t = wczytaj_tablice_wektorow("APPA_N_od_t.txt"); 
    vector<vector<int>> C_auto_t = wczytaj_tablice_wektorow("APPA_C_autorzy_od_t.txt"); 
    vector<vector<int>> C_obce_t = wczytaj_tablice_wektorow("APPA_C_obce2_od_t.txt"); 
 
	if((N_t.size() != C_obce_t.size()) || (N_t.size() != C_auto_t.size()) || (C_obce_t.size() != C_auto_t.size()) ){
		 cout<<"Rozna dlugosc plikow!"<<endl; 
		 return 1;
	}
	
	else{
		for(int i = 0; i < N_t.size(); i++) //czyli dla każdej osoby ----------- N_t.size()
		{
			//pliki do zapisu wynikow:
			//wszystkie cytowania
			string name="APPA_auto2_ost_wszystkie_" + to_string(i+1) + ".txt"; 
			ofstream plik_wynikowy(name);
			
			//cytowania własne
			string name_auto="APPA_auto2_ost_auto_" + to_string(i+1) + ".txt";
			ofstream plik_wynikowy_auto(name_auto);
			
			//cytowania obce
			string name_obce="APPA_auto2_ost_obce_" + to_string(i+1) + ".txt";
			ofstream plik_wynikowy_obce(name_obce);
			
			//czas
			string name_czas="APPA_auto2_ost_czas_" + to_string(i+1) + ".txt"; 
			ofstream plik_wynikowy_czas(name_czas);
			
			for (int p = 0; p < 100; p++)
			{
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); //poczatek symulacji:
				
				//wektory
				vector <int> N; 
				vector <int> tablica;
				vector <int> cyt_wlasne;
				vector <int> cyt_obce; 
				
				//N_t[i] - i-ta lista/osoba, N_t[i][j] - liczba cytowan/publikacji w j-tym roku
				for (int j = 0; j < C_obce_t[i].size(); j++){ //wstawianie wartosci - N, Cobce, Cauto - z wektorow do tablicy
					
					//wyliczenie sumy publikacji do j-tego kroku (do pozniejszej dystrybucji autocytowan)
					int sum_of_elems = 0;
					for(int s = 0; s < j; s++){
						sum_of_elems += N_t[i][s];
					}
					
					//pierwszy rok - początek symulacji - najpierw publikacje, potem cytowania 
					if(j == 0){
						//cout<<"jeden"<<endl;
						//najpierw publikacje
						for(int k = 0; k < N_t[i][j]; k++){
							N.push_back(k+1); //wstawienie numeru publikacji 
							tablica.push_back(N[N.size()- 1]); //wstawienie numeru publikacji do analizowanej tablicy - to co przed chwilą zostało dodane do N
						}
						
						//potem ewentualnie cytowania, jesli sa
						//cytowania własne:
						int x = 0; //iteracja "w dol wektora publikacji"
						int xx = 0; //iteracja, dzieki ktorej wroci do -1 ostatniej publikacji i znowu bedzie schodzil "w dol wektora publikacji"
						for(int l = 0; l < C_auto_t[i][j]; l++){
							tablica.push_back(N[N.size() - 1 - x - xx]); //N.size() - 1 = ostatni element w wektorze, - x = schodzi w dol, - xx = nizsze wartosci
							cyt_wlasne.push_back(tablica[tablica.size() - 1]);	
							
							int temp;
							temp = N.size() - 1 - xx - 1; //czy jest mozliwosc "cofnac sie" w wektorze publikacji, by ponownie schodzic w dol = xx - 1

							if(((N.size() - 1 - x - xx) == 0) && (temp >= 0)){ //dotarl do pierwszego elementu (indeks 0) wektora publikacji i moze sie cofnac, bo nie wyjdzie poza wektor
								xx +=1;	
								x = 0; //czyli cofa sie na koniec wektora publikacji i schodzi w dol, ale od dodatkowego -1 (xx)
							}
							else if(((N.size() - 1 - x - xx) == 0) && (temp < 0)) { //dotarl do pierwszego elementu, ale juz nie moze sie cofac, by schodzic w dol od nizszej publikacji...
								//...wiec zaczyna od poczatku
								xx = 0;
								x = 0;
							}
							else{
								x += 1; //jak nie dotarl do indeksu 0 w wektorze publikacji, to normalnie schodzi dalej
							}
						}
						//cytowania obce:	
						for(int m = 0; m < C_obce_t[i][j]; m ++){
							tablica.push_back(tablica[rand() % (tablica.size() - C_auto_t[i][j] - m)] ); //cytowania obce PREFERENCYJNIE
							// - C_auto_t[i][j] - by nie losowalo z dodanych przed chwila autocytowan
							cyt_obce.push_back(tablica[tablica.size() - 1]);
						}
						
						
						/*for(int f = 0; f < tablica.size(); f++){
							cout<<tablica[f]<<" ";
						}
						cout<<endl;*/	
					}
					
					//przypadek, gdy sumaryczna liczba publikacji do (j-1)-tego kroku jest mniejsza niż liczba autocytowan w j-tym kroku
					else if((j != 0) && (sum_of_elems < C_auto_t[i][j]) && ((sum_of_elems + N_t[i][j]) >= C_auto_t[i][j])){ 
						//cout<<"dwa"<<endl;
						//najpierw publikacje
						for(int k = 0; k < N_t[i][j]; k++){
							N.push_back(N.back() + 1);
							tablica.push_back(N[N.size()- 1]);
						}
						//cytowania własne:
						int x = 0; //iteracja "w dol wektora publikacji"
						int xx = 0; //iteracja, dzieki ktorej wroci do -1 ostatniej publikacji i znowu bedzie schodzil "w dol wektora publikacji"
						for(int l = 0; l < C_auto_t[i][j]; l++){
							tablica.push_back(N[N.size() - 1 - x - xx]); //N.size() - 1 = ostatni element w wektorze, - x = schodzi w dol, - xx = nizsze wartosci
							cyt_wlasne.push_back(tablica[tablica.size() - 1]);	
							
							int temp;
							temp = N.size() - 1 - xx - 1; //czy jest mozliwosc "cofnac sie" w wektorze publikacji, by ponownie schodzic w dol = xx - 1

							if(((N.size() - 1 - x - xx) == 0) && (temp >= 0)){ //dotarl do pierwszego elementu (indeks 0) wektora publikacji i moze sie cofnac, bo nie wyjdzie poza wektor
								xx +=1;	
								x = 0; //czyli cofa sie na koniec wektora publikacji i schodzi w dol, ale od dodatkowego -1 (xx)
							}
							else if(((N.size() - 1 - x - xx) == 0) && (temp < 0)) { //dotarl do pierwszego elementu, ale juz nie moze sie cofac, by schodzic w dol od nizszej publikacji...
								//...wiec zaczyna od poczatku
								xx = 0;
								x = 0;
							}
							else{
								x += 1; //jak nie dotarl do indeksu 0 w wektorze publikacji, to normalnie schodzi dalej
							}					
						}
						//cytowania obce:	
						for(int m = 0; m < C_obce_t[i][j]; m ++){
							tablica.push_back(tablica[rand() % (tablica.size() - C_auto_t[i][j] - m)] ); //cytowania obce PREFERENCYJNIE
							// - C_auto_t[i][j] - by nie losowalo z dodanych przed chwila autocytowan
							cyt_obce.push_back(tablica[tablica.size() - 1]);
						}
						
						/*for(int f = 0; f < tablica.size(); f++){
							cout<<tablica[f]<<" ";
						}
						cout<<endl;*/
					}
					
					//przypadek, gdy sumaryczna liczba publikacji do j-tego kroku jest mniejsza niż liczba autocytowan w j-tym kroku
					else if((j != 0) && ((sum_of_elems + N_t[i][j]) < C_auto_t[i][j])){
						//cout<<"trzy"<<endl;
						//najpierw publikacje
						for(int k = 0; k < N_t[i][j]; k++){
							N.push_back(N.back() + 1);
							tablica.push_back(N[N.size()- 1]);
						}
						//cytowania własne:
						int x = 0; //iteracja "w dol wektora publikacji"
						int xx = 0; //iteracja, dzieki ktorej wroci do -1 ostatniej publikacji i znowu bedzie schodzil "w dol wektora publikacji"
						for(int l = 0; l < C_auto_t[i][j]; l++){
							tablica.push_back(N[N.size() - 1 - x - xx]); //N.size() - 1 = ostatni element w wektorze, - x = schodzi w dol, - xx = nizsze wartosci
							cyt_wlasne.push_back(tablica[tablica.size() - 1]);	
							
							int temp;
							temp = N.size() - 1 - xx - 1; //czy jest mozliwosc "cofnac sie" w wektorze publikacji, by ponownie schodzic w dol = xx - 1

							if(((N.size() - 1 - x - xx) == 0) && (temp >= 0)){ //dotarl do pierwszego elementu (indeks 0) wektora publikacji i moze sie cofnac, bo nie wyjdzie poza wektor
								xx +=1;	
								x = 0; //czyli cofa sie na koniec wektora publikacji i schodzi w dol, ale od dodatkowego -1 (xx)
							}
							else if(((N.size() - 1 - x - xx) == 0) && (temp < 0)) { //dotarl do pierwszego elementu, ale juz nie moze sie cofac, by schodzic w dol od nizszej publikacji...
								//...wiec zaczyna od poczatku
								xx = 0;
								x = 0;
							}
							else{
								x += 1; //jak nie dotarl do indeksu 0 w wektorze publikacji, to normalnie schodzi dalej
							}
						}
						//cytowania obce:	
						for(int m = 0; m < C_obce_t[i][j]; m ++){
							tablica.push_back(tablica[rand() % (tablica.size() - C_auto_t[i][j] - m)] ); //cytowania obce PREFERENCYJNIE
							// - C_auto_t[i][j] - by nie losowalo z dodanych przed chwila autocytowan
							cyt_obce.push_back(tablica[tablica.size() - 1]);
						}
						
						/*for(int f = 0; f < tablica.size(); f++){
							cout<<tablica[f]<<" ";
						}
						cout<<endl;*/	
					}
					
					//kolejne/pozostale etapy symulacji - najpierw cytowania, potem publikacje
					else{ 
						//cout<<"cztery"<<endl;
						//najpierw cytowania:
						//własne:
						for(int l = 0; l < C_auto_t[i][j]; l++){
							tablica.push_back(N[(N.size()-1) - l]); //autocytowania do C_auto_t[i][j] OSTATNICH PRAC
							//tutaj juz bez -1, bo publikacje dochodza na koniec kroku
							cyt_wlasne.push_back(tablica[tablica.size() - 1]);
						}
						//obce:
						for(int m = 0; m < C_obce_t[i][j]; m ++){ 
							tablica.push_back(tablica[rand() % (tablica.size() - C_auto_t[i][j] - m)] ); //cytowania obce PREFERENCYJNIE
							// - C_auto_t[i][j] - by nie losowalo z dodanych przed chwila autocytowan
							cyt_obce.push_back(tablica[tablica.size() - 1]);
						}
						//kolejne publikacje:
						if(j < N_t[i].size()){ //by w przeciwnym wypadku nie dodawalo nie wiadomo czego
							for(int k = 0; k < N_t[i][j]; k++){
								N.push_back(N.back() + 1);
								tablica.push_back(N[N.size()- 1]); 
							}
						}
						
						/*for(int f = 0; f < tablica.size(); f++){
							cout<<tablica[f]<<" ";
						}
						cout<<endl;*/
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
			plik_wynikowy_auto.close();
			plik_wynikowy_obce.close();
			plik_wynikowy_czas.close();	
		}
	}
	return 0;
}
