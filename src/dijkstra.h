/** @file
 * Interfejs pliku zawierającego funkcje do obliczania najkrótszej ścieżki
 * w grafie.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include "map.h"
#include "help-structures.h"

/**
 * Struktura przechowująca kopiec do obliczania najkrótszej
 * ścieżki w grafie.
 */
typedef struct BinaryHeap BinaryHeap;

/** 
 * @brief Struktura przechowująca kopiec do obliczania najkrótszej ścieżki
 * w grafie.
 * Struktura binarnego kopca zawiera:
 * rozmiar kopca, czyli liczbę wierzchołków;
 * tablicę pozycji wierzchołków w tablicy;
 * tablicę wskaźników na wierzchołki jednoznacznie powiązane z miastami.
 */
struct BinaryHeap {
    int size;               ///< rozmiar kopca
    int *verticesPositions; ///< tablica pozycji wierzchołków w kopcu
    Vertex **vertices;      ///< tablica wskaźników na wierzchołki
};

/** @brief Znajduje najkrótszą ścieżkę w grafie.
 * @param[in] map            – wskaźnik na strukturę mapy;
 * @param[in] start          – identyfikator miasta początkowego;
 * @param[in] stop           – identyfikator miasta końcowego;
 * @param[in,out] previous   – tablica identyfikatorów poprzedników miast.
 * @return Długość najkrótszej ścieżki w grafie od miasta początkowego
 * do miasta końcowego.
 */
int dijkstra(Map *map, int start, int stop, int previous[]);

#endif /* __DIJKSTRA_H__ */
