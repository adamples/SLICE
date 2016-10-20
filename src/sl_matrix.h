#ifndef _SL_MATRIX_H_
#define _SL_MATRIX_H_


#include "slice.h"

	/**
	 * @file
	 *
	 * Macierz liczb zmiennoprzecinkowych
	 *
	 * Detailed desc
	 */

		/** Macierz liczb typu @ref SlFloat */
	typedef struct _SlMatrix *SlMatrix;

	/** Wewnętrzny identyfikator typu macierzy */
#define SL_MATRIX_TYPE (sl_matrix_type ())


			/**
			 * Zwraca wewnęrzny identyfikator typu macierzy
			 *
			 * Zwraca wewnęrzny identyfikator typu macierzy
			 *
			 * @return		Identyfikator typu
			 */
		SlType		sl_matrix_type ();

			/**
			 * Tworzy nową macierz.
			 *
			 * Tworzy nową pustą macierz.
			 * Aby stalić rozmiar macierzy należy użyć @ref sl_matrix_resize, aby
			 * dostać się do jej elementów &mdash; @ref sl_matrix_get oraz @ref sl_matrix_set.
			 *
			 * @return		Nowo utworzona macierz
			 */
		SlMatrix		sl_matrix_new (void);

			/**
			 * Tworzy nową macierz jednostkową
			 *
			 * Tworzy nową macierz jednostkową o rozmiarze @c size x @c size.
			 *
			 * @param size		rozmiar macierzy
			 * @return			Nowo utworzona macierz
			 */
		SlMatrix		sl_matrix_new_identity (int size);

			/* Dostęp do danych */

			/**
			 * Pobiera element macierzy
			 *
			 * Pobiera wartość elementu macierzy z wiersza @c row i kolumny @c column.
			 * Jeśli podane współrzędne są nieprawidłowe, zwracane jest 0.
			 *
			 * @param matrix		Macierz
			 * @param row			Numer wiersza
			 * @param column		Numer Kolumny
			 * @return				Wartość elementu o podanych współrzędnych
			 */
		SlFloat		sl_matrix_get (SlPointer matrix, int row, int column);

			/**
			 * Ustawia element macierzy
			 *
			 * Ustawia wartość elementu macierzy w wierszu @c row i kolumnie @c column.
			 * Jeśli podane współrzędne są nieprawidłowe, nie zostanie wykonane żadne
			 * działanie na macierzy.
			 *
			 * @param matrix		Macierz
			 * @param row			Numer wiersza
			 * @param column		Numer kolumny
			 * @param element		Wartość ustawianego elementu
			 */
		void			sl_matrix_set (SlPointer matrix, int row, int column, SlFloat element);

			/**
			 * Wypełnia macierz stałą wartością
			 *
			 * Ustawia wszystkie elementy macierzy na @c element
			 *
			 * @param matrix		Macierz
			 * @param element		Wartość, która ma się znaleźć w macierzy
			 */
		void			sl_matrix_fill (SlPointer matrix, SlFloat element);


			/**
			 * Kopiuje macierz.
			 *
			 * Kopiuje zawartość z macierzy @c source do @c destination. Macierz
			 * @c destination musi być poprawnie utworzonym obiektem @ref SlMatrix.
			 *
			 * @param source				Macierz źródłowa
			 * @param destination		Macierz docelowa
			 */
		void			sl_matrix_copy (SlPointer source, SlPointer destination);

			/**
			 * Klonuje macierz
			 *
			 * Tworzy nową macierz identyczną z podaną jako argument funkcji.
			 *
			 * @param source		Klonowana macierz
			 * @return				Nowo utworzona kopia macierzy
			 */
		SlMatrix		sl_matrix_clone (SlPointer source);

			/* Manipulacja rozmiarem */

			/**
			 * Szerokość macierzy
			 *
			 * Zwraca liczbę kolumn w macierzy
			 *
			 * @param matrix		Macierz
			 * @return				Liczba kolumn w @c matrix
			 */
		int			sl_matrix_width (SlPointer matrix);

			/**
			 * Wysokość macierzy
			 *
			 * Zwraca liczbę wierszy w macierzy
			 *
			 * @param matrix		Macierz
			 * @return				Liczba wierszy w @c matrix
			 */
		int			sl_matrix_height (SlPointer matrix);

			/**
			 * Zmienia rozmiar macierzy
			 *
			 * Ustawia liczbę kolumn i wierszy w macierzy. Podczas zmiany rozmiaru
			 * zawartość macierzy jest zachowywana. Przy zwiększaniu nowe wiersze i
			 * kolumny zawierają przypadkowe dane.
			 *
			 * Aby utworzyć pustą macierz jako oba parametry należy podać 0. W przeciwnym
			 * wypadku nie zostanie wykonane żadne działanie na macierzy.
			 *
			 * @param matrix		Macierz
			 * @param rows			Nowa liczba wierszy
			 * @param columns		Nowa liczba kolumn
			 */
		void			sl_matrix_resize (SlPointer matrix, int rows, int columns);

			/* Tworzenie macierzy */

			/**
			 * Ustawia macierz jednostkową
			 *
			 * Funkcja zamienia zawartość macierzy na macierz jednostkową o rozmiarze
			 * @c size.
			 *
			 * @param matrix		Macierz
			 * @param size			Rozmiar macierzy
			 */
		void			sl_matrix_setup_identity (SlPointer matrix, int size);

			/* Operacje elementarne */

			/**
			 * Zamienia wiersze macierzy
			 *
			 * Wykonuje elementarne przekształcenie wierszowe &mdash; zamianę wierszy.
			 *
			 * @param matrix		Macierz
			 * @param row_a		Numer pierwszego wiersza
			 * @param row_b		Numer drugiego wiersza
			 */
		void			sl_matrix_ero_switch (SlPointer matrix, int row_a, int row_b);

			/**
			 * Mnoży wiersz przez stałą
			 *
			 * Wykonuje elementarne przekształcenie wierszowe &mdash; pomnożenie wiersza przez stałą.
			 *
			 * @param matrix		Macierz
			 * @param row			Numer wiersza
			 * @param k				Stała
			 */
		void			sl_matrix_ero_multiply (SlPointer matrix, int row, SlFloat k);

			/**
			 * Dodaje wiersz do innego wiersza
			 *
			 * Wykonuje elementarne przekształcenie wierszowe &mdash; dodanie do wiersza
			 * innego, pomnożonego przez stałą.
			 *
			 * @param matrix		Macierz
			 * @param row_a		Wiersz, który zostanie pomnożony i dodany do drugiego
			 * @param row_b		Wiersz, do którego pierwszy zostanie dodany
			 * @param k				Stała, przez którą pierwszy wiersz zostanie pomnożony
			 */
		void			sl_matrix_ero_add (SlPointer matrix, int row_a, int row_b, SlFloat k);

			/**
			 * Zamienia kolumny macierzy
			 *
			 * Wykonuje elementarne przekształcenie kolumnowe &mdash; zamianę kolumn.
			 *
			 * @param matrix		Macierz
			 * @param row_a		Numer pierwszej kolumny
			 * @param row_b		Numer drugiej kolumny
			 */
		void			sl_matrix_eco_switch (SlPointer matrix, int col_a, int col_b);


			/**
			 * Usuwa wiersz macierzy
			 *
			 * Usuwa z macierzy @c matrix @r r-ty wiesz.
			 *
			 * @param matrix		Macierz
			 * @param r				Numer wiersza do usunięcia
			 */
		void			sl_matrix_delete_row (SlPointer matrix, int r);

			/**
			 * Usuwa kolumnę macierzy
			 *
			 * Usuwa z macierzy @c matrix @r r-tą kolumnę.
			 *
			 * @param matrix		Macierz
			 * @param r				Numer kolumny do usunięcia
			 */
		void			sl_matrix_delete_column (SlPointer matrix, int c);

			/* Arytmetyka */

			/**
			 * Sprawdza, czy macierze da się pomnożyć.
			 *
			 * Sprawdza, czy można pomnożyć macierze, tzn. czy liczba kolumn mnożnej
			 * jest równa liczbie wierszy mnożnika. Jeśli macierze są puste, zwraca
			 * @c false.
			 *
			 * @param multiplicant		Mnożna
			 * @param multiplier			Mnożnik
			 * @return						@c true, jeśli macierze da się pomnożyć
			 */
		SlBool		sl_matrix_can_multiply (SlPointer multiplicant, SlPointer multiplier);

			/**
			 * Mnoży macierze
			 *
			 * Mnoży macierz @c multiplicant przez @c multiplier i zwraca nową macierz
			 * zawierającą wynik.
			 *
			 * @param multiplicant		Mnożna
			 * @param multiplier			Mnożnik
			 * @return						Mnożna * Mnożnik lub NULL, jeśli macierze nie mogą zostać pomnożone
			 */
		SlMatrix		sl_matrix_multiply (SlPointer multiplicant, SlPointer multiplier);

			/* Obliczenia */

			/**
			 * Sprawdza, czy macierz jest kwadratowa
			 *
			 * Jeśli macierz jest kwadratowa, zwraca @c true. W przypadku macierzy
			 * pustej zwraca @c false.
			 *
			 * @param matrix		Sprawdzana macierz
			 * @return				@c true jeśli macierz jest kwadratowa
			 */
		SlBool		sl_matrix_is_square (SlPointer matrix);

			/**
			 * Sprawdza, czy macierz jest odwracalna
			 *
			 * Jeśli macierz jest odwracalna, zwraca @c true. W przypadku macierzy
			 * pustej zwraca @c false.
			 *
			 * @param matrix		Sprawdzana macierz
			 * @return				@c true jeśli macierz jest odwracalna
			 */
		SlBool		sl_matrix_is_invertible (SlPointer matrix);

			/**
			 * Wyznacznik macierzy
			 *
			 * Oblicza wyznacznik macierzy. Jeśli podana jako argument macierz nie
			 * jest kwadratowa, zwracane jest 0.
			 *
			 * @param matrix		Macierz
			 * @return				Obliczony wyznacznik macierzy
			 */
		SlFloat		sl_matrix_determinant (SlPointer matrix);

			/**
			 * Odwraca macierz
			 *
			 * Odwraca macierz zastępując oryginalną zawartość. Jeśli macierz jest
			 * nieodwracalna, nie zostaje wykonane na niej żadne działanie.
			 *
			 * @param matrix		Odwracana macierz
			 * @return				@c true, jeśli operacja się powiodła (macierz jest odwracalna)
			 */
		SlBool		sl_matrix_invert (SlPointer matrix);


			/**
			 * Drukuje macierz
			 *
			 * Drukuje macierz w składni programu MatLab na standardowe wyjście, za pomocą
			 * funkcji @c printf.
			 *
			 * @param matrix		Drukowana macierz
			 * @param name			Nazwa, która zostanie wyświetlona w wydruku. Funkcja
			 * 						nie wykonuje żadnych testów celem sprawdzenia poprawności
			 * 						nazwy.
			 */
		void			sl_matrix_print (SlPointer matrix, SlString name);


#endif
