/** @file
 * Plik zawierający główne funkcje operujące na mapie.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include "map.h"
#include "dijkstra.h"
#include "help-functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

Map* newMap(void) {
    Map *map = NULL; 

    map = (Map *)malloc(sizeof(Map));

    if (map != NULL) {
        setMapValues(map);
    }

    return map;
}

void deleteMap(Map *map) {
    if (map != NULL) {
        for (int i = 0; i < map->numberOfCities; i++) {
            freeCity(map->cities[i]);
        }
        freeRoads(map->firstRoad);
        freeRoutes(map->firstRoute);
        free(map->cities);
        free(map);
    }
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    Road *road = NULL;
    City *cityA = NULL, *cityB = NULL;
    bool correct = true;

    if ((map == NULL) || !isLengthCorrect(length) || !isYearCorrect(builtYear)
        || (strcmp(city1, city2) == 0) || !isCityNameCorrect(city1)
        || !isCityNameCorrect(city2)) {
        correct = false;
    }
    else {
        cityA = findCity(map, city1);
        cityB = findCity(map, city2);
        if (cityA == NULL) {
            cityA = addCity(map, city1);
        }
        if (cityB == NULL) {
            cityB = addCity(map, city2);
        }

        if ((cityA == NULL) || (cityB == NULL)) {
            correct = false;
        }
        else {
            road = findRoad(cityA, cityB);
            if (road != NULL) {
                correct = false;
            }
            else {
                road = newRoad(map, cityA, cityB, length, builtYear);
                if (road == NULL) {
                    correct = false;
                }
            }
        }
    }

    return correct;
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    City *cityA, *cityB;
    Road *road;
    bool correct = true;

    if ((map == NULL) || !isYearCorrect(repairYear)
        || (strcmp(city1, city2) == 0)) {
        correct = false;
    }
    else {
        cityA = findCity(map, city1);
        cityB = findCity(map, city2);
        if ((cityA == NULL) || (cityB == NULL)) {
            correct = false;
        }
        else {
            road = findRoad(cityA, cityB);
            if (road == NULL) {
                correct = false;
            }
            else {
                if (road->year > repairYear) {
                    correct = false;
                }
                else {
                    road->year = repairYear;
                }
            }
        }
    }

    return correct;
}

bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
    Route *route;
    City *start, *stop;
    int previous[map->numberOfCities];
    bool correct = true;

    if ((map == NULL) || !isRouteIdCorrect(routeId) 
        || (strcmp(city1, city2) == 0)) {
        correct = false;
    }
    else {
        route = findRoute(map, routeId);
        if (route != NULL) {
            correct = false;
        }
        else {
            start = findCity(map, city1);
            stop = findCity(map, city2);
            if ((start == NULL) || (stop == NULL)) {
                correct = false;
            }
            else {
                dijkstra(map, stop->vertex->id, start->vertex->id, previous);
                if (previous[start->vertex->id] == -1) {
                    correct = false;
                }
                else {
                    route = addRoute(map, routeId, start->vertex->id,
                                     stop->vertex->id, previous);
                    if (route == NULL) {
                        correct = false;
                    }
                    else {
                        if (!addRouteToMap(map, route)) {
                            correct = false;
                        }
                    }
                }
            }
        }
    }

    return correct;
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
    City *stop;
    Route *route;
    bool correct = true;
    int start;
    int distance1, distance2;
    int previous1[map->numberOfCities], previous2[map->numberOfCities];

    if ((map == NULL) || !isRouteIdCorrect(routeId)) {
        correct = false;
    }
    else {
        stop = findCity(map, city);
        route = findRoute(map, routeId);
        if ((stop == NULL) || (route == NULL)) {
            correct = false;
        }
        else {
            if (isCityInRoute(route, stop)) {
                correct = false;
            }
            else {
                markVerticies(route, true);
                distance1 = dijkstra(map, route->firstCity->vertex->id,
                                     stop->vertex->id, previous1);
                distance2 = dijkstra(map, stop->vertex->id,
                                     route->lastCity->vertex->id, previous2);
                markVerticies(route, false);
                if ((distance1 != INT_MAX) && (distance1 < distance2)) {
                    start = stop->vertex->id;
                    if (!extendFromFirstCity(map, route, start, previous1)) {
                        correct = false;
                    }
                }
                else if ((distance2 != INT_MAX) && (distance2 < distance1)) {
                    start = route->lastCity->vertex->id;
                    if (!extendFromLastCity(map, route, start, previous2)) {
                        correct = false;
                    }
                }
                else {
                    correct = false;
                }
            }
        }
    }

    return correct;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
    bool correct = true;
    City *cityA, *cityB;
    Road *road;

    if ((map == NULL) || (strcmp(city1, city2) == 0)) {
        correct = false;
    }
    else {
        cityA = findCity(map, city1);
        cityB = findCity(map, city2);
        if ((cityA == NULL) || (cityB == NULL)) {
            correct = false;
        }
        else {
            road = findRoad(cityA, cityB);
            if (road == NULL) {
                correct = false;
            }
            else {
                road->toRemove = true;
                if (!canRoadBeRemoved(map, road)) {
                    correct = false;
                    road->toRemove = false;
                }
                else {
                    removeRoadWithFillingRoutes(map, road);
                    removeListOfRoadsNodeFromCity(cityA, road);
                    removeListOfRoadsNodeFromCity(cityB, road);
                    removeRoadFromMap(map, road);
                }
            }
        }
    }

    return correct;
}

bool removeRoute(Map *map, unsigned routeId) {
    bool correct = true;
    ListOfRoutes *nodeToRemove, *previousNode;

    if ((map == NULL) || !isRouteIdCorrect(routeId)) {
        correct = false;
    }
    else {
        previousNode = NULL;
        nodeToRemove = map->firstRoute;

        while ((nodeToRemove != NULL)
               && (nodeToRemove->route->id != routeId)) {
            previousNode = nodeToRemove;
            nodeToRemove = nodeToRemove->next;
        }

        if (nodeToRemove == NULL) {
            correct = false;
        }
        else {
            removeRouteFromRoads(nodeToRemove->route);
            if (nodeToRemove == map->firstRoute) {
                if (nodeToRemove == map->lastRoute) {
                    map->firstRoute = NULL;
                    map->lastRoute = NULL;
                }
                else {
                    map->firstRoute = nodeToRemove->next;
                }
            }
            else if (nodeToRemove == map->lastRoute) {
                map->lastRoute = previousNode;
                previousNode->next = NULL;
            }
            else {
                previousNode->next = nodeToRemove->next;
            }

            free(nodeToRemove->route);
            free(nodeToRemove);
        }
    }

    return correct;
}

char const* getRouteDescription(Map *map, unsigned routeId) {
    const char *description = NULL;
    char *helpStr = NULL;
    Route *route;

    if (map != NULL) {
        route = findRoute(map, routeId);
        if (route != NULL) {
            description = routeDescription(route);
        }
        else {
            helpStr = (char *)malloc(sizeof(char));
            if (helpStr != NULL) {
                helpStr[0] = '\0';
                description = helpStr;
            }
        }
    }

    return description;
}
