/** @file 
 * Interfejs pliku zawierającego funkcje pomocnicze.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef __HELP_FUNCTIONS_H__
#define __HELP_FUNCTIONS_H__

#include "structures-functions.h"

/**
 * @brief Sprawdza, czy podana nazwa jest poprawną nazwą miasta.
 * @param[in] name           – napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli podana nazwa jest poprawna.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isCityNameCorrect(const char *name);

/**
 * @brief Dodaje miasto o podanej nazwie do mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] name           – napis reprezentujący nazwę miasta.
 * @return Wskaźnik na nowoutworzone miasto lub wartość NULL, jeśli
 * nie udało się zaalokować pamięci.
 */
City* addCity(Map *map, const char *name);

/**
 * @brief Dodaje drogę podanych wartościach do mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] cityA          – wskaźnik na strukturę miasta;
 * @param[in] cityB          – wskaźnik na strukturę miasta;
 * @param[in] length         – długość drogi;
 * @param[in] year           – rok budowy drogi.
 * @return Wskaźnik na nowoutworzoną drogę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Road* newRoad(Map *map, City *cityA, City *cityB, unsigned length, int year);

/**
 * @brief Sprawdza, czy podana długość drogi jest poprawna.
 * @param[in] length         – długość drogi.
 * @return Wartość @p true, jeśli podana długość jest poprawna.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isLengthCorrect(unsigned length);

/**
 * @brief Sprawdza, czy podany rok budowy lub ostatniego remontu drogi
 * jest poprawny.
 * @param[in] year           – rok budowy lub ostatniego remontu drogi.
 * @return Wartość @p true, jeśli rok jest poprawny.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isYearCorrect(int year);

/**
 * @brief Sprawdza, czy podany identyfikator drogi krajowej jest poprawny.
 * @param[in] routeId        – identyfikator drogi krajowej.
 * @return Wartość @p true, jeśli identyfikator jest poprawny.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isRouteIdCorrect(unsigned routeId);

/**
 * @brief Dodaje nową drogę krajową o podanych wartościach do mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] id             – identyfikator drogi krajowej;
 * @param[in] start          – identyfikator miasta początkowego;
 * @param[in] stop           – identyfikator miasta końcowego;
 * @param[in] previous       – tablica identyfikatorów poprzedników miast.
 * @return Wskaźnik na nowoutworzoną drogę krajową lub NULL, gdy nie udało się
 * zaalokować pamięci. 
 */
Route* addRoute(Map *map, unsigned id, int start, int stop, int previous[]);

/**
 * @brief Wydłuża drogę krajową od pierwszego miasta.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in,out] route      – wskaźnik na strukturę wydłużanej drogi krajowej;
 * @param[in] start          – identyfikator nowego miasta początkowego;
 * @param[in] previous       – tablica identyfikatorów poprzedników miast.
 * @return Wartość @p true, jeśli udało się wydłużyć drogę.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool extendFromFirstCity(Map *map, Route *route, int start, int previous[]);

/**
 * @brief Wydłuża drogę krajową od ostatniego miasta.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in,out] route      – wskaźnik na strukturę wydłużanej drogi krajowej;
 * @param[in] start          – identyfikator miasta, od którego wydłużamy drogę;
 * @param[in] previous       – tablica identyfikatorów poprzedników miast.
 * @return Wartość @p true, jeśli udało się wydłużyć drogę.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool extendFromLastCity(Map *map, Route *route, int start, int previous[]);

/**
 * @brief Sprawdza, czy odcinek drogi może zostać usunięty.
 * Sprawdza, czy da się utworzyć objazd dla każdej drogi krajowej, której
 * elementem jest podany odcinek drogi.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 * @return Wartość @p true, jeśli można usunąć podany odcinek drogi.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool canRoadBeRemoved(Map *map, Road *road);

/**
 * @brief Usuwa dany odcinek drogi z miasta.
 * Szuka w mieście węzła listy odcinków dróg zawierającego dany odcinek
 * drogi, a następnie usuwa ten węzeł z listy.
 * @param[in,out] city       – wskaźnik na strukturę miasta;
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 */
void removeListOfRoadsNodeFromCity(City *city, Road *road);

/**
 * @brief Usuwa dany odcinek drogi z mapy.
 * Szuka w mapie węzła listy odcinków dróg zawierającego dany odcinek drogi,
 * a następnie usuwa ten węzeł z listy oraz dany odcinek drogi z mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 */
void removeRoadFromMap(Map *map, Road *road);

/**
 * @brief Usuwa dany odcinek drogi z list tworząc objazd w drodze krajowej.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 * @return Wartość @p true, jeśli udało się utworzyć objazd.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool removeRoadWithFillingRoutes(Map *map, Road *road);

/**
 * @brief Usuwa drogę krajową z odcinków dróg, jednocześnie usuwając węzły
 * listy odcinków dróg z drogi krajowej.
 * @param[in,out] route      – wskaźnik na strukturę drogi krajowej.
 */
void removeRouteFromRoads(Route *route);

/**
 * @brief Tworzy opis drogi krajowej.
 * Alokuje pamięć potrzebną do utworzenia opisu oraz skleja parametry w napis.
 * @param[in] route          – wskaźnik na strukturę drogi krajowej.
 * @return Napis reprezentujący opis drogi krajowej lub NULL, gdy nie udało
 * się zaalokować pamięci.
 */
char const* routeDescription(Route *route);

/**
 * @brief Sprawdza, czy podana liczba mieści się w zakresie typu int.
 * @param[in] x              – liczba typu long.
 * @return Wartość @p true, jeśli podana liczba mieści się w zakresie typu int.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isInteger(long x);

/**
 * @brief Sprawdza, czy podana liczba mieści się w zakresie typu unsigned.
 * @param[in] x              – liczba typu unsigned long.
 * @return Wartość @p true, jeśli podana liczba mieści się w zakresie typu
 * unsigned.
 * Wartość @p false, w przeciwnym przypadku.
 */
bool isUnsigned(unsigned long x);

#endif /* __HELP_FUNCTIONS_H__ */