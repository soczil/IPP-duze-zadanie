/** @file
 * Plik zawierający funkcje do obliczania najkrótszej ścieżki w grafie.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#include "dijkstra.h"

#include <stdlib.h>
#include <limits.h>

/**
 * @brief Zwalnia pamięć zaalokowaną na kopiec.
 * @param[in] heap           – wskaźnik na strukturę kopca.
 */
static void removeBinaryHeap(BinaryHeap *heap) {
    free(heap->vertices);
    free(heap->verticesPositions);
    free(heap);
}

/** 
 * @brief Tworzy binarny kopiec.
 * @param[in] numberOfCities – liczba miast w mapie.
 * @return Wskaźnik na utworzony kopiec lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
static BinaryHeap* newBinaryHeap(int numberOfCities) {
    BinaryHeap *heap = (BinaryHeap *)malloc(sizeof(BinaryHeap));
    
    if (heap != NULL) {
        heap->size = 0;
        heap->verticesPositions = (int *)malloc(numberOfCities * sizeof(int));
        heap->vertices = (Vertex **)malloc(numberOfCities * sizeof(Vertex *));
        if ((heap->verticesPositions == NULL) || (heap->vertices == NULL)) {
            removeBinaryHeap(heap);
            heap = NULL;
        }
    }

    return heap;
}

/**
 * @brief Sprawdza, który z danych wierzchołków jest w mniejeszej odległości
 * od źródła.
 * @param[in] heap           – wskaźnik na strukturę kopca;
 * @param[in] min            – identyfikator wierzchołka;
 * @param[in] son            – identyfikator wierzchołka.
 * @return Identyfikator wierzchołka o mniejszej odległości od źródła.
 */
static int minimalVertexIndex(BinaryHeap *heap, int min, int son) {
    Vertex *minVertex, *helpVertex;

    if (son < heap->size) {
        minVertex = heap->vertices[min];
        helpVertex = heap->vertices[son];
        if (helpVertex->distanceFromSource < minVertex->distanceFromSource) {
            min = son;
        }
    }

    return min;
}

/**
 * @brief Zamienia ze sobą wskaźniki na dwa wierzchołki.
 * @param[in] v1             – wskaźnik na wskaźnik na strukturę wierzchołka;
 * @param[in] v2             – wskaźnik na wskaźnik na strukturę wierzchołka.
 */
static void swapVertices(Vertex **v1, Vertex **v2) {
    Vertex *temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

/**
 * @brief Zamienia ze sobą pozycję dwóch wierzchołków.
 * @param[in,out] heap       – wskaźnik na strukturę kopca;
 * @param[in] v1             – wskaźnik na strukturę wierzchołka;
 * @param[in] v2             – wskaźnik na strukturę wierzchołka;
 * @param[in] position1      – pozycja pierwszego wierzchołka;
 * @param[in] position2      – pozycja drugiego wierzchołka.
 */
static void swapVerticesPositions(BinaryHeap *heap, Vertex *v1, Vertex *v2,
                                  int position1, int position2) {
    heap->verticesPositions[v1->id] = position2;
    heap->verticesPositions[v2->id] = position1;
}

/**
 * @brief Przestawia elementy w kopcu, aby powstał kopiec minimalny.
 * @param[in,out] heap       – wskaźnik na strukturę kopca;
 * @param[in] index          – indeks, od którego zaczynamy wykonywać zamiany.
 */
static void makeMinHeap(BinaryHeap *heap, int index) {
    int minIndex;
    int lSonIndex = 2 * index + 1;
    int rSonIndex = lSonIndex + 1;

    minIndex = minimalVertexIndex(heap, index, lSonIndex);
    minIndex = minimalVertexIndex(heap, minIndex, rSonIndex);

    if (index != minIndex) {
        swapVerticesPositions(heap, heap->vertices[index],
                              heap->vertices[minIndex], index, minIndex);
        swapVertices(&(heap->vertices[index]), &(heap->vertices[minIndex]));
        makeMinHeap(heap, minIndex);
    }
}

/**
 * @brief Sprawdza, czy dany kopiec jest pusty.
 * @param[in] heap           – wskaźnik na strukturę kopca.
 * @return Wartość @p true, jeśli kopiec jest pusty.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool isEmpty(BinaryHeap *heap) {
    if (heap->size > 0) {
        return false;
    }
    else {
        return true;
    }
}

/** 
 * @brief Daje w wyniku wierzchołek o najmiejszej odległośći od źródła.
 * @param[in,out] heap       – wskaźnik na strukturę kopca.
 * @return Wskaźnik na wierzchołek o najmniejszej odległości od źródła lub
 * NULL, gdy kopiec jest pusty.
 */
static Vertex* extractMin(BinaryHeap *heap) {
    Vertex *min = NULL, *last;
    int firstIndex = 0, lastIndex = heap->size - 1;

    if (!isEmpty(heap)) {
        min = heap->vertices[firstIndex];
        last = heap->vertices[lastIndex];
        heap->vertices[firstIndex] = last;
        swapVerticesPositions(heap, min, last, firstIndex, lastIndex);
        heap->size -= 1;
        makeMinHeap(heap, firstIndex);
    }

    return min;
}

/**
 * @brief Oblicza indeks rodzica wierzchołka o danym identyfikatorze.
 * @param[in] sonIndex       – identyfikator wierzchołka.
 * @return Identyfikator rodzica danego wierzchołka.
 */
static int parentIndex(int sonIndex) {
    return ((sonIndex - 1) / 2);
}

/**
 * @brief Wykonuje operację zmniejszenia klucza w kopcu.
 * @param[in,out] heap       – wskaźnik na strukturę kopca;
 * @param[in] id             – identyfikator wierzchołka;
 * @param[in] distance       – odległość wierzchołka od źródła.
 */
static void decreasePriority(BinaryHeap *heap, int id, int distance) {
    Vertex *son, *father;
    int sonIndex = heap->verticesPositions[id];
    int fatherIndex = parentIndex(sonIndex);
    bool end = false;

    heap->vertices[sonIndex]->distanceFromSource = distance;
    while ((fatherIndex >= 0) && !end) {
        son = heap->vertices[sonIndex];
        father = heap->vertices[fatherIndex];
        if (son->distanceFromSource < father->distanceFromSource) {
            swapVerticesPositions(heap, son, father, sonIndex, fatherIndex);
            swapVertices(&(heap->vertices[sonIndex]),
                         &(heap->vertices[fatherIndex]));
            sonIndex = fatherIndex;
            fatherIndex = parentIndex(sonIndex);
        }
        else {
            end = true;
        }
    }
}

/**
 * @brief Sprawdza, czy podany wierzchołek jest w kopcu.
 * @param[in] heap           – wskaźnik na strukturę kopca;
 * @param[in] vertex         – wskaźnik na strukturę wierzchołka.
 * @return Wartość @p true, jeśli dany wierzchołek jest w kopcu.
 * Wartość @p false, w przeciwnym przypadku.
 */
static bool inHeap(BinaryHeap *heap, Vertex *vertex) {
    if (heap->verticesPositions[vertex->id] < heap->size) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Znajduje wierzchołek o podanym identyfikatorze i ustawia jego
 * odległość od źródła.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] id             – identyfikator wierzchołka;
 * @param[in] distance       – odległość wierzchołka od źródła.
 * @return Wskaźnik na znaleziony wierzchołek.
 */
static Vertex* findVertex(Map *map, int id, int distance) {
    Vertex *vertex = map->cities[id]->vertex;

    vertex->distanceFromSource = distance;

    return vertex;
}

/**
 * @brief Wypełnia kopiec oraz pomocnicze tablice.
 * @param[in,out] heap       – wskaźnik na strukturę kopca;
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in,out] dist       – tablica odległości od źródła;
 * @param[in,out] prev       – tablica identyfikatorów poprzedników miast.
 */
static void fillHeapAndArrays(BinaryHeap *heap, Map *map, int dist[], int prev[]) {
    for (int i = 0; i < heap->size; i++) {
        heap->vertices[i] = findVertex(map, i, INT_MAX);
        heap->verticesPositions[i] = i;
        heap->vertices[i]->road = NULL;
        heap->vertices[i]->oldestYear = INT_MAX;
        heap->vertices[i]->error = false;
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
}

int dijkstra(Map *map, int start, int stop, int previous[]) {
    City *city;
    Vertex *min, *vertex;
    int length;
    int distance[map->numberOfCities];
    bool found = false;
    BinaryHeap *heap = newBinaryHeap(map->numberOfCities);

    if (heap != NULL) {
        heap->size = map->numberOfCities;
        fillHeapAndArrays(heap, map, distance, previous);
        distance[start] = 0;
        decreasePriority(heap, start, 0);

        while (!isEmpty(heap) && !found) {
            min = extractMin(heap);
            if ((min->id == start) || (min->usedInRoute == false)) {
                city = map->cities[min->id];
                ListOfRoads *list = city->firstRoad;
                while (list != NULL) {
                    if (list->road->cityA == city) {
                        vertex = list->road->cityB->vertex;
                    }
                    else {
                        vertex = list->road->cityA->vertex;
                    }
                    if (inHeap(heap, vertex) && (distance[min->id] != INT_MAX)
                        && (list->road->toRemove == false)) {
                        length = list->road->length;
                        if (distance[min->id] + length < distance[vertex->id]) {
                            vertex->road = list->road;
                            previous[vertex->id] = min->id;
                            distance[vertex->id] = distance[min->id] + length;
                            if (vertex->oldestYear > min->oldestYear) {
                                vertex->oldestYear = min->oldestYear;
                            }
                            if (vertex->oldestYear > list->road->year) {
                                vertex->oldestYear = list->road->year;
                            }
                            vertex->error = min->error;
                            decreasePriority(heap, vertex->id, distance[vertex->id]);
                        }
                        else if (distance[min->id] + length == distance[vertex->id]) {
                            int oldest;
                            if (list->road->year > min->oldestYear) {
                                oldest = min->oldestYear;
                            }
                            else {
                                oldest = list->road->year;
                            }

                            if (vertex->oldestYear == oldest) {
                                vertex->error = true;
                            }
                            else if (vertex->oldestYear < oldest) {
                                vertex->road = list->road;
                                previous[vertex->id] = min->id;
                                distance[vertex->id] = distance[min->id] + length;
                                vertex->oldestYear = oldest;
                                vertex->error = min->error;
                                decreasePriority(heap, vertex->id, distance[vertex->id]);
                            }
                        }
                    }
                    list = list->next;
                }
                if (min->id == stop) {
                    found = true;
                }
            }
        }
        removeBinaryHeap(heap);

        if (map->cities[stop]->vertex->error == true) {
            previous[stop] = -1;
            distance[stop] = INT_MAX;
        }
        return distance[stop];
    }
    else {
        previous[stop] = -1;
        return INT_MAX;
    }
}
