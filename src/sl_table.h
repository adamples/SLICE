#ifndef _SL_TABLE_H_
#define _SL_TABLE_H_


#include "slice.h"


	/**
	 * @file
	 *
	 * Alokacja ciągłych obszarów pamięci.
	 *
	 * Obiekt tablicy ma za zadanie ułatwić alokację ciągłych obszarów pamięci,
	 * głównie, ale nie tylko, na potrzeby tablic. Pojedynczy obiekt może być użyty
	 * do zarządzania pamięcią jednej tablicy.
	 *
	 * Do zmiany rozmiaru przydzielonej pamięci służy metoda @ref sl_table_resize.
	 * Należy pamiętać, że jej wywołanie może nie mieć żadnych skutków ubocznych, jak
	 * również może wymagać skopiowania całej zawartości tablicy w nowe miejsce. W celu
	 * uzyskania dobrej wydajności kluczowe jest dobranie odpowiedniego rozmiaru bloku.
	 *
	 * Pamięć jest alokowana w blokach o stałym rozmiarze. Domyślny rozmiar bloku
	 * można zmienić za pomocą metody @ref sl_table_set_block_size.
	 *
	 * Przykład użycia obiektu:
	 *
	 * @code
	 * 	void foo (void) {
	 * 		SlFloat		*table = NULL;
	 * 		SlTable		supervisor = sl_table_new  (&table, sizeof (*table));
	 *  
	 * 			// Ustawiamy rozmiar tablicy na jeden element
	 * 		sl_table_resize (supervisor, 1);
	 * 			// Zmieniamy wartość elementu
	 * 		table [0] = 1.5;
	 * 			// Niepoprawne, ale nie spowoduje błędu, gdyż przydzielono
	 * 			// SL_DEFAULT_BLOCK_SIZE bajtów pamięci
	 * 		table [1] = 1.67;
	 * 			// Spowoduje naruszenie ochrony pamięci
	 * 		table [32] = 3.14;
	 * 	}
	 * @endcode
	 */

	/** Domyślny rozmiar bloku pamięci (64b) */
#define SL_DEFAULT_BLOCK_SIZE (64)

		/** Tablica */
	typedef struct _SlTable *SlTable;

	/** Wewnętrzny identyfikator typu tablicy */
#define SL_TABLE_TYPE (sl_table_type ())

			/** Tworzy identyfikator typu tablicy */
		SlType				sl_table_type ();

			/**
			 * Tworzy nową tablicę
			 *
			 * Tworzy nową tablicę dla zmiennej wskazywanej przez @c data. Zmienna ta może
			 * być wskaźnikiem dowolnego typu, koniecznie niezwiązanym z żadnym przydzielonym
			 * obszarem pamięci. Po utworzeniu obiektu zmienna wskazywana przez @c data
			 * będzie miała wartość @c NULL.
			 *
			 * @param data					Wskaźnik na zmienną wskaźnikową, której pamięcią
			 * 								będzie zarządzał obiekt
			 * @param element_size		Rozmiar elementu tablicy w bajtach
			 * @return						Nowo utworzony obiekt tablicy
			 */
#define sl_table_new(data,element_size) (_sl_table_new((void **) (data),(element_size)))
		SlTable			_sl_table_new (SlPointer *data, int element_size);

			/**
			 * Zmienia rozmiar tablicy
			 *
			 * Ustawia rozmiar tablicy @c table na @c size elementów. Pamięć dla
			 * nowych elementów zostaje wyzerowana.
			 *
			 * @param table		Tablica
			 * @param size			Nowy rozmiar tablicy
			 */
		void					sl_table_resize (SlPointer table, int size);

			/**
			 * Zmienia rozmiar bloku
			 *
			 * Ustawia rozmiar bloku na @c block_size. Rozmiar jest podawany w bajtach.
			 * Wywołanie tej metody może mieć skutki uboczne w postaci realokacji pamięci.
			 *
			 * @param table			Tablica
			 * @param block_size		Nowy rozmiar bloku
			 */
		void					sl_table_set_block_size (SlPointer table, int block_size);

			/**
			 * Zwraca aktualny rozmiar bloku
			 *
			 * Zwraca bieżący rozmiar bloku tablicy @c table.
			 *
			 * @param table		Tablica
			 * @return				Rozmiar bloku tablicy
			 */
		int					sl_table_get_block_size (SlPointer table);


#endif
