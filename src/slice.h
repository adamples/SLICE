#ifndef _SLICE_H_
#define _SLICE_H_

	/* NULL i size_t się zdarzają w nagłówkach */
#include <stdlib.h>

#include "sl_assert.h"
#include "sl_types.h"
#include "sl_table.h"
#include "sl_table_2d.h"
#include "sl_map.h"
#include "sl_map_si.h"
#include "sl_matrix.h"
#include "sl_lelement.h"
#include "sl_nlelement.h"
#include "sl_circuit.h"

	/**
	 * @file
	 *
	 * Główny plik nagłówkowy biblioteki slice
	 *
	 * Programy używające biblioteki slice powinny używać wyłącznie ninejszego nagłówka.
	 */

	/**
	 * @mainpage libSLICE Reference Manual
	 *
	 * @section Temat
	 *
	 * Tematem zadania było napisanie biblioteki do analizy stałoprądowej obwodów
	 * elektrycznych i elektronicznych. Biblioteka miała zapewnić możliwość obliczania
	 * napięć i prądów na konkretnych elementach.
	 *
	 * @section Analiza
	 *
	 * Poszukiwanie punktu pracy obwodu zrealizowano za pomocą metody potencjałów
	 * węzłowych. Metoda polega na rozwiązaniu układu równań, w tym przypadku metodą
	 * macierzową. Równanie ma postać: G * V = I, gdzie G to macierz konduktacji
	 * między węzłami obwodu, V — macierz potencjałów węzłów (szukana), I — macierz
	 * zastępczych sił prądomotorycznych.
	 *
	 * Metoda potencjałów węzłowych może być zastosowana wyłącznie w obwodach liniowych.
	 * Jeżeli w obwodzie znajdują się elementy nieliniowe, obwód jest sprowadzany do
	 * liniowego metodą linearyzacji odcinkowej elementów.
	 *
	 * Program zrealizowano w paradygmacie obiektowym, korzystając z prostego modelu
	 * opisanego w dokumentacji pliku sl_types.h. Poniższy rysunek prezentuje hierarchię
	 * zdefiniowanych typów obiektowych:
	 *
	 * @image html "hierarchia.png"
	 *
	 * W typowym zastosowaniu konieczne jest użycie wyłącznie typów: @ref SlCircuit,
	 * @ref SlResistor, @ref SlVsource, @ref SlCSource oraz @ref SlDiode. Przykładowy
	 * program znajduje się w katalogu "example".
	 *
	 * @section Kompilacja
	 *
	 * Cały kod biblioteki jest zgodny ze standardem ANSI C i może być skompilowany
	 * dowolnym kompilatorem wspierającym ten standard.
	 *
	 * Podczas pisania biblioteki i testów do kompilacji używane były następujące
	 * narzędzia:
	 * 	- kompilator gcc v4.4.5,
	 * 	- GNU make v3.81,
	 * 	- GNU binutils v2.20.1,
	 * 	- GNU autotools (automake v1.11.1, autoconf v2.67).
	 *
	 * W celu kompilacji za pomocą GNU make konieczne jest środowisko zgodne z POSIX.
	 * Typowy przebieg kompilacji:
	 *
	 * @code
	 * ./configure --prefix=/usr
	 * make
	 * make install (as superuser)
	 * @endcode
	 *
	 * Aby wygenerować dokumentację należy posłużyć się poleceniem:
	 *
	 * @code
	 * make doxygen-doc
	 * @endcode
	 *
	 * @section Testowanie
	 *
	 * W celu przetestowania działania biblioteki utworzono przykładowy program
	 * (example/example.c), pobierający na wejściu prostą netlistę w postaci pliku
	 * tekstowego. Przykładowa netlista:
	 *
	 * @code
	 * R1 1 0 1000
	 * D1 0 1 0.7 7.2 1e-6
	 * V1 2 0 DC 20
	 * I1 2 0 DC 1u
	 * @endcode
	 *
	 * W każdym wierszu netlisty znajduje się opis opjedynczego elementu. Opis składa
	 * się z nazwy elementu oraz nazw węzłów, między które element będzie włączony.
	 * Następnie występują parametry elementu.
	 * Pierwsza litera nazwy identyfikuje rodzaj elementu. Powyżej mamy kolejno: rezystor,
	 * diodę, SEM i SPM.
	 *
	 * Dla rezystora parametrem jest jego opór w ohmach. Dla źródeł — rodzaj i wartość wymuszenia.
	 * Jedynym rozpoznawanym rodzajem jest DC (stałe wymuszenie niezależne).
	 * W przypadku diody są to: napięcie przewodzenia, napięcie przebicia (wartość dodatnia)
	 * oraz prąd wsteczny.
	 *
	 * Testy wykazały przydatność narzędzia do rozwiązywania zarówno prostych, jak
	 * i bardziej skomplikowanych obwodów.
	 *
	 * Drugim programem testowym jest example/memtest.c, który ma służyć wykrywaniu
	 * wycieków pamięci. Tworzy on wielokrotnie złożone obwody, a następnie usuwa
	 * je z pamięci. Testy za pomocą programu pozwoliły usunąć z biblioteki wszelkie
	 * wycieki.
	 *
	 * @section Wnioski
	 *
	 * 	- Stworzony program należy uznać za proste, acz użyteczne narzędzie pomagające
	 * 	  w analizie obwodów. Udało się zaimplementować jego najważniejsze funkcje,
	 * 	  jednak kilka nie znalazło się w bibliotece (elementy: warystor, dioda tunelowa)
	 * 	  oraz w przykładowym programie (analiza krokowa).
	 * 	- Zastosowane metody okazały się adekwatne do zadania i pozwoliły je zrealizować
	 * 	  na przyzwoitym poziomie.
	 * 	- Podczas pisania programu przeanalizowano wiele modeli obiektowych, zarówno
	 * 	  natywnych modeli języków programowania, jak i implementacji w językach
	 * 	  nieobiektowych. Pozwoliło to poznać sposoby realizacji obiektowości i z
	 * 	  pewnością pomoże w lepszym zrozumieniu programowania obiektowego w C++.
	 *
	 * @section Bibliografia
	 */


#endif