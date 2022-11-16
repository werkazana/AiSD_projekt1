/*
Dla zadanej tablicy liczb ca³kowitych o rozmiarze n zawieraj¹cej wartoœci z przedzia³u [1, n-1] znajdŸ element powtarzaj¹cy siê.
Przyk³ad:
we = 1, 2, 3, 4, 4
-> 4
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std::chrono;


// deklaracje funkcji
std::vector<int> generuj_losowy_ciag(int n);
void wypisz_ciag(std::vector<int> arr);
std::vector<int> znajdz_duplikaty_histogram(std::vector<int> arr);
bool zawiera_element(std::vector<int> arr, int liczba);
std::vector<int> znajdz_duplikaty_2_petle(std::vector<int> arr);
void zapisz_ciag_do_pliku(std::string nazwa, std::vector<int> arr);
std::vector<int> odczytaj_ciag_z_pliku(std::string nazwa);
void zapisz_ciag_floatow_do_pliku(std::string nazwa_pliku, std::vector<double> arr);
void testy();


// zmienne globalne - do pomiaru czasu
high_resolution_clock::time_point start;
high_resolution_clock::time_point stop;
std::chrono::duration<double> czas;


/////////////////////////////////////////////////////////////////////
/* funkcja znajduje i zwraca elementy powtarzaj¹ce siê - algorytm histogramu */
/* Algorytm o zlozonosci O(2n) */
std::vector<int> znajdz_duplikaty_histogram(std::vector<int> arr)
{
    std::vector<int> wynik; // tablica zawieraj¹ca znalezione powtarzajace sie liczby
    std::vector<int> histogram(arr.size(), 0); // tablica histogramu wype³nione zerami

    for (size_t i = 0; i < arr.size(); i++)
    {
        int liczba = arr[i];
        histogram[liczba] ++; // zwiêkszanie elementu histogramu o 1
    }

    for (size_t i = 0; i < histogram.size(); i++)
    {
        if (histogram[i] > 1) // jesli wartosc > 1 to znaczy ze liczba sie powtarza co najmniej 2x
            wynik.push_back(i);
    }

    return wynik;
}


/////////////////////////////////////////////////////////////////////
/* funkcja znajduje i zwraca elementy powtarzaj¹ce siê - algorytm 2 zagniezdzonych petli */
/* Algorytm o zlozonosci O(n^2)-n */
std::vector<int> znajdz_duplikaty_2_petle(std::vector<int> arr)
{
    std::vector<int> wynik;

    for (size_t i = 0; i < arr.size(); i++)
    {
        int liczba1 = arr[i]; // pierwsza liczba do porównywania
        
        if (!zawiera_element(wynik, liczba1)) // sprawdzenie czy liczba1 nie jest ju¿ zapisana w tablicy powtorki jako powtarzajaca sie
        {
            for (size_t j = i + 1; j < arr.size(); j++)
            {
                int liczba2 = arr[j]; // druga liczba do porównania

                if (liczba1 == liczba2 && !zawiera_element(wynik, liczba2))
                    wynik.push_back(liczba1); // dodaj liczbe jesli liczba1==liczba2 i nie ma jej w tablicy powtorki
            }
        }
    }
    return wynik;
}


/////////////////////////////////////////////////////////////////////
/* funkcja sprawdza czy podana liczba wystêpuje w tablicy */
bool zawiera_element(std::vector<int> arr, int liczba)
{
    return std::find(arr.begin(), arr.end(), liczba) != arr.end();
}


/////////////////////////////////////////////////////////////////////
/* funkcja generuje tablice n elementów typu int z przedzia³u [1, n-1] */
std::vector<int> generuj_losowy_ciag(int n)
{
    std::vector<int> arr;
    int zakres = n - 1;

    for (size_t i = 0; i < n; i++)
    {
        int liczba = rand() % zakres + 1; // wygenerowana liczba losowa z zakresu [1, n-1]
        arr.push_back(liczba);
    }

    return arr;
}


/////////////////////////////////////////////////////////////////////
/* funkcja wypisuje wszystkie elementy tablicy */
void wypisz_ciag(std::vector<int> arr)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i];
        if (i < arr.size() - 1)
            std::cout << ", ";
    }

    std::cout << std::endl;
}


/////////////////////////////////////////////////////////////////////
/* funkcja zapisuje tablice elementów int do pliku */
void zapisz_ciag_do_pliku(std::string nazwa_pliku, std::vector<int> arr)
{
    std::fstream plik;
    plik.open(nazwa_pliku.c_str(), std::ios::out); // otworz plik o nazwie nazwa do zapisu (out)
    for (size_t i = 0; i < arr.size(); i++)
    {
        plik << arr[i] << " "; // wrzuc element tablicy do pliku
    }

    plik.close(); // zamknij plik
}


/////////////////////////////////////////////////////////////////////
/* funkcja odczytuje tablice elementów int z pliku */
std::vector<int> odczytaj_ciag_z_pliku(std::string nazwa_pliku)
{
    std::vector<int> arr;

    std::fstream plik;
    plik.open(nazwa_pliku.c_str(), std::ios::in); // otworz plik o nazwie nazwa do odczytu (in)
    
    int liczba;
    while (plik >> liczba)
    {
        arr.push_back(liczba);
    }

    plik.close();

    return arr;
}


/////////////////////////////////////////////////////////////////////
/* funkcja odczytuje tablice elementów float z pliku */
void zapisz_ciag_floatow_do_pliku(std::string nazwa_pliku, std::vector<double> arr)
{
    std::fstream plik;
    plik.open(nazwa_pliku.c_str(), std::ios::out);
    for (size_t i = 0; i < arr.size(); i++)
    {
        plik << arr[i] << std::endl;
    }

    plik.close();
}


/////////////////////////////////////////////////////////////////////
/* funkcja przeprowadza pomiary czasów obliczeñ dla obu algorytmów i ró¿nych wielkoœci tablic */
void testy()
{
    int arrLength = 100; // zmienna pomocnicza
    std::vector<int> arr; // tablica z wygenerowanymi liczbami
    int liczba_testow = 10;
    std::vector<int> wynik; // tablica z powtórzonymi liczbami
    std::vector<double> czas2Petle(liczba_testow); // czasy obliczen alg. 2 petle
    std::vector<double> czasHistogram(liczba_testow); // czasy obliczen alg. histogram
    std::string plik_we = "plik_we";
    std::string plik_wy = "plik_wy";
    for (int i = 0; i < liczba_testow; i++)
    {
        /* zmienne pomocnicze - nazwy plikow wejsciowych i wyjsciowych */
        std::string nazwa_we = plik_we + std::to_string(i) + ".txt";
        std::string nazwa_wy = plik_wy + std::to_string(i) + ".txt";

        arr = generuj_losowy_ciag(arrLength);
        zapisz_ciag_do_pliku(nazwa_we, arr);
    
        /* testy algorytmu 2 petle */
        start = high_resolution_clock::now();
        wynik = znajdz_duplikaty_2_petle(arr);
        stop = high_resolution_clock::now();
        czas = stop - start;
        czas2Petle[i] = czas.count();
        //std::cout << czasA[i] << "s\n";

        /* testy algorytmu histogram */
        start = high_resolution_clock::now();
        wynik = znajdz_duplikaty_histogram(arr);
        stop = high_resolution_clock::now();
        czas = stop - start;
        czasHistogram[i] = czas.count();
        //std::cout << czasB[i] << "s\n";

        //zapisz_ciag_do_pliku(nazwa_wy, wynik);

        // zwiekszenie rozmiarow tablicy
        arrLength = arrLength * 2;
    }

    zapisz_ciag_floatow_do_pliku("czasy_2Petle.txt", czas2Petle);
    zapisz_ciag_floatow_do_pliku("czasy_Histogram.txt", czasHistogram);
}



int main()
{
    // inicjalizacja generatora liczb losowych
    srand(time(NULL));

    std::cout << "Tablica naiwna:" << std::endl;
    std::vector<int> arr = {1, 2, 3, 4, 4, 5, 2};
    wypisz_ciag(arr);
    std::vector<int> wyniki = znajdz_duplikaty_2_petle(arr);
    std::cout << "Powtarzajace sie liczby: " << std::endl;
    wypisz_ciag(wyniki);
    std::cout << std::endl;

    int rozmiar_tablicy = 200;
    std::cout << "Tablica losowa:" << std::endl;
    arr = generuj_losowy_ciag(rozmiar_tablicy);
    wypisz_ciag(arr);
    std::cout << std::endl;
    zapisz_ciag_do_pliku("arr.txt", arr);
    //std::vector<int> farr = odczytaj_ciag_z_pliku("arr.txt");

    // 2 petle
    std::cout << "*** Algorytm: 2 petle" << std::endl;
    start = high_resolution_clock::now();
    std::vector<int> wyniki_2_petle = znajdz_duplikaty_2_petle(arr);
    stop = high_resolution_clock::now();
    czas = stop - start;
    std::cout << "Powtarzajace sie liczby:" << std::endl;
    wypisz_ciag(wyniki_2_petle);
    std::cout << "Czas obliczen: " << std::setw(9) << czas.count() << " s." << std::endl;
    zapisz_ciag_do_pliku("wyniki_2_petle.txt", wyniki_2_petle);
    std::cout << std::endl;

    // histogram
    std::cout << "*** Algorytm: histogram" << std::endl;
    start = high_resolution_clock::now();
    std::vector<int> wyniki_histogram = znajdz_duplikaty_histogram(arr);
    stop = high_resolution_clock::now();
    czas = stop - start;
    std::cout << "Powtarzajace sie liczby:" << std::endl;
    wypisz_ciag(wyniki_histogram);
    std::cout << "Czas obliczen: " << std::setw(9) << czas.count() << " s." << std::endl;
    zapisz_ciag_do_pliku("wyniki_histogram.txt", wyniki_histogram);
    std::cout << std::endl;

    // testy
    std::cout << "*** Testy - prosze czekac..." << std::endl;
    testy();
    std::cout << "Wyniki zostaly zapisane do plików." << std::endl;
}
