#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>

struct przedmioty_struct // struktura do klasyfikacji przedmiotow
{
	int numer_przedmiotu;
	int waga_przedmiotu;
	int wartosc_przedmiotu;

	przedmioty_struct(int nr_p, int wg_p, int wa_p)
	{
		numer_przedmiotu = nr_p;
		waga_przedmiotu = wg_p;
		wartosc_przedmiotu = wa_p;
	}
};

struct chromosomy_struct // struktura do vektora ze stringiem na nazwe chromosomu i kod chromosomu w postaci binarnej <0,31>
{
	std::string nr_chromosomu;
	int kod_chromosomu_tab[10];
	chromosomy_struct(std::string nr_chromosomu_x, int kod_chromosomu_tab_x[10])
	{
		nr_chromosomu = nr_chromosomu_x;
		kod_chromosomu_tab[0] = kod_chromosomu_tab_x[0];
		kod_chromosomu_tab[1] = kod_chromosomu_tab_x[1];
		kod_chromosomu_tab[2] = kod_chromosomu_tab_x[2];
		kod_chromosomu_tab[3] = kod_chromosomu_tab_x[3];
		kod_chromosomu_tab[4] = kod_chromosomu_tab_x[4];
		kod_chromosomu_tab[5] = kod_chromosomu_tab_x[5];
		kod_chromosomu_tab[6] = kod_chromosomu_tab_x[6];
		kod_chromosomu_tab[7] = kod_chromosomu_tab_x[7];
		kod_chromosomu_tab[8] = kod_chromosomu_tab_x[8];
		kod_chromosomu_tab[9] = kod_chromosomu_tab_x[9];
	}
};

std::vector <przedmioty_struct> przedmioty_vector;
std::vector <chromosomy_struct> chromosomy_vector_obliczenia;
std::vector <chromosomy_struct> chromosomy_vector_glowny;
std::random_device rd;

std::vector<przedmioty_struct> zapisPrzedmiotow() // zapis przedmiotow z pliku ustawiony sztywno na 10 elementow
{
	std::vector<przedmioty_struct> zapis_przedmiotow;
	int a_zapis, b_zapis, c_zapis;
	std::ifstream plik_przedmioty;
	plik_przedmioty.open("dane.txt");
	if (!plik_przedmioty.good())
	{
		std::cout << "Nie ma pliku!!!!!" << std::endl;
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			plik_przedmioty >> a_zapis;
			plik_przedmioty >> b_zapis;
			plik_przedmioty >> c_zapis;
			zapis_przedmiotow.push_back(przedmioty_struct(a_zapis, b_zapis, c_zapis));
		}
	}
	plik_przedmioty.close();
	return zapis_przedmiotow;
}

void losowanieChromosomow(int tab_chromosomy_losowanie[10])
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 1); //ustawianie zakresu losowania na 0-1
	int liczba_losowanie;
	for (int i = 0; i < 10; i++)
	{
		liczba_losowanie = dist(gen);		   //losowanie elementu chromosomu
		tab_chromosomy_losowanie[i] = liczba_losowanie; // zapisanie chromosomu do tablicy
	}
}

void funkcjaPrzystosowania(std::vector <chromosomy_struct> chromosomy_funkcja, int funkcja_przystosowania_tab[6])
{
	std::vector <przedmioty_struct> przedmioty_f_p = zapisPrzedmiotow();
	int wynik;
	
	for (int i = 0; i < 6; i++)
	{
		wynik = 0;
		for (int j = 0; j < 10; j++)
		{
			if (chromosomy_funkcja[i].kod_chromosomu_tab[j] == 0)
			{
				wynik = wynik + (przedmioty_f_p[j].wartosc_przedmiotu * 0);
			}
			else if (chromosomy_funkcja[i].kod_chromosomu_tab[j] == 1)
			{
				wynik = wynik + (przedmioty_f_p[j].wartosc_przedmiotu * 1);
			}
		}
		funkcja_przystosowania_tab[i] = wynik;
	}
}

std::vector <chromosomy_struct> zmianaPrzedmiotuWaga(std::vector <chromosomy_struct> chromosomy_zmiana, int waga_tab[6], int max_waga) // sprawdzanie i poprawianie wagi przedmiotu
{
	bool test = false;
	int i_zmiana = 0;
	int los;
	int wynik;
	std::vector <przedmioty_struct> przedmioty_f_p = zapisPrzedmiotow();
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 9);
	while (!test)
	{
		for (int i = i_zmiana; i < 6; i++)
		{
			wynik = 0;
			for (int j = 0; j < 10; j++)
			{
				wynik = wynik + (przedmioty_f_p[j].waga_przedmiotu * chromosomy_zmiana[i].kod_chromosomu_tab[j]);
			}
			waga_tab[i] = wynik;
		}

		if (waga_tab[i_zmiana] > max_waga)
		{
			los = dist(gen);
			chromosomy_zmiana[i_zmiana].kod_chromosomu_tab[los] = 0;

		}
		else
		{
			i_zmiana++;
		}

		if (i_zmiana == 6)
		{
			test = true;
		}
	}
	return chromosomy_zmiana;
}

void koloFortunyObliczenia(int funkcja_przystosowania_tab[6], double kolo_fortuny_zakresy[6], int tab[1]) // liczenie zakresow do ruletki
{
	int max_funkcji = 0;
	double zakres;
	for (int i = 0; i < 6; i++) // dodawanie wszystkich f przystosowania
	{
		max_funkcji = max_funkcji + funkcja_przystosowania_tab[i];
	}
	for (int i = 0; i < 6; i++) // obliczanie dla kazdego chromosomu zakresu
	{
		zakres = funkcja_przystosowania_tab[i];
		zakres = zakres * 10;
		kolo_fortuny_zakresy[i] = zakres / max_funkcji; // zapisanie zakresu do tablicy
	}
	tab[0] = max_funkcji;							// zapisanie sumy funkcji aby korzystac w warunku stopu

}

int koloFortunyLosowanie(double kolo_fortuny_zakresy[6]) // ruletka
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 10); // ustawianie zakresu losowania 0-10
	double los_kolo_fortuny = dist(gen);			// losowanie w kole fortuny
	bool koniec = false;
	int i = 0;
	double zakres_od = 0;
	double zakres_do = kolo_fortuny_zakresy[0];
	int a = 0;
	while (!koniec)
	{
		if (zakres_od <= los_kolo_fortuny and los_kolo_fortuny <= zakres_do) // jesli miesci sie w zakresie zapisanie numeru chromosomu
		{
			a = i;
			koniec = true;
		}
		i++;
		//zmienianie zakresu
		zakres_od = zakres_do;
		zakres_do = zakres_do + kolo_fortuny_zakresy[i];
	}
	return a; // zwracanie numeru wylosowanego chromosomu
}

int main()
{
	std::mt19937 gen(rd()); // losowanie liczb
	std::uniform_int_distribution<> dist_1(1, 8);
	std::uniform_int_distribution<> dist_4(0, 9);
	std::uniform_int_distribution<> dist_2(0, 100);
	std::uniform_int_distribution<> dist_3(0, 5);
	//Deklaracja zmiennych
	int max_fukcji[1], tab_chromosomy_losowanie[10], funkcja_przystosowania_tab[6], waga_tab[6];
	double kolo_fortuny_zakresy[6];
	int max_waga, Pk, Pm, Los, los_selekcja, los_Pk, los_Pm, los_n_0, los_n_1, zamiana_1, zamiana_2, iteracja = 0, max_f = 0, it_end=0;
	bool test=false, test_2=false;
	//Koniec Deklaracji
	
	//Pobieranie danych
	std::cout << "Podaj Poziom krzyzowania: " << std::endl;
	std::cin >> Pk; system("cls");
	std::cout << "Podaj Poziom mutacji: " << std::endl;
	std::cin >> Pm; system("cls");
	std::cout << "Podaj maksymalna wage plecaka: " << std::endl;
	std::cin >> max_waga; system("cls");
	//Koniec pobierania danych

	//Dodawanie losowych chromosomow do vectora
	for (int i = 0; i < 6; i++)
	{
		losowanieChromosomow(tab_chromosomy_losowanie);
		chromosomy_vector_obliczenia.push_back(chromosomy_struct(("Ch" + std::to_string(i)), tab_chromosomy_losowanie));
		chromosomy_vector_glowny.push_back(chromosomy_struct(("Ch" + std::to_string(i)), tab_chromosomy_losowanie));
	}
	//Koniec dodawania losowych chromosomow do vectora

	funkcjaPrzystosowania(chromosomy_vector_glowny,funkcja_przystosowania_tab); // obliczanie funkcji przystosowania
	chromosomy_vector_glowny = zmianaPrzedmiotuWaga(chromosomy_vector_glowny, waga_tab, max_waga); // obliczanie wagi i sprawdzanie czy nie przekracza + zmiana jesli przekracza

	//Wyswietlanie danych poczatkowych
	std::cout << "Poczatkowe chromosomy: " << std::endl;
	for (int i = 0; i < 6; i++)
	{

		std::cout << chromosomy_vector_glowny[i].nr_chromosomu << ": ";
		for (int j = 0; j < 10; j++)
		{
			std::cout << chromosomy_vector_glowny[i].kod_chromosomu_tab[j];
		}
		std::cout << " Przystosowanie: " << funkcja_przystosowania_tab[i] <<" Waga: "<<waga_tab[i]<< std::endl;
	}
	//Koniec wyswietlania danych poczatkowych

	while (!test)
	{
		funkcjaPrzystosowania(chromosomy_vector_glowny, funkcja_przystosowania_tab);
		chromosomy_vector_glowny = zmianaPrzedmiotuWaga(chromosomy_vector_glowny, waga_tab, max_waga);
		koloFortunyObliczenia(funkcja_przystosowania_tab, kolo_fortuny_zakresy, max_fukcji);

		// Zatrzymywanie petli
		if (funkcja_przystosowania_tab[0]==funkcja_przystosowania_tab[1] && funkcja_przystosowania_tab[0] == funkcja_przystosowania_tab[2] && funkcja_przystosowania_tab[0] == funkcja_przystosowania_tab[3] && funkcja_przystosowania_tab[0] == funkcja_przystosowania_tab[4] && funkcja_przystosowania_tab[0] == funkcja_przystosowania_tab[5])
		{
			if (max_f < max_fukcji[0]) // szukanie najwiekszej f przystosowania
			{
				max_f = max_fukcji[0]; // nadpisywanie jesli znalazlo wieksza
				it_end = 0;			// zerowanie licznika
			}
			else if (max_f == max_fukcji[0]) // sprawdzanie ile razy wypadla najwieksza
			{
				it_end++;				// zwiekszanie jesli ponownie wypadla najwieksza
				if (it_end == 5)		// jesli wypadla 5 razy zatrzymanie petli
				{
					test = true;
					continue;
				}
			}
		}
		if (iteracja > 49999) // zatrzymanie petli gdy liczba iteracji wyniesie 50000
		{
			test = true;
		}
		// Koniec zatrzymywania petli

		// Selekcja chromosomow do krzyzowania i mutacji
		for (int i = 0; i < 6; i++)
		{
			los_selekcja = koloFortunyLosowanie(kolo_fortuny_zakresy);
			for (int j = 0; j < 10; j++)
			{
				chromosomy_vector_obliczenia[i].kod_chromosomu_tab[j] = chromosomy_vector_glowny[los_selekcja].kod_chromosomu_tab[j];
			}
		}
		// Koniec selekcji

		// Krzyzowanie chromosomow
		for (int i = 0; i < 3; i++)
		{
			los_Pk = dist_2(gen);			// losowanie Pk
			Los = dist_1(gen);			// losowanie n
			test_2 = false;
			while (!test_2)
			{
				los_n_0 = dist_3(gen);	// losowanie pierwszego chromosomu
				los_n_1 = dist_3(gen);	// losowanie drugiego chromosomu
				if (los_n_0 != los_n_1)		// nie mozna krzyzowac tego samego 
				{
					test_2 = true;
				}
			}
			if (Pk > los_Pk)
			{
				for (int k = 8; k >= Los; k--) // krzyzowanie chromosomow
				{
					zamiana_1 = chromosomy_vector_obliczenia[los_n_0].kod_chromosomu_tab[k];
					zamiana_2 = chromosomy_vector_obliczenia[los_n_1].kod_chromosomu_tab[k];
					chromosomy_vector_obliczenia[los_n_0].kod_chromosomu_tab[k] = zamiana_2;
					chromosomy_vector_obliczenia[los_n_1].kod_chromosomu_tab[k] = zamiana_1;
				}
			}

		}
		// Koniec krzyzowania

		// Mutacja chromosomow
		for (int i = 0; i < 6; i++)
		{
			los_Pm = dist_2(gen); // losowanie Pm
			Los = dist_4(gen);	// losowanie n
			if (los_Pm < Pm)	// jesli wylosowana Pm miesci sie w zakresie
			{
				if (chromosomy_vector_obliczenia[i].kod_chromosomu_tab[Los] == 1)
				{
					chromosomy_vector_obliczenia[i].kod_chromosomu_tab[Los] = 0;
				}
				else if (chromosomy_vector_obliczenia[i].kod_chromosomu_tab[Los] == 0)
				{
					chromosomy_vector_obliczenia[i].kod_chromosomu_tab[Los] = 1;
				}
			}
		}
		// Koniec mutacji

		// Podmienianie chromosomow
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				chromosomy_vector_glowny[i].kod_chromosomu_tab[j] = chromosomy_vector_obliczenia[i].kod_chromosomu_tab[j];
			}
		}
		// Koniec podmieniania

		chromosomy_vector_glowny = zmianaPrzedmiotuWaga(chromosomy_vector_glowny, waga_tab, max_waga);
		iteracja++;

	}
	funkcjaPrzystosowania(chromosomy_vector_glowny, funkcja_przystosowania_tab);
	std::cout << "Chromosomy koncowe: " << std::endl;
	for (int i = 0; i < 6; i++)
	{

		std::cout << chromosomy_vector_glowny[i].nr_chromosomu << ": ";
		for (int j = 0; j < 10; j++)
		{
			std::cout << chromosomy_vector_glowny[i].kod_chromosomu_tab[j];
		}
		std::cout << " Przystosowanie: " << funkcja_przystosowania_tab[i]<<" Waga: "<<waga_tab[i]<< std::endl;
	}
	// koniec wyswietlania
	std::cout << std::endl;
	std::cout << iteracja << " - liczba iteracji"; // wyswietlanie iteracji
	return 0;
}