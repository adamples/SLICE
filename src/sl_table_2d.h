#ifndef _SL_TABLE_2D_H_
#define _SL_TABLE_2D_H_

#include "slice.h"


	/**
	 * @file
	 *
	 * Tablica dwuwymiarowa
	 *
	 * @ref SlTable2D alokuje pamięć na potrzeby tablicy dwuwymiarowej w jednym,
	 * ciągłym bloku. Różnicą w stosunku do @ref SlTable jest przesuwanie danych
	 * po realokacji, tak aby zawsze możliwy był dostęp do nich na tych samych
	 * zasadach:
	 *
	 * @code
	 * int				width = 32;
	 * int				height = 32;
	 * SlFloat			*table;
	 * SlTable2D		t2d = sl_table_2d_new (&table, sizeof (SlFloat));
	 *
	 * sl_table_2d_resize (t2d, width, height);
	 * printf ("table [9][5] = %g.\n", table [5 + width * 9]);
	 * @endcode
	 */

		/** Tablica dwuwymiarowa */
	typedef struct _SlTable2D *SlTable2D;

	/** Wewnętrzny identyfikator typu SlTable2D */
#define SL_TABLE_2D_TYPE (sl_table_2d_type ())

			/** Tworzy identyfikator typu SlTable2D */
		SlType				sl_table_2d_type ();

			/**
			 * Tworzy tablicę dwuwymiarową
			 *
			 * Tworzy nową tablicę dwuwymiarową dla zmiennej wskazywanej przez @c data.
			 * Po utworzeniu obiektu zmienna będzie miała wartość @c NULL.
			 *
			 * @return		Nowo utworzony obiekt tablicy
			 */
#define sl_table_2d_new(data,element_size) (_sl_table_2d_new((void **) (data), (element_size)))
		SlTable2D			_sl_table_2d_new (SlPointer *data, size_t element_size);

			/**
			 * Zmienia rozmiar tablicy
			 *
			 * Zmienia rozmiar tablicy @c table na <tt>width&#215;height</tt>. Jeśli
			 * wymiary są nieprawidłowe, nie zostanie podjęte żadne działanie.
			 *
			 * @param table		Tablica
			 * @param width		Nowa szerokość tablicy
			 * @param height		Nowa wysokość tablicy
			 */
		void					sl_table_2d_resize (SlPointer table, int width, int height);


#endif