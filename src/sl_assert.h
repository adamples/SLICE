#ifndef _SL_ASSERT_H_
#define _SL_ASSERT_H_

	/**
	 * @file
	 *
	 * Niestandardowe asercje biblioteki
	 *
	 * Plik zawiera definicje asercji używanych przez funkcje biblioteczne.
	 * W przeciwieństwie do funkcji assert z biblioteki standardowej
	 * zdefiniowane tutaj makra nie zatrzymują wykonywania programu
	 * w przypadku niepowodzenia oraz zwracają wartość logiczną. Mogą
	 * być używane w ten sam sposób, jednak intencją jest następująca
	 * konstrukcja:
	 *
	 * @code
	 * if (sl_assert (warunek)) {
	 * 	// Kod zależny od warunku
	 * }
	 * @endcode
	 *
	 * W zależności zdefiniowanych stałych preprocesora wywoła to następujące
	 * skutki:
	 * 	- zdeifniowane NDEBUG i NCHECK - do kodu wstawiona zostanie
	 * 		jedynka, co jest równoznaczne z pominięciem całej konstrukcji
	 * 	- zdefiniowane NDEBUG - do kodu wstawiony zostanie warunek
	 * 		w niezmienionej formie
	 * 	- brak w.w. stałych - w przypadku gdy warunek zostanie obliczony
	 * 		jako 0, na ekranie zostanie wyświetlony komunikat błędu
	 * 		za pomocą standardowej funkcji printf; zwrócona zostaje
	 * 		wartość logiczna odpowiadająca warunkowi
	 */


#ifdef NDEBUG
	#ifdef NCHECK
		#define sl_assert(a) ( 1 )
	#else
		#define sl_assert(a) ( a )
	#endif
#else
	#include <stdio.h>
	#define sl_assert(a) ( (a) ? (1) : ( printf ("%s:%d: %s(): assertion failed (%s).\n", __FILE__, __LINE__, __FUNCTION__, #a), 0) )
#endif


#endif