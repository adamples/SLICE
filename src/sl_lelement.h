#ifndef _SL_LELEMENT_H_
#define _SL_LELEMENT_H_

#include "slice.h"


		/**
		 * @file
		 *
		 * Elementy liniowe.
		 *
		 * Plik zawiera definicje typów obiektowych reprezenujących elementy liniowe:
		 * 	- @ref SlLElement (typ abstrakcyjny),
		 * 	- @ref SlResistor – opór elektryczny,
		 * 	- @ref SlVSource – idealne, niezależne wymuszenie napięciowe,
		 * 	- @ref SlCSource – idealne, niezależne wymuszenie prądowe.
		 *
		 * Każdy element posiada swoją nazwę-identyfikator, który musi być dla niego
		 * unikalny w skali obwodu. Podczas tworzenia elementu łańcuch znaków podany
		 * jako nazwa jest kopiowany przez obiekt.
		 *
		 * Cechą charakterystyczną elementów jest posiadanie przez nie dwóch elektrod,
		 * z których jedna jest umownie nazywana "dodatnią" (o potencjale V<sub>n<sup>+</sup></sub>),
		 * natomiast druga – "ujemną" (V<sub>n<sup>-</sup></sub>).
		 * Dokładne informacje na ten temat są podane w opisach funkcji tworzących
		 * obiekty.
		 */

		/** Element liniowy */
	typedef struct _SlLElement *SlLElement;

	/** Wewnętrzny identyfikator typu SlLElement */
#define SL_LELEMENT_TYPE (sl_lelement_type ())

			/** Tworzy wewnętrzny identyfikator typu SlLElement */
		SlType			sl_lelement_type ();

			/**
			 * Zwraca nazwę elementu.
			 *
			 * Zwraca łańcuch znaków zawierający nazwę elementu. Pamięć przydzielona
			 * na potrzeby łańcucha jest zarządzana przez obiekt.
			 *
			 * @param element		Element
			 * @return				Nazwa elementu
			 */
		SlString			sl_lelement_get_id (SlPointer element);

			/**
			 * Zwraca macierz konduktancji.
			 *
			 * Zwraca szablon macierzy konduktancji dla elementu.
			 *
			 * @param element		Element
			 * @return				Szablon macierzy konduktancji
			 */
		SlMatrix			sl_lelement_get_g (SlPointer element);

			/**
			 * Zwraca macierz prądów.
			 *
			 * Zwraca szablon macierzy prądów dla elementu.
			 *
			 * @param element		Element
			 * @return				Szablon macierzy prądów
			 */
		SlMatrix			sl_lelement_get_i (SlPointer element);

		/** Rezystor */
	typedef struct _SlResistor *SlResistor;

	/** Wewnętrzny identyfikator typu SlResistor */
#define SL_RESISTOR_TYPE (sl_resistor_type ())

			/** Tworzy wewnętrzny identyfikator typu SlResistor */
		SlType			sl_resistor_type ();

			/**
			 * Tworzy rezystor.
			 *
			 * Tworzy rezystor o oporze @c resistance. W przypadku rezystora, ze względu
			 * na jego symetrię, nie rozróżnia się elektrod.
			 *
			 * @param id				Nazwa elementu
			 * @param resistance		Opór elektryczny w ohmach
			 * @return					Nowo utworzony rezystor
			 */
		SlResistor		sl_resistor_new (SlString id, SlFloat resistance);

			/**
			 * Zwraca opór rezystora
			 *
			 * Zwraca wartość oporu elektrycznego rezystora @c resistor. W przypadku
			 * błędu zwraca @c 0.
			 *
			 * @param resistor		Rezystor
			 * @return					Wartość oporu w ohmach
			 */
		SlFloat			sl_resistor_get_resistance (SlPointer resistor);

			/**
			 * Ustawia opór rezystora
			 *
			 * Ustawia wartość oporu elektrycznego rezystora @c resistor na @c resistance.
			 *
			 * @param resistor		Rezystor
			 * @param resistance		Opór elektryczny w ohmach
			 */
		void				sl_resistor_set_resistance (SlPointer resistor, SlFloat resistance);


		/** Siła elektromotoryczna */
	typedef struct _SlVSource *SlVSource;

	/** Wewnętrzny identyfikator typu SlVSource */
#define SL_VSOURCE_TYPE (sl_vsource_type ())

			/** Tworzy wewnętrzny identyfikator typu SlVSource */
		SlType			sl_vsource_type ();

			/**
			 * Tworzy nową siłę elektromotoryczną.
			 *
			 * Tworzy SEM o napięcu @c voltage, gdzie @c voltage to U<sub>n<sup>+</sup>n<sup>-</sup></sub>.
			 *
			 * @param id			Nazwa elementu
			 * @param voltage		Napięcie siły w woltach
			 * @return				Nowo utworzona siła elektromotoryczna
			 */
		SlVSource		sl_vsource_new (SlString id, SlFloat voltage);

			/**
			 * Zwraca wartość siły elektromotorycznej
			 *
			 * Zwraca napięcie siły elektromotorycznej @c source lub @c 0 w przypadku
			 * błędu.
			 *
			 * @param vsource		Siła elektromotoryczna
			 * @return				Napięcie siły w woltach
			 */
		SlFloat			sl_vsource_get_voltage (SlPointer vsource);

			/**
			 * Ustawia wartość siły elektromotorycznej
			 *
			 * Ustawia napięcie siły elektromotorycznej @c vsource na @c voltage.
			 *
			 * @param vsource		Siła elektromotoryczna
			 * @param voltage		Napięcie w woltach
			 */
		void				sl_vsource_set_voltage (SlPointer vsource, SlFloat voltage);


		/** Siły prądomotoryczna */
	typedef struct _SlCSource *SlCSource;

	/** Wewnętrzny identyfikator typu SlCSource */
#define SL_CSOURCE_TYPE (sl_csource_type ())

			/** Tworzy wewnętrzny identyfikator typu SlCSource */
		SlType			sl_csource_type ();

			/**
			 * Tworzy nową siłę prądomotoryczną.
			 *
			 * Tworzy SPM o prądzie @c current. Kierunek wymuszenia z elektrody "dodatniej"
			 * do "ujemnej" (przy dodatniej wartości prądu).
			 *
			 * @param id			Nazwa siły
			 * @param current		Wartość siły w amperach
			 * @return				Nowo utworzona siła prądomotoryczna
			 */
		SlCSource		sl_csource_new (SlString id, SlFloat current);

			/**
			 * Zwraca wartość siły prądomotorycznej
			 *
			 * Zwraca wartość siły prądomotorycznej @c csource lub @c 0 w przypadku błędu.
			 *
			 * @param csource		Siła prądomotoryczna
			 * @return				Prąd w amperach
			 */
		SlFloat			sl_csource_get_current (SlPointer csource);

			/**
			 * Ustawia wartość siły prądomotorycznej
			 *
			 * Ustawia wartość siły prądomotorycznej @c csource na @c current.
			 *
			 * @param csource		Siła prądomotoryczna
			 * @param current		Prąd w amperach
			 */
		void				sl_csource_set_current (SlPointer csource, SlFloat current);

#endif
