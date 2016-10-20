#ifndef _SL_NLELEMENT_H_
#define _SL_NLELEMENT_H_

#include "slice.h"


		/**
		 * @file
		 *
		 * Elementy nieliniowe
		 *
		 * Element nieliniowy jest pewnym rozwinięciem idei elementu liniowego.
		 */

		/** Element nieliniowy */
	typedef struct _SlNLElement *SlNLElement;

	/** Wewnętrzny identyfikator typu SlNLElement */
#define SL_NLELEMENT_TYPE (sl_nlelement_type ())

			/** Tworzy identyfikator typu SlNLElement */
		SlType			sl_nlelement_type (void);

			/**
			 * Tworzy element nieliniowy.
			 *
			 * Tworzy element nieliniowy o nazwie @c id.
			 *
			 * @param id		Nazwa tworzonego elementu.
			 * @return			Nowo utworzony element.
			 */
		SlNLElement		sl_nlelement_new (SlString id);


			/**
			 * Liczba odcinków charakterystyki elmentu.
			 *
			 * Zwraca liczbę odcinków charakterystyki elmentu.
			 *
			 * @param element		Element nieliniowy.
			 * @return				Liczba odcinków charakterystyki elementu.
			 */
		int			sl_nlelement_segments_number (SlPointer element);

			/**
			 * Ustawia aktywny odcinek.
			 *
			 * Ustawia n-ty odcinek charakterystyki jako aktywny. Od tej chwili
			 * element przyjmuje charakterystykę elementu liniowego opisanego
			 * przez ten odcinek. Odcinki są numerowane od zera.
			 *
			 * @param element		Element nieliniowy.
			 * @param n				Numer odcinka.
			 */
		void			sl_nlelement_set_segment (SlPointer element, int segment);

			/**
			 * Pobiera aktywny odcinek.
			 *
			 * Zwraca numer aktywnego odcinka charakterystyki elementu nieliniowego.
			 *
			 * @param element		Element nieliniowy.
			 * @return				Numer aktywnego odcinka charakterystyki elementu.
			 */
		int			sl_nlelement_get_segment (SlPointer element);

			/**
			 * Minimalne napięcie w odcinku.
			 *
			 * Pobiera minimalne poprawne napięcie w aktywnym odcinku charakterystyki.
			 *
			 * @param element		Element nieliniowy.
			 * @return				Minimalne poprawne napięcie w aktywnym odcinku.
			 */
		SlFloat		sl_nlelement_get_segment_vmin (SlPointer element);

			/**
			 * Maksymalne napięcie w odcinku.
			 *
			 * Pobiera maksymalne poprawne napięcie w aktywnym odcinku charakterystyki.
			 *
			 * @param element		Element nieliniowy.
			 * @return				Maksymalne poprawne napięcie w aktywnym odcinku.
			 */
		SlFloat		sl_nlelement_get_segment_vmax (SlPointer element);

			/**
			 * Minimalny prąd w odcinku.
			 *
			 * Pobiera minimalny poprawny prąd w aktywnym odcinku charakterystyki.
			 *
			 * @param element		Element nieliniowy.
			 * @return				Minimalny poprawny prąd w aktywnym odcinku.
			 */
		SlFloat		sl_nlelement_get_segment_imin (SlPointer element);

			/**
			 * Maksymalny prąd w odcinku.
			 *
			 * Pobiera maksymalny poprawny prąd w aktywnym odcinku charakterystyki.
			 *
			 * @param element		Element nieliniowy.
			 * @return				Maksymalny poprawny prąd w aktywnym odcinku.
			 */
		SlFloat		sl_nlelement_get_segment_imax (SlPointer element);

		/** Dioda */
	typedef struct _SlDiode *SlDiode;

	/** Wewnętrzny identyfikator typu SlDiode */
#define SL_DIODE_TYPE (sl_diode_type ())

			/** Tworzy identyfikator typu SlDiode */
		SlType		sl_diode_type (void);

			/**
			 * Tworzy diodę.
			 *
			 * Tworzy nowy obiekt diody o napięciu w kierunku przewodzenia @c Vd,
			 * napięciu przebicia @c Vbr i prądzie wstecznym @c Ir. Wszystkie
			 * wartości muszą być nieujemne, a napięcie przebicia dodatkowo
			 * niezerowe.
			 *
			 * @param id		Nazwa elementu.
			 * @param Vd		Napięcie w kierunku przewodzenia.
			 * @param Vbr		Napięcie przebicia.
			 * @param Ir		Prąd wsteczny.
			 * @return			Nowo utworzona dioda o podanej charakterystyce.
			 */
		SlDiode		sl_diode_new (SlString id, SlFloat Vd, SlFloat Vbr, SlFloat Ir);

#endif