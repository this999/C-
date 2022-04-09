#include <iostream>
#include <vector> 
#include <math.h>

struct wspolrzedne // struktura zapisujaca wpolrzedne do wektora
{
	int a;
	int b;
	wspolrzedne(int a_1, int b_1)
	{
		a = a_1;
		b = b_1;
	}
};

std::vector <wspolrzedne> wspolrzedne_all; // wektor do przechowywania wspolrzednych na ktorych bede operowac
std::vector <wspolrzedne> wspolrzedne_kolejnosc; // wektor z najblizszymi sasiadami
std::vector <wspolrzedne> wspolrzedne_zapis; // przechowanie wszystkich wspolrzednych
std::vector <double> odleglosc_t; // wektor do odleglosci


int main()
{
	int w_1, w_2; // zmienna na wspolrzedne
	double j = 0;
	double c = 0;
	double odleglosc_max=0;
	int zmiana = 0; // zmienna do zapisania kolejnosci podmieniania w wektorze
	int rozmiar_t = 0; 
	std::cout << "Ile wspolrzednych ma graf?" << std::endl;
	int graf_w = 0; // ilosc wspolrzednych w grafie
	std::cin >> graf_w; system("cls");
	int loop = graf_w - 1;
	rozmiar_t = graf_w - 2;
	double min;
	std::cout << "Pierwsze wspolrzedne to wspolrzedne startowe" << std::endl;



	for (int i = 0; i < graf_w; i++) // zapisywanie wspolrzednych do wektora
	{
		std::cout << "Podaj wspolrzedna x: " << std::endl;
		std::cin >> w_1; system("cls");
		std::cout << "Podaj wspolrzedna y: " << std::endl;
		std::cin >> w_2; system("cls");
		wspolrzedne_all.push_back(wspolrzedne(w_1, w_2)); // dodawanie wspolrzednych do wektora obliczeniowego
		wspolrzedne_zapis.push_back(wspolrzedne(w_1, w_2)); // dodawanie wspolrzednych do wektora zapisu
	}

	wspolrzedne_kolejnosc.push_back(wspolrzedne(wspolrzedne_all[0].a, wspolrzedne_all[0].b)); // zapisanie wspolrzednej startowej

	for (int w = 0; w < loop; w++)
	{
		rozmiar_t = graf_w - 2; // zmienna do tab dynamicznej na wyniki odleglosci

		for (int i = 1; i < graf_w; i++) // oblicza odleglosc i zapisuje do tablicy
		{
			c = sqrt(pow((wspolrzedne_all[i].a - wspolrzedne_all[0].a), 2) + (pow((wspolrzedne_all[i].b - wspolrzedne_all[0].b), 2)));
			odleglosc_t.push_back(c);
		}

		min = odleglosc_t[0]; // wartosc minimalna poczatkowa

		for (int k = 1; k < graf_w - 1; k++) // szuka najmniejszej odleglosci
		{
			if (min > odleglosc_t[k])
			{
				min = odleglosc_t[k];
			}
		}

		for (int l = 1; l < graf_w; l++) // sprawdza ktore bylo najmniejsze
		{
			j = pow((wspolrzedne_all[l].a - wspolrzedne_all[0].a), 2) + pow((wspolrzedne_all[l].b - wspolrzedne_all[0].b), 2);
			if (min == sqrt(j))
			{
				zmiana = l;
			}

		}

		wspolrzedne_kolejnosc.push_back(wspolrzedne(wspolrzedne_all[zmiana].a, wspolrzedne_all[zmiana].b)); // zapisanie nastepnego elementu do wektora kolejnosc
		wspolrzedne_all[0].a = wspolrzedne_all[zmiana].a; // nadpisanie nowego punktu startowego x
		wspolrzedne_all[0].b = wspolrzedne_all[zmiana].b; // nadpisanie nowego punktu startowego y
		wspolrzedne_all.erase(wspolrzedne_all.begin() + zmiana); // usuniecie duplikatu
		odleglosc_max = odleglosc_max + min;
		graf_w--;
		odleglosc_t.clear();
		
	}
	wspolrzedne_kolejnosc.push_back(wspolrzedne(wspolrzedne_kolejnosc[0].a, wspolrzedne_all[0].b));
	c = sqrt(pow((wspolrzedne_kolejnosc[loop].a - wspolrzedne_kolejnosc[0].a), 2) + (pow((wspolrzedne_kolejnosc[loop].b - wspolrzedne_kolejnosc[0].b), 2)));
	odleglosc_max = odleglosc_max + c;

	for (int z = 0; z < loop + 1; z++) // wyswietlanie wspolrzednych
	{
		std::cout << "[" << z + 1 << "] " << " (" << wspolrzedne_zapis[z].a << "," << wspolrzedne_zapis[z].b<<")" << std::endl;
	}
	std::cout << "pokonana odleglosc wynosi: " << odleglosc_max<<std::endl;

	std::cout << "najblizszy sasiad: " << std::endl;

	for (int v = 0; v <= loop; v++) // wyswietlanie kolejnosci wspolrzednych wspolrzednych
	{
		std::cout << "(" << wspolrzedne_kolejnosc[v].a << "," << wspolrzedne_kolejnosc[v].b << ")";
		if (v < loop)
		{
			std::cout << "->";
		}
	}
	std::cout << std::endl;
	//std::cout << "pokonana odleglosc wynosi: " << odleglosc_max;
	return 0;
}