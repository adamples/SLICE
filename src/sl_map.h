#ifndef _SL_MAP_H_
#define _SL_MAP_H_

#include "slice.h"


		/**
		 * @file
		 *
		 * Mapa
		 *
		 * @c SlMap to abstrakcyjny typ dla obiektów przechowujących pary (unikalny klucz,
		 * wartość), gdzie zarówno klucz jak i wartość są typu wskaźnikowego. Możliwe
		 * jest sprawdzenie, czy klucz został umieszczony na mapie (@ref sl_map_isset),
		 * dodanie nowej pary (@ref sl_map_add) oraz pobranie wartości dla danego
		 * klucza (@ref sl_map_get).
		 *
		 * Przykładowe typy, zarówno dla klucza, jak i wartości to m.in. wszystkie
		 * typy obiektowe oraz ciągi znaków (@ref SlString).
		 *
		 * Relacja równości kluczy jest ustalana na poziomie implementacji w typie
		 * potomnym (patrz @ref SlMapSI).
		 *
		 */

		/** Mapa */
	typedef struct _SlMap *SlMap;

	/** Wewnętrzny identyfikator typu SlMap */
#define SL_MAP_TYPE (sl_map_type ())

			/** Tworzy wewnętrzny identyfikator typu SlMap */
		SlType		sl_map_type ();

			/**
			 * Sprawdza, czy klucz istnieje
			 *
			 * Sprawdza czy w mapie @c map został zdefiniowany klucz równy @ref key.
			 *
			 * @param map		Mapa
			 * @param key		Klucz
			 * @return			@c true, jeśli wartość dla klucza została zdefiniowana
			 */
		SlBool		sl_map_isset (SlPointer map, SlPointer key);

			/**
			 * Dodaje parę do mapy
			 *
			 * Dodaje do mapy @c map parę (@ref key, @ref value). Jeśli klucz został
			 * wcześniej dodany, nie jest wykonywane żadne działanie.
			 *
			 * @param map			Mapa
			 * @param key			Klucz
			 * @param value		Wartość
			 */
		void			sl_map_add (SlPointer map, SlPointer key, SlPointer value);

			/**
			 * Pobiera wartość
			 *
			 * Pobiera z mapy @c map wartość zdefiniowaną dla klucza @c key. Jeśli
			 * wartość nie jest zdefiniowana, zwraca @c NULL.
			 *
			 * @param map		Mapa
			 * @param key		Klucz
			 * @return			Wartość dla klucza lub @c NULL
			 */
		SlPointer	sl_map_get (SlPointer map, SlPointer key);


#endif
