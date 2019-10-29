/** @file
 * Plik zawierający funkcje pomocnicze.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include "help-functions.h"
#include "dijkstra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MIN_ROUTE_ID 1   ///< najmniejszy numer drogi krajowej
#define MAX_ROUTE_ID 999 ///< największy numer drogi krajowej

bool isCityNameCorrect(const char *name) {
    int i = 0;
    bool correct = true;

    while ((name[i] != '\0') && correct) {
        if (((name[i] >= 0) && (name[i] <= 31)) || (name[i] == ';')) {
            correct = false;
        }
        i++;
    }

    if (i == 0) {
        correct = false;
    }

    return correct;
}

City* addCity(Map *map, const char *name) {
    City *city = NULL;
    Vertex *vertex = NULL;
    bool correct = true;

    if (map->numberOfCities == map->sizeOfCitiesArray) {
        if (!enlargeCitiesArray(map)) {
            correct = false;
        }
    }

    if (correct) {
        city = (City *)malloc(sizeof(City));
        if (city != NULL) {
            setCityValues(city, name);
            if (city->name == NULL) {
                free(city);
                city = NULL;
            }
            else {
                vertex = (Vertex *)malloc(sizeof(Vertex));
                if (vertex == NULL) {
                    freeCity(city);
                    city = NULL;
                }
                else {
                    setVertexValues(vertex, map->numberOfCities);
                    city->vertex = vertex;
                    map->cities[map->numberOfCities] = city;
                    map->numberOfCities += 1;
                }
            }
        }
    }

    return city;
}

Road* newRoad(Map *map, City *cityA, City *cityB, unsigned length, int year) {
    bool error = false;
    Road *road = (Road *)malloc(sizeof(Road));

    if (road != NULL) {
        setRoadValues(road, cityA, cityB, length, year);
        if (!addRoadToMap(map, road) || !addRoadToCity(cityA, road)
            || !addRoadToCity(cityB, road)) {
            error = true;
        }
    }

    if (error) {
        free(road);
        road = NULL;
    }

    return road;
}

bool isLengthCorrect(unsigned length) {
    if (length > 0) {
        return true;
    }
    else {
        return false;
    }
}

bool isYearCorrect(int year) {
    if (year == 0) {
        return false;
    }
    else {
        return true;
    }
}

bool isRouteIdCorrect(unsigned routeId) {
    if ((routeId >= MIN_ROUTE_ID) && (routeId <= MAX_ROUTE_ID)) {
        return true;
    }
    else {
        return false;
    }
}

Route* addRoute(Map *map, unsigned id, int start, int stop, int previous[]) {
    Route *route = NULL;

    route = (Route *)malloc(sizeof(Route));
    if (route != NULL) {
        setRouteValues(route, id, map->cities[start], map->cities[stop]);
        while ((previous[start] != -1) && (route != NULL)) {
            if (!addRoadToRoute(route, map->cities[start]->vertex->road)
                || !addRouteToRoad(map->cities[start]->vertex->road, route)) {
                route = NULL;
            }
            start = previous[start];
        }
    }

    return route;
}

bool extendFromFirstCity(Map *map, Route *route, int start, int previous[]) {
    ListOfRoads *first, *last;
    City *firstCity;
    bool correct = true;

    firstCity = map->cities[start];
    start = previous[start];
    first = route->firstRoad;
    last = route->lastRoad;

    while ((start != -1) && correct) {
        if (!addRoadToRoute(route, map->cities[start]->vertex->road)
            || !addRouteToRoad(map->cities[start]->vertex->road, route)) {
            correct = false;
        }
        start = previous[start];
    }

    if (correct) {
        route->firstCity = firstCity;
        route->firstRoad = last->next;
        last->next = NULL;
        route->lastRoad->next = first;
        route->lastRoad = last;
    }

    return correct;
}

bool extendFromLastCity(Map *map, Route *route, int start, int previous[]) {
    bool correct = true;

    while ((previous[start] != -1) && correct) {
        if (!addRoadToRoute(route, map->cities[start]->vertex->road)
            || !addRouteToRoad(map->cities[start]->vertex->road, route)) {
            correct = false;
        }
        start = previous[start];
    }

    if (correct) {
        route->lastCity = map->cities[start];
    }

    return correct;
}

bool canRoadBeRemoved(Map *map, Road *road) {
    bool error = false;
    int previous[map->numberOfCities];
    ListOfRoutes *node = road->firstRoute;

    while ((node != NULL) && !error) {
        markVerticies(node->route, true);
        dijkstra(map, road->cityA->vertex->id, road->cityB->vertex->id, previous);
        markVerticies(node->route, false);
        if (previous[road->cityB->vertex->id] == -1) {
            error = true;
        }
        node = node->next;
    }

    if (error) {
        return false;
    }
    else {
        return true;
    }
}

void removeListOfRoadsNodeFromCity(City *city, Road *road) {
    ListOfRoads *node = city->firstRoad;
    ListOfRoads *nodeToRemove = NULL;
    bool found = false;

    if (node != NULL) {
        if (city->firstRoad == city->lastRoad) {
            nodeToRemove = city->firstRoad;
            city->firstRoad = NULL;
            city->lastRoad = NULL;
        }
        else if (city->firstRoad->road == road) {
            nodeToRemove = city->firstRoad;
            city->firstRoad = nodeToRemove->next;
        }
        else {
            while ((node != NULL) && (node->next != NULL) && !found) {
                if (node->next->road == road) {
                    nodeToRemove = node->next;
                    node->next = nodeToRemove->next;
                    if (nodeToRemove == city->lastRoad) {
                        city->lastRoad = node;
                        city->lastRoad->next = NULL;
                    }
                    found = true;
                }
                node = node->next;
            }
        }
        free(nodeToRemove);
    }
}

void removeRoadFromMap(Map *map, Road *road) {
    ListOfRoads *node = map->firstRoad;
    ListOfRoads *nodeToRemove = NULL;
    bool found = false;

    if (node != NULL) {
        if (map->firstRoad == map->lastRoad) {
            nodeToRemove = map->firstRoad;
            map->firstRoad = NULL;
            map->lastRoad = NULL;
        }
        else if (map->firstRoad->road == road) {
            nodeToRemove = map->firstRoad;
            map->firstRoad = nodeToRemove->next;
        }
        else {
            while ((node != NULL) && (node->next != NULL) && !found) {
                if (node->next->road == road) {
                    nodeToRemove = node->next;
                    node->next = nodeToRemove->next;
                    if (nodeToRemove == map->lastRoad) {
                        map->lastRoad = node;
                        map->lastRoad->next = NULL;
                    }
                    found = true;
                }
                node = node->next;
            }
        }
        free(nodeToRemove);
        freeListOfRoutes(road->firstRoute);
        free(road);
    }
}

/**
 * @brief Znajduje poprawną kolejność miast do wywołania funkcji znajdującej
 * najkrótszą ścieżkę w grafie.
 * Szuka w drodze krajowej danego odcinka drogi, następnie sprawdza, od którego,
 * do którego miasta powinna zostać wywołana funkcja znajdująca najkrótszą
 * ścieżkę w grafie, aby można było utworzyć objazd.
 * @param[in] start          – wskaźnik na identyfikator miasta początkowego;
 * @param[in] stop           – wskaźnik na identyfikator miasta końcowego;
 * @param[in] route          – wskaźnik na strukturę drogi krajowej;
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 * @return Wskaźnik na węzeł poprzedzający węzeł, w którym znajduje się dany
 * odcinek drogi.
 */
static ListOfRoads* findCorrectCitiesForDijkstra(int *start, int *stop,
                                                 Route *route, Road *road) {
    ListOfRoads *node = route->firstRoad;
    ListOfRoads *previousNode = NULL;
    City *cityA, *cityB;

    cityA = road->cityA;
    cityB = road->cityB;

    if (route->firstRoad->road == road) {
        if (route->firstCity == cityA) {
            *start = cityA->vertex->id;
            *stop = cityB->vertex->id;
        }
        else {
            *start = cityB->vertex->id;
            *stop = cityA->vertex->id;
        }
    }
    else {
        while ((node != NULL) && (node->road != road)) {
            previousNode = node;
            node = node->next;
        }

        if (node == route->lastRoad) {
            if (route->lastCity == cityA) {
                *start = cityB->vertex->id;
                *stop = cityA->vertex->id;
            }
            else {
                *start = cityA->vertex->id;
                *stop = cityB->vertex->id;
            }
        }
        else {
            if (previousNode->road->cityA == cityA
                || previousNode->road->cityB == cityA) {
                *start = cityA->vertex->id;
                *stop = cityB->vertex->id;
            }
            else {
                *start = cityB->vertex->id;
                *stop = cityA->vertex->id;
            }
        }
    }

    return previousNode;
}

/**
 * @brief Tworzy nowe węzły listy odcinków dróg w drodze krajowej.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in,out] route      – wskaźnik na strukturę wydłużanej drogi krajowej;
 * @param[in] start          – identyfikator miasta, od którego wydłużamy drogę;
 * @param[in] previous       – tablica identyfikatorów poprzedników miast.
 * @return Wartość @p true, jeśli udało się utworzyć nowe węzły listy.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool fillListOfRoads(Map *map, Route *route, int start, int previous[]) {
    bool correct = true;

    while ((previous[start] != -1) && correct) {
        if (!addRoadToRoute(route, map->cities[start]->vertex->road)
            || !addRouteToRoad(map->cities[start]->vertex->road, route)) {
            correct = false;
        }
        start = previous[start];
    }

    return correct;
}

/**
 * @brief Tworzy objazd w drodze krajowej.
 * Funkcja sprawdza różne warianty umiejscowienia odcinka drogi do usunięcia
 * i w zależności od tego tworzy objazd w drodze krajowej. Na koniec funkcja
 * usuwa węzeł listy odcinków dróg zawierający odcinek do usunięcia.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in,out] route      – wskaźnik na strukturę wydłużanej drogi krajowej;
 * @param[in] start          – identyfikator miasta, od którego wydłużamy drogę;
 * @param[in] previous       – tablica identyfikatorów poprzedników miast;
 * @param[in] node           – wskaźnik na węzeł listy odcinków dróg.
 * @return Wartość @p true, jeśli udało się utworzyć objazd.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool fillTheRoute(Map *map, Route *route, int start, int previous[],
                         ListOfRoads *node) {
    ListOfRoads *nodeToRemove, *helpNode;
    ListOfRoads *last = route->lastRoad;
    bool correct = true;

    if (node == NULL) {
        nodeToRemove = route->firstRoad;
        if (route->lastRoad == nodeToRemove) {
            route->firstRoad = NULL;
            route->lastRoad = NULL;
            if (!fillListOfRoads(map, route, start, previous)) {
                correct = false;
            }
        }
        else {
            helpNode = nodeToRemove->next;
            route->firstRoad = NULL;
            route->lastRoad = NULL;
            if (!fillListOfRoads(map, route, start, previous)) {
                correct = false;
            }
            else {
                route->lastRoad->next = helpNode;
                route->lastRoad = last;
            }
        }
    }
    else if (node->next == route->lastRoad) {
        nodeToRemove = route->lastRoad;
        node->next = NULL;
        route->lastRoad = node;
        if (!fillListOfRoads(map, route, start, previous)) {
            correct = false;
        }
    }
    else {
        nodeToRemove = node->next;
        helpNode = nodeToRemove->next;
        route->lastRoad = node;
        if (!fillListOfRoads(map, route, start, previous)) {
            correct = false;
        }
        else {
            route->lastRoad->next = helpNode;
            route->lastRoad = last;
        }
    }

    free(nodeToRemove);

    return correct;
}

bool removeRoadWithFillingRoutes(Map *map, Road *road) {
    ListOfRoads *node;
    ListOfRoutes *routes = road->firstRoute;
    bool correct = true;
    int previous[map->numberOfCities];
    int start = -1, stop = -1;

    while ((routes != NULL) && correct) {
        node = findCorrectCitiesForDijkstra(&start, &stop, routes->route, road);
        markVerticies(routes->route, true);
        dijkstra(map, stop, start, previous);
        fillTheRoute(map, routes->route, start, previous, node);
        markVerticies(routes->route, false);
        routes = routes->next;
    }

    return correct;
}

/**
 * @brief Usuwa drogę krajową z odcinka drogi.
 * @param[in,out] road       – wskaźnik na strukturę odcinka drogi;
 * @param[in] route          – wskaźnik na strukturę drogi krajowej.
 */
static void removeRouteFromRoad(Road *road, Route *route) {
    ListOfRoutes *node, *nodeToRemove;
    bool end = false;

    if (road->firstRoute->route == route) {
        nodeToRemove = road->firstRoute;
        if (road->lastRoute->route == route) {
            road->firstRoute = NULL;
            road->lastRoute = NULL;
        }
        else {
            road->firstRoute = nodeToRemove->next;
        }
    }
    else {
        node = road->firstRoute;
        while ((node != NULL) && (node->next != NULL) && !end) {
            if (node->next->route == route) {
                end = true;
            }
            else {
                node = node->next;
            }
        }

        nodeToRemove = node->next;
        node->next = nodeToRemove->next;

        if (nodeToRemove == road->lastRoute) {
            road->lastRoute = node;
        }
    }

    free(nodeToRemove);
}

void removeRouteFromRoads(Route *route) {
    ListOfRoads *node = route->firstRoad;
    ListOfRoads *helpNode = NULL;

    while (node != NULL) {
        removeRouteFromRoad(node->road, route);
        helpNode = node;
        node = node->next;
        free(helpNode);
    }
}

/**
 * @brief Zlicza liczbę znaków potrzebnych do zapisania liczby jako napisu.
 * @param[in] x              – liczba całkowita.
 * @return Liczba znaków potrzebnych do zapisania liczby jako napisu.
 */
static int lengthOfInteger(int x) {
    int counter = 0;

    if (x < 0) {
        counter++;
        x = -x;
    }

    while (x > 0) {
        x /= 10;
        counter++;
    }

    return counter;
}

/**
 * @brief Zlcza liczbę znaków potrzebnych do zapisania liczby typu unsigned
 * jako napisu.
 * @param[in] x              – nieujemna liczba całkowita.
 * @return Liczba znaków potrzebnych do zapisania liczby typu unsigned
 * jako napisu.
 */
static int lengthOfUnsigned(unsigned x) {
    int counter = 0;

    while (x > 0) {
        x /= 10;
        counter++;
    }

    return counter;
}

char const* routeDescription(Route *route) {
    char *description = NULL;
    size_t size, tempSize, cityNameLength;
    int lengthNumberLength, yearNumberLength;
    ListOfRoads *node = route->firstRoad;
    City *city = route->firstCity;

    size = lengthOfUnsigned(route->id) + 1;
    cityNameLength = strlen(route->firstCity->name) + 1;
    description = (char *)malloc(size + cityNameLength);
    if (description != NULL) {
        sprintf(description, "%u;", route->id);
        sprintf(description + size, "%s", city->name);
        size += cityNameLength;
    }

    while ((node != NULL) && (description != NULL)) {
        if (node->road->cityA == city) {
            city = node->road->cityB;
        }
        else {
            city = node->road->cityA;
        }
        
        lengthNumberLength = lengthOfUnsigned(node->road->length) + 1;
        yearNumberLength = lengthOfInteger(node->road->year) + 1;
        cityNameLength = strlen(city->name) + 1;
        tempSize = lengthNumberLength + yearNumberLength + cityNameLength;
        description = (char *)realloc(description, size + tempSize);
        
        if (description != NULL) {
            sprintf(description + size - 1, ";%u;%d;%s", node->road->length,
                    node->road->year, city->name);
            size += tempSize;
            node = node->next;
        }
    }

    return description;
}

bool isInteger(long x) {
    if ((x >= INT_MIN) && (x <= INT_MAX)) {
        return true;
    }
    else {
        return false;
    }
}

bool isUnsigned(unsigned long x) {
    if (x <= UINT_MAX) {
        return true;
    }
    else {
        return false;
    }
}
