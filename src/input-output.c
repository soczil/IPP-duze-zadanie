/** @file
 * Plik zawierający funkcje do obsługi interfejsu tekstowego.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#define _GNU_SOURCE

#include "input-output.h"
#include "help-functions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Drukuje informację o błędzie standardowe wyjście diagnostyczne.
 * @param[in] lineNumber     – numer linii, w której wystąpił błąd.
 */
static void printError(int lineNumber) {
    fprintf(stderr, "ERROR %d\n", lineNumber);
}

/**
 * @brief Daje w wyniku wskaźnik na następny parametr.
 * Przechodzi po napisie do czasu napotkania znaku ';', następnie wstawia
 * w jego miejsce znak końca napisu i przesuwa wskaźnik @p parameters na
 * pierwszy element po wstawionym znaku końca napisu.
 * Nic nie robi, jeśli wskaźnik podany jako parametr ma wartość NULL.
 * @return Wskaźnik na pierwszy element parametru lub NULL, gdy w podanym
 * jako parametr napisie nie ma znaku ';', lub jeśli znak ';' jest ostatnim
 * znakiem w napisie przed znakiem nowej linii.
 */
static char* getParameter(char **parameters) {
    char *param = *parameters;
    int i = 0;
    bool end = false;

    if (*parameters != NULL) {
        while ((param[i] != '\0') && !end) {
            if (((param[i] == ';') && (param[i + 1] != '\n'))
                || (param[i] == '\n')) {
                *parameters += i + 1;
                param[i] = '\0';
                end = true;
            }
            i++;
        }

        if (!end) {
            param = NULL;
        }
    }

    return param;
}

/**
 * @brief Sprawdza czy podany jako parametr napis może być liczbą.
 * Przechodzi (aż do napotkania znaku końca napisu) po każdym znaku podanego
 * jako parametr napisu i sprawdza czy dany znak jest znakiem reprezentującym
 * cyfrę. Jeśli liczba nie musi być nieujemna, funckja pomija pierwszy znak.
 * @param[in] string         – wskaźnik na pierwszy element sprawdzanego napisu;
 * @param[in] isUnsigned     – zmienna zawierająca informację o tym czy dana 
 *                             liczba ma być nieujemna.
 * @return Wartość @p true, jeśli podany napis może być liczbą.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool isNumber(char *string, bool isUnsigned) {
    int i = 0;
    bool number = true;

    if ((string[0] == '-') && !isUnsigned) {
        i++;
    }

    while ((string[i] != '\0') && number) {
        if ((string[i] < '0') || (string[i] > '9')) {
            number = false;
        }
        i++;
    }

    if (i == 0) {
        number = false;
    }

    return number;
}

/**
 * @brief Funkcja odpowiedzialna za obsługę polecenia addRoad.
 * Funkcja wyodrębnia poszczególne parametry z podanego napisu, następnie,
 * jeśli udało się sparsować długość oraz rok budowy drogi na liczbę, wywołuje
 * funkcję addRoad z interfejsu mapy. Wywołuje funkcję drukującą informację
 * o błędzie, jeśli podczas działania funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebne parametry;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję.
 */
static void addRoadCommand(Map *map, char *parameters, int lineNumber) {
    char *city1, *city2, *length, *builtYear;
    unsigned long lengthNumber;
    long yearNumber;
    bool error = false;

    city1 = getParameter(&parameters);
    city2 = getParameter(&parameters);
    length = getParameter(&parameters);
    builtYear = getParameter(&parameters);

    if ((city1 == NULL) || (city2 == NULL) || (length == NULL)
        || (builtYear == NULL) || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!isNumber(length, true) || !isNumber(builtYear, false)) {
            error = true;
        }
        else {
            lengthNumber = strtoul(length, NULL, 10);
            yearNumber = strtol(builtYear, NULL, 10);
            if (!isUnsigned(lengthNumber) || !isInteger(yearNumber)) {
                error = true;
            }
            else {
                if (!addRoad(map, city1, city2, lengthNumber, yearNumber)) {
                    error = true;
                }
            }
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * @brief Funkcja odpowiedzialna za obsługę polecenia repairRoad.
 * Funkcja wyodrębnia poszczególne parametry z podanego napisu, następnie,
 * jeśli udało się sparsować rok budowy drogi na liczbę, wywołuje funkcję
 * repairRoad z interfejsu mapy. Wywołuje funkcję drukującą informację
 * o błędzie, jeśli podczas działania funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebne parametry;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void repairRoadCommand(Map *map, char *parameters, int lineNumber) {
    char *city1, *city2, *repairYear;
    long yearNumber;
    bool error = false;

    city1 = getParameter(&parameters);
    city2 = getParameter(&parameters);
    repairYear = getParameter(&parameters);

    if ((city1 == NULL) || (city2 == NULL) || (repairYear == NULL)
        || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!isNumber(repairYear, false)) {
            error = true;
        }
        else {
            yearNumber = strtol(repairYear, NULL, 10);
            if (!isInteger(yearNumber)) {
                error = true;
            }
            else {
                if (!repairRoad(map, city1, city2, yearNumber)) {
                    error = true;
                }
            }
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * Funkcja odpowiedzialna za obsługę polecenia getRouteDescription.
 * Funkcja wyodrębnia potrzebny parametr z podanego napisu, następnie,
 * jeśli udało się sparsować identyfikator drogi krajowej na liczbę,
 * wywołuje funkcję getRouteDescription z interfejsu mapy.
 * Wywołuje funkcję drukującą informację o błędzie, jeśli podczas 
 * działania funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebny parametr;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void getRouteDescriptionCommand(Map *map, char *parameters,
                                       int lineNumber) {
    char *routeId;
    const char *description;
    unsigned long routeIdNumber;
    bool error = false;

    routeId = getParameter(&parameters);

    if ((routeId == NULL) || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!isNumber(routeId, true)) {
            error = true;
        }
        else {
            routeIdNumber = strtoul(routeId, NULL, 10);
            if (!isUnsigned(routeIdNumber)) {
                error = true;
            }
            else {
                description = getRouteDescription(map, routeIdNumber);
                if (description == NULL) {
                    error = true;
                }
                else {
                    printf("%s\n", description);
                }
                free((void *)description);
            }
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * Funkcja odpowiedzialna za obsługę polecenia newRoute.
 * Funkcja wyodrębnia potrzebne parametry z podanego napisu, następnie,
 * jeśli parametry są poprawne, wywołuje funkcję newRoute z interfejsu mapy.
 * Wywołuje funkcję drukującą informację o błędzie, jeśli podczas działania
 * funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebne parametry;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void newRouteCommand(Map *map, char *parameters, int lineNumber) {
    char *routeId, *city1, *city2;
    unsigned long routeIdNumber;
    bool error = false;

    routeId = getParameter(&parameters);
    city1 = getParameter(&parameters);
    city2 = getParameter(&parameters);

    if ((routeId == NULL) || (city1 == NULL) || (city2 == NULL)
        || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!isNumber(routeId, true)) {
            error = true;
        }
        else {
            routeIdNumber = strtoul(routeId, NULL, 10);
            if (!isUnsigned(routeIdNumber)) {
                error = true;
            }
            else {
                if (!newRoute(map, routeIdNumber, city1, city2)) {
                    error = true;
                }
            }
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * Funkcja odpowiedzialna za obsługę polecenia extendRoute.
 * Funkcja wyodrębnia potrzebne parametry z podanego napisu, następnie,
 * jeśli parametry są poprawne, wywołuje funkcję extendRoute z interfejsu mapy.
 * Wywołuje funkcję drukującą informację o błędzie, jeśli podczas działania
 * funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebne parametry;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void extendRouteCommand(Map *map, char *parameters, int lineNumber) {
    char *routeId, *city;
    unsigned long routeIdNumber;
    bool error = false;

    routeId = getParameter(&parameters);
    city = getParameter(&parameters);

    if ((routeId == NULL) || (city == NULL) || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!isNumber(routeId, true)) {
            error = true;
        }
        else {
            routeIdNumber = strtoul(routeId, NULL, 10);
            if (!isUnsigned(routeIdNumber)) {
                error = true;
            }
            else {
                if (!extendRoute(map, routeIdNumber, city)) {
                    error = true;
                }
            }
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * Funkcja odpowiedzialna za obsługę polecenia removeRoad.
 * Funkcja wyodrębnia potrzebne parametry z podanego napisu, następnie,
 * jeśli parametry są poprawne, wywołuje funkcję removeRoad z interfejsu mapy.
 * Wywołuje funkcję drukującą informację o błędzie, jeśli podczas działania
 * funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebne parametry;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void removeRoadCommand(Map *map, char *parameters, int lineNumber) {
    char *city1, *city2;
    bool error = false;

    city1 = getParameter(&parameters);
    city2 = getParameter(&parameters);

    if ((city1 == NULL) || (city2 == NULL) || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!removeRoad(map, city1, city2)) {
            error = true;
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * Funkcja odpowiedzialna za obsługę polecenia removeRoute.
 * Funkcja wyodrębnia potrzebny parametr z podanego napisu, następnie,
 * jeśli identyfikator drogi jest poprawną liczbą, wywołuje funkcję
 * removeRoute z interfejsu mapy.
 * Wywołuje funkcję drukującą informację o błędzie, jeśli podczas 
 * działania funkcji wystąpił błąd.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] parameters     – napis zawierający potrzebny parametr;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void removeRouteCommand(Map *map, char *parameters, int lineNumber) {
    char *routeId;
    unsigned long routeIdNumber;
    bool error = false;

    routeId = getParameter(&parameters);

    if ((routeId == NULL) || (parameters[0] != '\0')) {
        error = true;
    }
    else {
        if (!isNumber(routeId, true)) {
            error = true;
        }
        else {
            routeIdNumber = strtoul(routeId, NULL, 10);
            if (!isUnsigned(routeIdNumber)) {
                error = true;
            }
            else {
                if (!removeRoute(map, routeIdNumber)) {
                    error = true;
                }
            }
        }
    }

    if (error) {
        printError(lineNumber);
    }
}

/**
 * @brief Oblicza liczbę miast w poleceniu tworzenia drogi krajowej.
 * Przechodzi przez podany jako parametr napis i zlicza wystąpienia znaku ';',
 * następnie sprawdza czy podana liczba jest podzielna przez 3. Jeśli tak,
 * ustawia zmienną @p result na podzieloną przez 3 i zwiększoną o 1 liczbę
 * średników w poleceniu, co odpowiada liczbie nazw miast w poprawnym poleceniu.
 * W przeciwnym wypadku ustawia wartość zmiennej @p result na 1.
 * @param[in] line           – wskaźnik na pierwszy element polecenia.
 * @return Zmienna @result reprezentującą liczbę miast lub, w przypadku
 * niepoprawnego polecenia, liczbę 1.
 */
static int calculateNumberOfCities(char *line) {
    int counter = 0, i = 0;
    int result;

    while (line[i] != '\0') {
        if (line[i] == ';') {
            counter++;
        }
        i++;
    }

    if (counter % 3 == 0) {
        result = counter / 3 + 1;
    }
    else {
        result = 1;
    }

    return result;
}

/**
 * @brief Wydziela z podanego napisu potrzebne dane.
 * Funkcja wyodrębnia z podanego jako parametr napisu potrzebne później dane,
 * takie jak: nazwy miast, długości dróg oraz lata budowy lub ostatniego
 * remontu dróg, a następnie zapisuje je do odpowiednich tablic.
 * @param[in] parameters     – napis zawierający parametry;
 * @param[in] numberOfCities – liczba miast w poleceniu budowy drogi krajowej;
 * @param[in,out] lengths    – tablica długości dróg;
 * @param[in,out] years      – tablica lat budowy lub ostatniego remontu dróg;
 * @param[in,out] names      – tablica nazw miast.
 * @return Wartość @p true, jeśli dane są poprawne.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool getData(char *parameters, int numberOfCities,
                    unsigned lengths[], int years[], char *names[]) {
    int i = 0;
    bool correct = true;
    char *length, *year;
    long yearNumber;
    unsigned long lengthNumber;

    while ((i < numberOfCities - 1) && correct) {
        names[i] = getParameter(&parameters);
        length = getParameter(&parameters);
        year = getParameter(&parameters);

        if ((names[i] == NULL) || (length == NULL) || (year == NULL)) {
            correct = false;
        }
        else {
            if (!isCityNameCorrect(names[i]) || !isNumber(length, true)
                || !isNumber(year, false)) {
                correct = false;
            }
            else {
                lengthNumber = strtoul(length, NULL, 10);
                yearNumber = strtol(year, NULL, 10);
                if (!isUnsigned(lengthNumber) || !isInteger(yearNumber)) {
                    correct = false;
                }
                else {
                    if ((lengthNumber == 0) || (yearNumber == 0)) {
                        correct = false;
                    }
                    else {
                        lengths[i] = (unsigned)lengthNumber;
                        years[i] = (int)yearNumber;
                    }
                }
            }
        }
        i++;
    }

    if (correct) {
        names[i] = getParameter(&parameters);
        if ((names[i] == NULL) || !isCityNameCorrect(names[i])) {
            correct = false;
        }
    }

    return correct;
}

/**
 * @brief Sprawdza, czy droga krajowa może zostać utworzona.
 * Szuka miast o podanych nazwach i jeśli łączy je już odcinek drogi, sprawdza,
 * czy podana długość oraz rok budowy nie przeszkadzają w utworzeniu drogi
 * krajowej wraz ze znalezionym odcinkiem drogi.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] numberOfCities – liczba miast w drodze krajowej;
 * @param[in] names          – tablica nazw miast;
 * @param[in] lengths        – tablica długości odcinków dróg;
 * @param[in] years          – tablica lat budowy lub ostatniego remontu dróg.
 * @return Wartość @p true, jeśli można utworzyć drogę krajową.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool canRouteBeMade(Map *map, int numberOfCities, char *names[],
                           unsigned lengths[], int years[]) {
    City *cityA, *cityB;
    Road *road;
    int i = 0;
    bool correct = true;

    cityA = findCity(map, names[i]);
    while ((i < numberOfCities - 1) && correct) {
        cityB = findCity(map, names[i + 1]);
        if ((cityA != NULL) && (cityB != NULL)) {
            road = findRoad(cityA, cityB);
            if (road != NULL) {
                if ((road->length != lengths[i]) || (road->year > years[i])) {
                    correct = false;
                }
            }
        }
        cityA = cityB;
        i++;
    }

    return correct;
}

/**
 * @brief Szuka miasta o podanej nazwie w mapie lub dodaje miasto, jeśli
 * miasto podanej nazwie nie istnieje.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] name           – nazwa miasta;
 * @param[in,out] cities     – tablica wskaźników na miasta;
 * @param[in] index          – indeks szukanego miasta w tablicy @p cities.
 * @return Wartość @p true, jeśli znaleziono miasto lub udało się utworzyć
 * miasto o podanej nazwie.
 * Wartość @p false, jeśli nie udało się dodać miasta.
 */
static bool findOrAddCity(Map *map, char *name, City **cities, int index) {
    cities[index] = findCity(map, name);
    
    if (cities[index] == NULL) {
        cities[index] = addCity(map, name);
    }

    if (cities[index] != NULL) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Tworzy tablicę wskaźników na miasta.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] numberOfCities – liczba miast w drodze krajowej;
 * @param[in] names          – tablica nazw miast.
 * @return Wskaźnik na tablicę wskaźników na miasta lub wartość NULL,
 * jeśli nie udało się zaalokować pamięci.
 */
static City** makeCitiesArray(Map *map, int numberOfCities, char *names[]) {
    bool error = false;
    int i = 1;
    City **cities = (City **)malloc(numberOfCities * sizeof(City *));

    if (cities != NULL) {
        if (!findOrAddCity(map, names[0], cities, 0)) {
            error = true;
        }
        else {
            while ((i < numberOfCities) && !error) {
                if (!findOrAddCity(map, names[i], cities, i)) {
                    error = true;
                }
                i++;
            }
        }
    }

    if (error) {
        free(cities);
        cities = NULL;
    }

    return cities;
}

/**
 * @brief Sprawdza, czy w tworzonej drodze nie ma pętli lub samoprzecięć.
 * Przechodzi po tablicy miast zaznaczająć odwiedzone miasta. W przypadku
 * napotkania odwiedzonego już wcześniej miasta, przerywa pętlę i zapisuje
 * informację o błędzie. Na koniec, zaznacza wszystkie miasta jako nieodwiedzone.
 * @param[in] cities         – tablica wskaźników na miasta;
 * @param[in] numberOfCities – liczba miast w tablicy.
 * @return Wartość @p true, jeśli w tworzonej drodze nie ma pętli.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool checkLoops(City **cities, int numberOfCities) {
    int i = 0;
    bool correct = true;
    
    while ((i < numberOfCities) && correct) {
        if (cities[i]->vertex->usedInRoute == true) {
            correct = false;
        }
        else {
            cities[i]->vertex->usedInRoute = true;
        }
        i++;
    }

    i = 0;
    while (i < numberOfCities) {
        cities[i]->vertex->usedInRoute = false;
        i++;
    }

    return correct;
}

/**
 * @brief Dodaje potrzebne do utworzenia drogi krajowej odcinki dróg.
 * Jeśli droga między miastami nie istnieje, to ją tworzy. Jeśli podany rok
 * budowy lub ostatniego remontu jest większy od roku drogi, aktualizuje
 * ten rok.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] cities         – tablica wskaźników na miasta;
 * @param[in] numberOfCities – liczba miast w drodze krajowej;
 * @param[in] lengths        – tablica długości odcinków dróg;
 * @param[in] years          – tablica lat budowy lub ostatniego remontu dróg.
 * @return Wartość @p true, jeśli udało się utworzyć potrzebne odcinki dróg.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool addRoads(Map *map, City **cities, int numberOfCities,
                     unsigned lengths[], int years[]) {
    int i = 0;
    bool correct = true;
    Road *road;

    while ((i < numberOfCities - 1) && correct) {
        road = findRoad(cities[i], cities[i + 1]);
        if (road == NULL) {
            road = newRoad(map, cities[i], cities[i + 1], lengths[i], years[i]);
            if (road == NULL) {
                correct = false;
            }
        }
        else {
            if (road->year < years[i]) {
                road->year = years[i];
            }
        }

        if (correct) {
            cities[i]->vertex->road = road;
        }
        i++;
    }

    return correct;
}

/**
 * @brief Tworzy drogę krajową o podanym przebiegu.
 * Tworzy tablicę identyfikatorów poprzedników miast, a następnie wywołuje
 * funkcję tworzącą drogę krajową. Na końcu zapisuje nowoutworzoną drogę do mapy.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] id             – identyfikator drogi krajowej;
 * @param[in] cities         – tablica wskaźników na miasta;
 * @param[in] numberOfCities – liczba miast w drodze krajowej.
 * @return Wartość @p true, jeśli udało utworzyć się drogę krajową.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool makeRoute(Map *map, unsigned id, City **cities, int numberOfCities) {
    int previous[map->numberOfCities];
    Route *route;
    int i = 0;
    bool correct = true;

    while ((i < numberOfCities - 1) && correct) {
        previous[cities[i]->vertex->id] = cities[i + 1]->vertex->id;
        i++;
    }
    previous[cities[i]->vertex->id] = -1;

    route = addRoute(map, id, cities[0]->vertex->id,
                     cities[numberOfCities - 1]->vertex->id, previous);
    
    if (route == NULL) {
        correct = false;
    }
    else {
        addRouteToMap(map, route);
    }

    return correct;
}

/**
 * @brief Funkcja odpowiedzialna za obsługę polecenia tworzenia nowej drogi.
 * Funkcja sprawdza różne przypadki, w których mógł wystąpić błąd, a następnie,
 * jeśli nie ma błędu, wywołuje funkcję tworzącą nową drogę krajową.
 * @param[in,out] map        – wskaźnik na strukturę mapy;
 * @param[in] id             – identyfikator drogi krajowej;
 * @param[in] parameters     – napis zawierający potrzebne parametry;
 * @param[in] lineNumber     – numer linii, w której wywołano funkcję. 
 */
static void makeRouteCommand(Map *map, unsigned id, char *parameters,
                             int lineNumber) {
    bool error = false;
    int numberOfCities = calculateNumberOfCities(parameters);
    char *names[numberOfCities];
    unsigned lengths[numberOfCities - 1];
    int years[numberOfCities - 1];
    City **cities = NULL;

    if ((numberOfCities == 1) || !isRouteIdCorrect(id)
        || (findRoute(map, id) != NULL)) {
        error = true;
    }
    else {
        if (!getData(parameters, numberOfCities, lengths, years, names)) {
            error = true;
        }
        else {
            if (!canRouteBeMade(map, numberOfCities, names, lengths, years)) {
                error = true;
            }
            else {
                cities = makeCitiesArray(map, numberOfCities, names);
                if (cities == NULL) {
                    error = true;
                }
                else {
                    if (!checkLoops(cities, numberOfCities)
                        || !addRoads(map, cities, numberOfCities, lengths, years)) {
                        error = true;
                    }
                    else {
                        if (!makeRoute(map, id, cities, numberOfCities)) {
                            error = true;
                        }
                    }
                }
            }
        }
    }

    free(cities);
    if (error) {
        printError(lineNumber);
    }
}

/**
 * @brief Wyodrębnia z napisu numer drogi krajowej.
 * Wywołuje funkcję dającą w wyniku kolejny parametr, następnie sprawdza
 * czy parametr ten jest liczbą i parsuje ten parametr na liczbę.
 * W przypadku, gdy podany parametr nie jest liczbą, lub parsowanie
 * nie powiodło się, ustawia wartość @p routeIdNumber na -1.
 * @param[in] route      – wskaźnik na napis zawierający identyfikator drogi
 *                         krajowej jako pierwszy element.
 * @return Wartość @p route reprezentującą numer drogi krajowej lub -1,
 * w przypadku niepoprawnych danych.
 */
static unsigned routeIdNumber(char *route) {
    char *routeId;
    unsigned long routeIdNumber;
    bool error = false;

    routeId = getParameter(&route);
    if ((routeId == NULL) || (routeId[0] == '\0')) {
        error = true;
    }
    else {
        if (!isNumber(routeId, true)) {
            error = true;
        }
        else {
            routeIdNumber = strtoul(routeId, NULL, 10);
            if (!isUnsigned(routeIdNumber)) {
                error = true;
            }
        }
    }

    if (error) {
        routeIdNumber = 0;
    }

    return routeIdNumber;
}

/**
 * @brief Wybiera poprawną funkcję obsługującą dane polecenie.
 * Sprawdza, które polecenie zostało wczytane ze standardowego wejścia,
 * następnie wywołuje funkcję do obsługi danego polecenia. Oprócz tego
 * na zmienną @p parameters zapisuje wskaźnik na pierwszy element po
 * pierwszym średniku, czyli po poleceniu.
 * @param[in,out] map    – wskaźnik na strukturę mapy;
 * @param[in] line       – wskaźnik na pierwszy znak wczytanej linii;
 * @param[in] lineNumber – numer wczytanej linii.
 */
static void chooseFunction(Map *map, char *line, int lineNumber) {
    char *parameters = strchr(line, ';');
    char *route;
    unsigned routeId;

    if (parameters != NULL) {
        parameters++;
        if (strstr(line, "addRoad;") == line) {
            addRoadCommand(map, parameters, lineNumber);
        }
        else if (strstr(line, "repairRoad;") == line) {
            repairRoadCommand(map, parameters, lineNumber);
        }
        else if (strstr(line, "getRouteDescription;") == line) {
            getRouteDescriptionCommand(map, parameters, lineNumber);
        }
        else if (strstr(line, "newRoute;") == line) {
            newRouteCommand(map, parameters, lineNumber);
        }
        else if (strstr(line, "extendRoute;") == line) {
            extendRouteCommand(map, parameters, lineNumber);
        }
        else if (strstr(line, "removeRoad;") == line) {
            removeRoadCommand(map, parameters, lineNumber);
        }
        else if (strstr(line, "removeRoute;") == line) {
            removeRouteCommand(map, parameters, lineNumber);
        }
        else {
            route = line;
            routeId = routeIdNumber(route);
            if (routeId != 0) {
                makeRouteCommand(map, routeId, parameters, lineNumber);
            }
            else {
                printError(lineNumber);
            }
        }
    }
    else {
        printError(lineNumber);
    }
}

void readAndDoCommands(void) {
    Map *map = NULL;
    char *line = NULL;
    int charactersInString = 0, lineNumber = 1;
    size_t length;

    map = newMap();
    if (map != NULL) {
        setMapValues(map);

        while ((charactersInString = getline(&line, &length, stdin)) != -1) {
            if ((line[0] != '#') && (line[0] != '\n')) {
                if (line[charactersInString - 1] == '\n') {
                    chooseFunction(map, line, lineNumber);
                }
                else {
                    printError(lineNumber);
                }
            }
            lineNumber++;
        }

        deleteMap(map);
        free(line);
    }
}
