#include <iostream>
#include <vector>
#include <random>
#include <string>

struct chromosomy_struct // structura do vektora ze stringiem na nazwe chromosomu i kod chromosomu w postaci binarnej <0,31>
{
	std::string nr_chromosomu;
	int kod_chromosomu_tab[5];
	chromosomy_struct(std::string nr_chromosomu_x, int kod_chromosomu_tab_x[5])
	{
		nr_chromosomu = nr_chromosomu_x;
		kod_chromosomu_tab[0] = kod_chromosomu_tab_x[0];
		kod_chromosomu_tab[1] = kod_chromosomu_tab_x[1];
		kod_chromosomu_tab[2] = kod_chromosomu_tab_x[2];
		kod_chromosomu_tab[3] = kod_chromosomu_tab_x[3];
		kod_chromosomu_tab[4] = kod_chromosomu_tab_x[4];
	}
};

std::vector <chromosomy_struct> chromosomy_vector; // vector na poczatkowe chromosomy i chromosomy do selekcji
std::vector <chromosomy_struct> chromosomy_vector_2; // vector na wyselekcjonowane chromosomy
std::vector <chromosomy_struct> chromosomy_vector_poczatek; // vector na startowe chromosomy
std::random_device rd; // deklarowanie seeda do funkcji z klasy random

void funkcjaPrzystosowania(int a,int b, int c, int d, int x[6], int tbf[6])
{
	int wynik;
	for (int i = 0; i < 6; i++)
	{
		wynik = (a * (x[i]* x[i] * x[i])) + (b * (x[i]* x[i])) + (c * x[i]) + d; // obliczanie f przystosowania
		tbf[i] = wynik;															 // zapisanie funkcji przystosowania do tablicy
	}
}

void losowanieChromosomow(int tab_chromosomy_losowanie[5])
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0,1); //ustawianie zakresu losowania na 0-1
	int liczba_losowanie;
	for (int i = 0; i < 5; i++)
	{
		liczba_losowanie = dist(gen);		   //losowanie elementu chromosomu
		tab_chromosomy_losowanie[i] = liczba_losowanie; // zapisanie chromosomu do tablicy
	}
}

void konwertowanieBinToDec(std::vector <chromosomy_struct> chromosomy_vector_to_dec, int bin_to_dec[6])
{
	int d;
	int pot;
	int liczba_bin_to_dec;
	for (int i = 0; i < 6; i++)
	{
		liczba_bin_to_dec = 0;
		d = 0; // zmienna sluzaca jako wykladnik
		for (int j = 4; j>=0; j--) // konwertowanie z bin na dec
		{
			if (chromosomy_vector_to_dec[i].kod_chromosomu_tab[j] == 1) // sprawdzanie czy element chromosomu jest rowny 1
			{
				pot = pow(2, d);										
				liczba_bin_to_dec = liczba_bin_to_dec + pot;
			}
			d++;
		}
		bin_to_dec[i] = liczba_bin_to_dec; // zapisanie wartosci dec do tablicy
	}

}

void koloFortunyObliczenia(int tab_dec_kf[6], double kolofortunyzakresy[6], int tab[1]) // liczenie zakresow do ruletki
{
	int chromosomy_dec_max = 0;
	double dziwna;
	for (int i = 0; i < 6; i++) // dodawanie wszystkich f przystosowania
	{
		chromosomy_dec_max = chromosomy_dec_max + tab_dec_kf[i]; 
	}
	for (int i = 0; i < 6; i++) // obliczanie dla kazdego chromosomu zakresu
	{
		dziwna = tab_dec_kf[i];
		dziwna = dziwna * 10;
		kolofortunyzakresy[i] = dziwna/chromosomy_dec_max; // zapisanie zakresu do tablicy
	}
	tab[0] = chromosomy_dec_max;							// zapisanie sumy f funkcji aby korzystac w warunku stopu
	
}

int koloFortunyLosowanie(double kolofortunyzakresyl[6]) // ruletka
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 10); // ustawianie zakresu losowania 0-10
	double los_kolo_fortuny = dist(gen);			// losowanie w kole fortuny
	bool koniec = false;
	int i = 0;
	double zakres_od=0;
	double zakres_do=kolofortunyzakresyl[0];
	int a=0;
	while(!koniec)
	{
		if (zakres_od <= los_kolo_fortuny and los_kolo_fortuny <= zakres_do) // jesli miesci sie w zakresie zapisanie numeru chromosomu
		{
			a = i;
			koniec = true;
		}
		i++;
		//zmienianie zakresu
		zakres_od = zakres_do;
		zakres_do = zakres_do + kolofortunyzakresyl[i];
	}
	return a; // zwracanie numeru wylosowanego chromosomu
}


int main()
{	
	//generowanie i ustawianie losowania liczb z klasy random
	std::mt19937 gen_1(rd());
	std::mt19937 gen_2(rd());
	std::uniform_int_distribution<> dist_1(1, 4);
	std::uniform_int_distribution<> dist_4(0, 4);
	std::uniform_int_distribution<> dist_2(0, 100);
	std::uniform_int_distribution<> dist_3(0, 5);
	// zmienne i tablice
	int a, b, c, d; int it_end = 0; int los_n, los_n_1, los_n_0; int zamiana_1 = 0; int zamiana_2 = 0; int losowanie_chrom = 0;
	int losowa_n = 0;
	int max_f_1 = 0; int max_f_2 = 0;
	int los_pm, Pk_max, Pm_max, los_pk;
	int tab[5]; int tab_dec[6]; int tbf[6]; int max_f[1];
	bool test = false;
	bool test_2 = false;
	double kolofortuny_zakresy[6];
	int proba = 0;
	// pobieranie danych
	std::cout << "Podaj wspolczynniki a, b, c, d: " << std::endl;
	std::cin >> a >> b >> c >> d; system("cls");
	std::cout << "Podaj wspolczynnik Poziomu krzyzowania:" << std::endl;
	std::cin >> Pk_max; system("cls");
	std::cout << "Podaj wspolczynnik Poziomu mutacji:" << std::endl;
	std::cin >> Pm_max; system("cls");
	// koniec pobierania danych

	// dodawanie losowych chromosomow do vectora
	for (int i = 0; i < 6; i++)
	{
		losowanieChromosomow(tab);
		chromosomy_vector.push_back(chromosomy_struct(("Ch"+ std::to_string(i)), tab));
		chromosomy_vector_poczatek.push_back(chromosomy_struct(("Ch" + std::to_string(i)), tab));
		chromosomy_vector_2.push_back(chromosomy_struct(("Ch" + std::to_string(i)), tab));

	}
	// koniec dodawania

	konwertowanieBinToDec(chromosomy_vector, tab_dec);
	funkcjaPrzystosowania(a, b, c, d, tab_dec, tbf);
	std::cout << "Chromosomy poczatkowe: " << std::endl;
	for (int i = 0; i < 6; i++)
	{

		std::cout << chromosomy_vector[i].nr_chromosomu << ": ";
		for (int j = 0; j < 5; j++)
		{
			std::cout << chromosomy_vector[i].kod_chromosomu_tab[j];
		}
		std::cout << " fenotyp: " << tab_dec[i] << " przystosowanie: " << tbf[i] << std::endl;
	}
	

	while (!test)
	{
		konwertowanieBinToDec(chromosomy_vector, tab_dec);
		funkcjaPrzystosowania(a, b, c, d, tab_dec, tbf);
		koloFortunyObliczenia(tbf, kolofortuny_zakresy, max_f);

		// Selekcja chromosomow do krzyzowania i mutacji
		for (int i = 0; i < 6; i++)
		{
			losowanie_chrom = koloFortunyLosowanie(kolofortuny_zakresy);
			for (int j = 0; j < 5; j++)
			{
				chromosomy_vector_2[i].kod_chromosomu_tab[j] = chromosomy_vector[losowanie_chrom].kod_chromosomu_tab[j];
			}
		}
		// Koniec selekcji

		// krzyzowanie chromosomow
		for (int i = 0; i < 3; i++)
		{
			los_pk = dist_2(gen_1);			// losowanie Pk
			los_n = dist_1(gen_1);			// losowanie n
			test_2 = false;					
			while (!test_2)
			{
				los_n_0 = dist_3(gen_1);	// losowanie pierwszego chromosomu
				los_n_1 = dist_3(gen_2);	// losowanie drugiego chromosomu
				if (los_n_0 != los_n_1)		// nie mozna krzyzowac tego samego 
				{
					test_2 = true;
				}
			}
			if (Pk_max > los_pk)
			{
				for (int k = 4; k >= los_n; k--) // krzyzowanie chromosomow
				{
					zamiana_1 = chromosomy_vector_2[los_n_0].kod_chromosomu_tab[k];
					zamiana_2 = chromosomy_vector_2[los_n_1].kod_chromosomu_tab[k];
					chromosomy_vector_2[los_n_0].kod_chromosomu_tab[k] = zamiana_2;
					chromosomy_vector_2[los_n_1].kod_chromosomu_tab[k] = zamiana_1;
				}
			}

		}
		// koniec krzyzowania

		// Mutacja chromosomow
		for (int i = 0; i < 6; i++)
		{
			los_pm = dist_2(gen_1); // losowanie Pm
			los_n = dist_4(gen_1);	// losowanie n
			if (los_pm < Pm_max)	// jesli wylosowana Pm miesci sie w zakresie
			{
				if (chromosomy_vector_2[i].kod_chromosomu_tab[los_n] == 1)
				{
					chromosomy_vector_2[i].kod_chromosomu_tab[los_n] = 0;
				}
				else if (chromosomy_vector_2[i].kod_chromosomu_tab[los_n] == 0)
				{
					chromosomy_vector_2[i].kod_chromosomu_tab[los_n] = 1;
				}
			}
		}
		// koniec mutacji

		// Podmienianie chromosomow
		for (int i = 0; i < 6; i++) 
		{
			for (int j = 0; j < 5; j++)
			{
				chromosomy_vector[i].kod_chromosomu_tab[j] = chromosomy_vector_2[i].kod_chromosomu_tab[j];
			}
		}
		// koniec podmieniania

		proba = proba + 1; // liczenie iteracji

		// Zatrzymywanie petli
		konwertowanieBinToDec(chromosomy_vector, tab_dec);
		if (tab_dec[0] == tab_dec[1] && tab_dec[0] == tab_dec[2] && tab_dec[0] == tab_dec[3] && tab_dec[0] == tab_dec[4] && tab_dec[0] == tab_dec[5]) // warunek ze wszystkie musza byc takie same
		{
			if (max_f_1 < max_f[0]) // szukanie najwiekszej f przystosowania
			{
				max_f_1 = max_f[0]; // nadpisywanie jesli znalazlo wieksza
				it_end = 0;			// zerowanie licznika
			}
			else if (max_f_1 == max_f[0]) // sprawdzanie ile razy wypadla najwieksza
			{
				it_end++;				// zwiekszanie jesli ponownie wypadla najwieksza
				if (it_end == 5)		// jesli wypadla 5 razy zatrzymanie petli
				{
					test = true;
				}
			}
		}
		if (proba>49999) // zatrzymanie petli gdy liczba iteracji wyniesie 50000
		{
			test = true;
		}
		// koniec zatrzymywania petli

	}

	konwertowanieBinToDec(chromosomy_vector, tab_dec);
	funkcjaPrzystosowania(a, b, c, d, tab_dec, tbf);
	// wyswietlanie chromosomow
	std::cout << "Chromosomy koncowe: " << std::endl;
	for (int i = 0; i < 6; i++)
	{

		std::cout << chromosomy_vector[i].nr_chromosomu << ": ";
		for (int j = 0; j < 5; j++)
		{
			std::cout << chromosomy_vector[i].kod_chromosomu_tab[j];
		}
		std::cout << " fenotyp: " << tab_dec[i] << " przystosowanie: " << tbf[i] << std::endl;
	}
	// koniec wyswietlania
	std::cout << std::endl;
	std::cout << proba << " - liczba iteracji"; // wyswietlanie iteracji

	return 0;
}