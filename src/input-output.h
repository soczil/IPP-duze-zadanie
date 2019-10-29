/** @file
 * Interfejs pliku zawierającego funkcje do obsługi interfejsu tekstowego.
 * 
 * @author Karol Soczewica <ks394468@students.mimuw.edu.pl>
 * @date 01.09.2019
 */

#ifndef __INPUT_OUTPUT_H__
#define __INPUT_OUTPUT_H__

/**
 * @brief Czyta pojedyncze linie ze standardowego wejścia i wykonuje polecenia.
 * Tworzy nową mapę, a następnie pojedynczo wczytuje linie teskstu
 * ze standardowego wejścia, dopóki nie napotka znaku końca danych. Pomija
 * linie rozpoczynające się znakami '#' lub '\n'. Jeśli linia tekstu jest
 * poprawna tj. kończy się znakiem nowej linii, wywołuje od niej funkcję
 * wybierającą funkcję do obsługi wczytanego polecenia.
 */
void readAndDoCommands(void);

#endif /* __INPUT_OUTPUT_H__ */
