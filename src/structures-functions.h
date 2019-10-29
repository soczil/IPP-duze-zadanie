/** @file
 * Interfejs pliku zawierającego podstawowe funkcje operujące na strukturach.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef __STRUCTURES_FUNCTIONS_H__
#define __STRUCTURES_FUNCTIONS_H__

#include "map.h"
#include "help-structures.h"

/**
 * @brief Ustawia podstawowe wartości mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy.
 */
void setMapValues(Map *map);

/**
 * @brief Zwalnia pamięć zaalokowaną na dane miasto.
 * @param[in] city           – wskaźnik na strukturę miasta.
 */
void freeCity(City *city);

/**
 * @brief Zwalnia pamięć zaalokowaną na daną listę dróg oraz drogi.
 * @param[in] list           – lista odcinków dróg.
 */
void freeRoads(ListOfRoads *list);

/**
 * @brief Zwalnia pamięć zaalokowaną na daną drogę krajową.
 * @param[in] route          – wskaźnik na strukturę drogi krajowej.
 */
void freeRoute(Route *route);

/**
 * @brief Zwalnia pamięć zaalokowaną na listę dróg krajowych oraz na
 * drogi krajowe.
 * @param[in] list           – wskaźnik na listę dróg krajowych.
 */
void freeRoutes(ListOfRoutes *list);

/**
 * @brief Zwalnia pamięć zaalokowaną na listę dróg krajowych.
 * @param[in] list           – wskaźnik na listę dróg krajowych.
 */
void freeListOfRoutes(ListOfRoutes *list);

/**
 * @brief Szuka w mapie miasta o podanej nazwie.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] name           – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wskaźnik na znalezione miasto lub NULL, gdy miasto o podanej nazwie
 * nie istnieje.
 */
City* findCity(Map *map, const char *name);

/**
 * @brief Zwiększa rozmiar tablicy wskaźników na miasta w mapie.
 * @param[in,out] map        – wskaźnik na strukturę mapy.
 * @return Wartość @p true, jeśli udało się powiększyć tablicę.
 * Wartość @p false, gdy nie udało się zaalokować pamięci.
 */
bool enlargeCitiesArray(Map *map);

/**
 * @brief Ustawia podstawowe wartości miasta.
 * @param[in,out] city       – wskaźnik na strukturę miasta;
 * @param[in] name           – wskaźnik na napis reprezentujący nazwę miasta.
 */
void setCityValues(City *city, const char *name);

/**
 * @brief Ustawia podstawowe wartości wierzchołka.
 * @param[in,out] vertex     – wskaźnik na strukturę wierzchołka;
 * @param[in] id             – identyfikator wierzchołka/miasta.
 */
void setVertexValues(Vertex *vertex, int id);

/**
 * @brief Ustawia podstawowe wartości odcinka drogi.
 * @param[in,out] road       – wskaźnik na strukturę odcinka drogi;
 * @param[in] cityA          – wskaźnik na strukturę miasta;
 * @param[in] cityB          – wskaźnik na strukturę miasta;
 * @param[in] length         – długość drogi;
 * @param[in] builtYear      – rok budowy drogi.
 */
void setRoadValues(Road *road, City *cityA, City *cityB,
                   unsigned length, int builtYear);

/**
 * @brief Dodaje odcinek drogi do mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] road           – wskaźnik na dodawany odcinek drogi.
 * @return Wartość @p true, jeśli udało się dodać odcinek drogi do mapy.
 * Wartość @p false, gdy nie udało się zaalokować pamięci.
 */
bool addRoadToMap(Map *map, Road *road);

/**
 * @brief Dodaje odcinek drogi do miasta.
 * @param[in,out] city       – wskaźnik na strukturę miasta;
 * @param[in] road           – wskaźnik na dodawany odcinek drogi.
 * @return Wartość @p true, jeśli udało się dodać odcinek drogi do miasta.
 * Wartość @p false, gdy nie udało się zaalokować pamięci.
 */
bool addRoadToCity(City *city, Road *road);

/**
 * @brief Szuka drogi pomiędzy dwoma podanymi miastami.
 * @param[in] cityA          – wskaźnik na strukturę miasta;
 * @param[in] cityB          – wskaźnik na strukturę miasta.
 * @return Wskaźnik na znaleziony odcinek drogi lub NULL,
 * gdy odcinek drogi pomiędzy podanymi miastami nie istnieje.
 */
Road* findRoad(City *cityA, City *cityB);

/**
 * @brief Dodaje drogę krajową do mapy.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] route          – wskaźnik na strukturę drogi krajowej.
 * @return Wartość @p true, jeśli udało się dodać drogę krajową do mapy.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool addRouteToMap(Map *map, Route *route);

/**
 * @brief Powiększa tablicę wskaźników na drogi krajowe w mapie.
 * @param[in,out] map        – wskaźnik na strukturę mapy.
 * @return Wartość @p true, jeśli udało się powiększyć tablicę.
 * Wartość @p false, gdy nie udało się zaalokować pamięci.
 */
bool enlargeRoutesArray(Map *map);

/**
 * @brief Ustawia podstawowe wartości drogi krajowej.
 * @param[in,out] route      – wskaźnik na strukturę drogi krajowej;
 * @param[in] id             – identyfikator drogi krajowej;
 * @param[in] firstCity      – wskaźnik na strukturę miasta;
 * @param[in] lastCity       – wskaźnik na strukturę miasta.
 */
void setRouteValues(Route *route, unsigned id, City *firstCity, City *lastCity);

/**
 * @brief Dodaje odcinek drogi do drogi krajowej.
 * @param[in,out] route      – wskaźnik na strukturę drogi krajowej;
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 * @return Wartość @p true, jeśli udało się dodać odcinek do drogi krajowej.
 * Wartość @p false, gdy nie udało się zaalokować pamięci.
 */
bool addRoadToRoute(Route *route, Road *road);

/**
 * @brief Dodaje drogę krajową do odcinka drogi.
 * @param[in,out] road       – wskaźnik na strukturę odcinka drogi;
 * @param[in] route          – wskaźnik na strukturę drogi krajowej.
 * @return Wartość @p true, jeśli udało się dodać drogę krajową do odcinka.
 * Wartość @p false, gdy nie udało się zaalokować pamięci.
 */
bool addRouteToRoad(Road *road, Route *route);

/**
 * @brief Szuka w mapie drogi krajowej o podanym numerze.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] id             – identyfikator drogi krajowej.
 * @return Wskaźnik na znalezioną drogę krajową lub NULL, gdy droga krajowa
 * o podanym identyfikatorze nie istnieje.
 */
Route* findRoute(Map *map, unsigned id);

/**
 * @brief Sprawdza, czy podane miasto znajduje się w danej drodze krajowej.
 * @param[in] route          – wskaźnik na strukturę drogi krajowej;
 * @param[in] city           – wskaźnik na strukturę miasta.
 * @return Wartość @p true, jeśli dana droga przechodzi przez dane miasto.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isCityInRoute(Route *route, City *city);

/**
 * @brief Przypisuje wszystkim wierzchołkom w danej drodze podaną wartość.
 * @param[in,out] route      – wskaźnik na strukturę drogi krajowej;
 * @param[in] value          – przypisywana wartość.
 */
void markVerticies(Route *route, bool value);

#endif /* __STRUCTURES_FUNCTIONS_H__ */