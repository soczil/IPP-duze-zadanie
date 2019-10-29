/** @file
 * Plik zawierający podstawowe funkcje operujące na strukturach.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include "structures-functions.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MULTIPLIER 3 ///< mnożnik do powiększania tablic
#define DIVIDER 2    ///< dzielnik do powiększania tablic

void setMapValues(Map *map) {
    map->numberOfCities = 0;
    map->sizeOfCitiesArray = 0;
    map->cities = NULL;
    map->firstRoad = NULL;
    map->lastRoad = NULL;
    map->firstRoute = NULL;
    map->lastRoute = NULL;
}

void freeCity(City *city) {
    ListOfRoads *node;

    while (city->firstRoad != NULL) {
        node = city->firstRoad;
        city->firstRoad = city->firstRoad->next;
        free(node);
    }

    free((void *)city->name);
    free(city->vertex);
    free(city);
}

void freeRoads(ListOfRoads *list) {
    ListOfRoads *node;

    while (list != NULL) {
        node = list;
        list = list->next;
        freeListOfRoutes(node->road->firstRoute);
        free(node->road);
        free(node);
    }
}

void freeRoute(Route *route) {
    ListOfRoads *node;

    while (route->firstRoad != NULL) {
        node = route->firstRoad;
        route->firstRoad = route->firstRoad->next;
        free(node);
    }

    free(route);
}

void freeRoutes(ListOfRoutes *list) {
    ListOfRoutes *node;

    while (list != NULL) {
        node = list;
        list = list->next;
        freeRoute(node->route);
        free(node);
    }
}

void freeListOfRoutes(ListOfRoutes *list) {
    ListOfRoutes *node;

    while (list != NULL) {
        node = list;
        list = list->next;
        free(node);
    }
}

City* findCity(Map *map, const char *name) {
    City *city = NULL;
    int i = 0;
    bool found = false;

    while ((i < map->numberOfCities) && !found) {
        if (strcmp(map->cities[i]->name, name) == 0) {
            city = map->cities[i];
            found = true;
        }
        i++;
    }

    return city;
}

/**
 * @brief Powiększa podany rozmiar.
 * @param[in] size           – liczba całkowita.
 * @return Powiększony rozmiar.
 */
static int enlargeSize(int size) {
    return 1 + size * MULTIPLIER / DIVIDER;
}

bool enlargeCitiesArray(Map *map) {
    int newSize;
    bool correct = true;

    newSize = enlargeSize(map->sizeOfCitiesArray);
    map->cities = (City **)realloc(map->cities, newSize * sizeof(City *));

    if (map->cities == NULL) {
        correct = false;
    }
    else {
        for (int i = map->numberOfCities; i < newSize; i++) {
            map->cities[i] = NULL;
        }
        map->sizeOfCitiesArray = newSize;
    }

    return correct;
}

void setCityValues(City *city, const char *name) {
    char *cityName = (char *)malloc(strlen(name) + 1);

    if (cityName == NULL) {
        city->name = NULL;
    }
    else {
        strcpy(cityName, name);
        city->name = cityName;
        city->firstRoad = NULL;
        city->lastRoad = NULL;
        city->vertex = NULL;
    }
}

void setVertexValues(Vertex *vertex, int id) {
    vertex->id = id;
    vertex->distanceFromSource = INT_MAX;
    vertex->usedInRoute = false;
    vertex->road = NULL;
    vertex->oldestYear = 0;
    vertex->error = false;
}

void setRoadValues(Road *road, City *cityA, City *cityB,
                   unsigned length, int builtYear) {
    road->cityA = cityA;
    road->cityB = cityB;
    road->length = length;
    road->year = builtYear;
    road->toRemove = false;
    road->firstRoute = NULL;
    road->lastRoute = NULL;
}

/**
 * @brief Alokuje pamięć i ustawia wartości dla nowego węzła listy dróg.
 * @param[in] road           – wskaźnik na strukturę odcinka drogi.
 * @return Wskaźnik na węzeł lub NULL, gdy nie udało się zaalokować pamięci.
 */
static ListOfRoads* addListOfRoadsNode(Road *road) {
    ListOfRoads *node = NULL;

    node = (ListOfRoads *)malloc(sizeof(ListOfRoads));
    if (node != NULL) {
        node->road = road;
        node->next = NULL;
    }

    return node;
}

bool addRoadToMap(Map *map, Road *road) {
    ListOfRoads *node = addListOfRoadsNode(road);

    if (node != NULL) {
        if (map->firstRoad == NULL) {
            map->firstRoad = node;
            map->lastRoad = map->firstRoad;
        }
        else {
            map->lastRoad->next = node;
            map->lastRoad = map->lastRoad->next;
        }
        return true;
    }
    else {
        return false;
    }
}

bool addRoadToCity(City *city, Road *road) {
    ListOfRoads *node = addListOfRoadsNode(road);

    if (node != NULL) {
        if (city->firstRoad == NULL) {
            city->firstRoad = node;
            city->lastRoad = city->firstRoad;
        }
        else {
            city->lastRoad->next = node;
            city->lastRoad = city->lastRoad->next;
        }
        return true;
    }
    else {
        return false;
    }
}

Road* findRoad(City *cityA, City *cityB) {
    Road *road = NULL;
    ListOfRoads *node = cityA->firstRoad;
    bool found = false;

    while ((node != NULL) && !found) {
        if ((node->road->cityA == cityB) || (node->road->cityB == cityB)) {
            road = node->road;
            found = true;
        }
        node = node->next;
    }

    return road;
}

/**
 * @brief Tworzy węzeł listy dróg krajowych przypisując do niego podaną
 * drogę krajową.
 * @param[in] route          – wskaźnik na drogę krajową.
 * @return Wskaźnik na utworzony element listy lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
static ListOfRoutes* addListOfRoutesNode(Route *route) {
    ListOfRoutes *node = NULL;

    node = (ListOfRoutes *)malloc(sizeof(ListOfRoutes));
    if (node != NULL) {
        node->route = route;
        node->next = NULL;
    }

    return node;
}

bool addRouteToMap(Map *map, Route *route) {
    ListOfRoutes *node = addListOfRoutesNode(route);

    if (node != NULL) {
        if (map->firstRoute == NULL) {
            map->firstRoute = node;
            map->lastRoute = map->firstRoute;
        }
        else {
            map->lastRoute->next = node;
            map->lastRoute = node;
        }
        return true;
    }
    else {
        return false;
    }
}

void setRouteValues(Route *route, unsigned id, City *firstCity, City *lastCity) {
    route->id = id;
    route->firstRoad = NULL;
    route->lastRoad = NULL;
    route->firstCity = firstCity;
    route->lastCity = lastCity;
}

bool addRoadToRoute(Route *route, Road *road) {
    ListOfRoads *node = addListOfRoadsNode(road);

    if (node != NULL) {
        if (route->firstRoad == NULL) {
            route->firstRoad = node;
            route->lastRoad = route->firstRoad;
        }
        else {
            route->lastRoad->next = node;
            route->lastRoad = route->lastRoad->next;
        }
        return true;
    }
    else {
        return false;
    }
}

bool addRouteToRoad(Road *road, Route *route) {
    ListOfRoutes *node = addListOfRoutesNode(route);

    if (node != NULL) {
        if (road->firstRoute == NULL) {
            road->firstRoute = node;
            road->lastRoute = road->firstRoute;
        }
        else {
            road->lastRoute->next = node;
            road->lastRoute = node;
        }
        return true;
    }
    else {
        return false;
    }
}

Route* findRoute(Map *map, unsigned id) {
    Route *route = NULL;
    ListOfRoutes *node = map->firstRoute;
    bool found = false;

    while ((node != NULL) && !found) {
        if (node->route->id == id) {
            route = node->route;
            found = true;
        }
        node = node->next;
    }

    return route;
}

bool isCityInRoute(Route *route, City *city) {
    ListOfRoads *node;
    Road *road;
    bool found = false;

    node = route->firstRoad;
    while ((node != NULL) && !found) {
        road = node->road;
        if ((road->cityA == city) || (road->cityB == city)) {
            found = true;
        }
        node = node->next;
    }

    return found;
}

void markVerticies(Route *route, bool value) {
    ListOfRoads *node;

    node = route->firstRoad;
    while (node != NULL) {
        node->road->cityA->vertex->usedInRoute = value;
        node->road->cityB->vertex->usedInRoute = value;
        node = node->next;
    }
}
