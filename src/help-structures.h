/** @file
 * Plik nagłówkowy zawierający pomocnicze struktury.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef __HELP_STRUCTURES_H__
#define __HELP_STRUCTURES_H__

#include "stdbool.h"

/**
 * Struktura przechowująca węzeł potrzebny do wyszukiwania
 * najkrótszej ścieżki w grafie.
 */
typedef struct Vertex Vertex;

/**
 * Struktura przechowująca listę odcinków dróg.
 */
typedef struct ListOfRoads ListOfRoads;

/**
 * Struktura przechowująca listę dróg krajowych.
 */
typedef struct ListOfRoutes ListOfRoutes;

/**
 * Struktura przechowująca odcinek drogi.
 */
typedef struct Road Road;

/**
 * Struktura przechowująca miasto.
 */
typedef struct City City;

/**
 * Struktura przechowująca drogę krajową.
 */
typedef struct Route Route;


/**
 * @brief Struktura przechowująca mapę dróg krajowych.
 * Struktura mapy zawiera:
 * tablicę wskaźników na miasta oraz ich liczbę i rozmiar tablicy;
 * wskaźniki na pierwszy i ostatni element listy zawierającej odcinki dróg;
 * wskaźniki na pierwszy i ostatni element listy zawierającej drogi krajowe.
 */
struct Map {
    int numberOfCities;       ///< liczba miast
    int sizeOfCitiesArray;    ///< rozmiar tablicy wskaźników na miasta
    City **cities;            ///< tablica wskaźników na miasta
    ListOfRoads *firstRoad;   ///< wskaźnik na pierwszą drogę
    ListOfRoads *lastRoad;    ///< wskaźnik na ostatnią drogę
    ListOfRoutes *firstRoute; ///< wskaźnik na pierwszą drogę krajową
    ListOfRoutes *lastRoute;  ///< wskaźnik na ostatnią drogę krajową
};

/**
 * @brief Struktura przechowująca węzeł potrzebny do wyszukiwania najkrótszej
 * ścieżki w grafie.
 * Struktura wierzchołka zawiera:
 * identyfikator;
 * odległość wierzchołka od źródła przy obliczniu najkrótszej ścieżki w grafie;
 * informację o tym, czy dane miasto, jednoznacznie połączone z wierzchołkiem,
 * zostało użyte w drodze krajowej;
 * wskaźnik na odcinek drogi, potrzebny do zbudowania drogi krajowej;
 * informację o roku budowy lub ostatniego remontu najstarszego odcinka drogi,
 * prowadzącego do danego wierzchołka;
 * informację o tym, czy w podanej ścieżce podczas tworzenia drogi wystąpił błąd.
 */
struct Vertex {
    int id;                 ///< identyfikator wierzchołka
    int distanceFromSource; ///< odległość wierzchołka od źródłą
    bool usedInRoute;       ///< informacja o tym, czy miasto jest użyte w drodze
    Road *road;             ///< wskaźnik na strukturę odcinka drogi
    int oldestYear;         ///< rok budowy lub ostatniego remontu najstarszej drogi
    bool error;             ///< informacja o błędzie
};

/**
 * @brief Struktura przechowująca listę odcinków dróg.
 * Struktura listy odcinków dróg zawiera:
 * wskaźnik na powiązany odcinek drogi;
 * wskaźnik na następny element listy.
 */
struct ListOfRoads {
    Road *road;        ///< wskaźnik na drogę
    ListOfRoads *next; ///< wskaźnik na następny element
};

/**
 * @brief Struktura przechowująca listę dróg krajowych.
 * Struktura listy dróg krajowych zawiera:
 * wskaźnik na powiązaną drogę krajową;
 * wskaźnik na następny element listy.
 */
struct ListOfRoutes {
    Route *route;       ///< wskaźnik na drogę krajową
    ListOfRoutes *next; ///< wskaźnik na następny element
};

/**
 * @brief Struktura przechowująca odcinek drogi.
 * Struktura odcinka drogi zawiera: 
 * wskaźniki na miasta, które dany odcinek łączy;
 * długość odcinka drogi;
 * rok budowy lub ostatniego remontu odcinka drogi;
 * zmienną określającą, czy dany odcinek drogi ma zostać usunięty,
 * używaną przy operacji usuwania odcinka drogi;
 * wskaźniki na pierwszą i ostatnią drogę krajową zawierającą dany odcinek.
 */
struct Road {
    City *cityA;              ///< wskaźnik na strukturę miasta
    City *cityB;              ///< wskaźnik na strukturę miasta
    unsigned length;          ///< długość odcinka drogi
    int year;                 ///< rok budowy lub ostatniego remontu odcinka drogi
    bool toRemove;            ///< informacja o tym, czy droga jest do usunięcia
    ListOfRoutes *firstRoute; ///< wskaźnik na pierwszą drogę krajową
    ListOfRoutes *lastRoute;  ///< wskaźnik na ostatnią drogę krajową
};

/**
 * @brief Struktura przechowująca miasto.
 * Struktura miasta zawiera:
 * nazwę miasta;
 * wskaźniki na pierwszy i ostatni element listy odcinków dróg;
 * wskaźnik na powiązany z danym miastem wierzchołek,
 * wykorzystywany przy obliczaniu najkrótszej ścieżki w grafie.
 */
struct City {
    const char *name;       ///< nazwa miasta
    ListOfRoads *firstRoad; ///< wskaźnik na pierwszą drogę
    ListOfRoads *lastRoad;  ///< wskaźnik na ostatnią drogę
    Vertex *vertex;         ///< wskaźnik na powiązany z miastem wierzchołek
};

/**
 * @brief Struktura przechowująca drogę krajową.
 * Struktura drogi krajowej zawiera:
 * identyfikator; 
 * wskaźniki na pierwszy i ostatni element listy odcinków dróg;
 * wskaźniki na pierwsze i ostatnie miasto należące do danej drogi krajowej.
 */
struct Route {
    unsigned id;            ///< identyfikator drogi krajowej
    ListOfRoads *firstRoad; ///< wskaźnik na pierwszą drogę
    ListOfRoads *lastRoad;  ///< wskaźnik na ostatnią drogę
    City *firstCity;        ///< wskaźnik na pierwsze miasto
    City *lastCity;         ///< wskaźnik na ostatnie miasto
};

#endif /* __HELP_STRUCTURES_H__ */