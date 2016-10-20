#ifndef _SL_MAP_SI_H_
#define _SL_MAP_SI_H_

#include "slice.h"

	/**
	 * @file
	 *
	 * Mapa (ciąg znaków, liczba całkowita)
	 *
	 * @ref SlMapSI mapuje ciągi znaków (@ref SlString) na liczby całkowite. Dwa
	 * ciągi są uważane za jednakowe, jeśli ich zawartość jest identyczna.
	 */

		/** Mapa (ciąg znaków, liczba całkowita) */
	typedef struct _SlMapSI *SlMapSI;

	/** Wewnętrzny identyfikator typu dla SlMapSI */
#define SL_MAP_SI_TYPE (sl_map_si_type ())

			/** Zwraca wewnętrzny identyfikator typu dla SlMapSI */
		SlType		sl_map_si_type ();

			/**
			 * Tworzy mapę (ciąg znaków, liczba całkowita)
			 *
			 * Tworzy nową mapę (ciąg znaków, liczba całkowita)
			 *
			 * @return		Nowo utworzona mapa
			 */
		SlMapSI		sl_map_si_new ();

			/**
			 * Dodaję parę do mapy
			 *
			 * Dodaje do mapy @c map parę (@c key, @c value). Klucz jest klonowany
			 * przez obiekt. Jeśli klucz został wcześniej umieszczony w mapie, nie
			 * zostanie podjęte żadne działanie.
			 *
			 * @param map			Mapa
			 * @param key			Klucz (ciąg znaków)
			 * @param value		Wartość (liczba całkowita)
			 */
		void			sl_map_si_add (SlPointer map, SlString key, size_t value);

			/**
			 * Pobiera wartość
			 *
			 * Pobiera z mapy @c map wartość zdefiniowaną dla klucza @c key. Jeśli wartość
			 * nie jest zdefiniowana, zwraca @c NULL.
			 *
			 * @param map		Mapa
			 * @param key		Klucz
			 * @return			Wartość dla klucza lub @c NULL
			 */
		int			sl_map_si_get (SlPointer map, SlString key);


#endif