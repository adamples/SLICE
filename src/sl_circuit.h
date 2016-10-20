#ifndef _SL_CIRCUIT_H_
#define _SL_CIRCUIT_H_

#include "slice.h"

	/**
	 * @file
	 *
	 * Obwód elektryczny
	 *
	 * @ref SlCircuit jest obiektem reprezentującym obwód elektryczny i pozwalającym
	 * wykonać jego analizę stałoprądową (poszukiwanie punktu pracy).
	 *
	 * Obwód składa się z elementów, których parametry są reprezentowane przez obiekty
	 * potomne @ref SlLElement. Elementy są identyfikowane za pomocą nazwy; nie jest
	 * możliwe dodanie do obwodu dwóch elementów o tej samej nazwie.
	 *
	 * Do umieszczania elementów w obwodzie służy metoda @ref sl_circuit_add_element.
	 * Po zdefiniowaniu obwodu jego poprawność można sprawdzić wywołując
	 * @ref sl_circuit_recalculate, ale nie jest to konieczne. Przy próbie pozyskania
	 * wartości napięcia (za pomocą @ref sl_circuit_voltage_by_nodes lub
	 * @ref sl_circuit_voltage_by_element) lub prądu (@ref sl_circuit_current_by_element)
	 * wszystkie wartości są w razie potrzeby automatycznie przeliczane.
	 *
	 * Elementy są wstawiane pomiędzy dwa węzły, które są identyfikowane przez
	 * ich nazwy. Prąd na elemencie jest zawsze strzałkowany z węzła "dodatniego" do
	 * "ujemnego", a napięcie w kierunku przeciwnym. Węzeł "dodatni" i "ujemny" to
	 * jedynie umowne nazwy, pozwalające ustalić sposób umieszczenia elementu w
	 * obwodzie. Każdy element posiada elektrodę "dodatnią" i "ujemną" (dokładne
	 * informacje na temat ich rozmieszczenia znajdują się w dokumentacji elementów).
	 * Węzeł połączony z elektrodą "dodatnią" jest nazywany "dodatnim", analogicznie
	 * węzeł połączony z elektrodą "ujemną".
	 */

		/**
		 * Obwód elektryczny
		 */
	typedef struct _SlCircuit *SlCircuit;

	/** Wewnętrzny identyfikator typu obwodu */
#define SL_CIRCUIT_TYPE (sl_circuit_type ())

			/** Zwraca identyfikator typu obwodu */
		SlType			sl_circuit_type (void);

			/**
			 * Tworzy obiekt obwodu.
			 *
			 * Tworzy nowy obiekt reprezentujący pusty obwód elektryczny.
			 *
			 * @return		Nowo utworzony obwód.
			 */
		SlCircuit		sl_circuit_new (void);

			/**
			 * Dodaje element do obwodu.
			 *
			 * Wstawia element do obwodu pomiędzy węzłami @c p_node oraz @c n_node.
			 * Prąd jest strzałkowany zawsze z @c p_node do @c n_node, a napięcie
			 * przeciwnie do prądu. Szczegółowe informacje na temat konkretnych
			 * elementów znajdują się w ich dokumentacji.
			 *
			 * W przypadku próby wstawienia do obwodu dwa razy tego samego elementu
			 * lub podania błędnych argumentów (którykolwiek równy @c NULL lub złego
			 * typu) żadne działanie na obwodzie nie zostanie wykonane.
			 *
			 * @param circuit		Obwód, do którego zostanie wstawiony element.
			 * @param element		Wstawiany element.
			 * @param p_node		Nazwa węzła "dodatniego".
			 * @param n_node		Nazwa węzła "ujemnego".
			 */
		void				sl_circuit_add_element (SlPointer circuit, SlPointer element, SlString p_node, SlString n_node);

			/**
			 * Poszukuje punktu pracy obwodu.
			 *
			 * Znajduje punkt pracy obwodu i zwraca @c true w przypadku powodzenia
			 * lub @c false, gdy punkt pracy nie może być ustalony (np. w wyniku
			 * sprzeczności w budowie obwodu).
			 *
			 * @param circuit		Obwód.
			 * @return				@c true, jeśli punkt pracy został znaleziony.
			 */
		SlBool			sl_circuit_recalculate (SlPointer circuit);

			/**
			 * Zwraca napięcie między dwoma węzłami.
			 *
			 * Zwraca napięcie między węzłem @c p_node, a @c n_node. W przypadku braku
			 * rozwiązania obwodu zwrócone zostaje @c 0. Podobnie w sytuacji, gdy nie
			 * zostaną znalezione węzły o podanych nazwach.
			 *
			 * @param circuit		Obwód.
			 * @param p_node		Nazwa węzła "dodatniego".
			 * @param n_node		Nazwa węzła "ujemnego".
			 * @return				Napięcie między węzłami lub @c 0 w przypadku braku
			 * 						rozwiązania obwodu.
			 */
		SlFloat			sl_circuit_voltage_by_nodes (SlPointer circuit, SlString p_node, SlString n_node);

			/**
			 * Zwraca napięcie na elemencie.
			 *
			 * Zwraca napięcie elemecie o podanej nazwie. Jeżeli wcześniej
			 * nie została wywołana funkcja @ref sl_circuit_recalculate, zostaje wywołana
			 * wewnątrz niniejszej i w przypadku braku rozwiązania obwodu zwrócone
			 * zostaje @c 0. Podobnie, jeśli nie zostanie znaleziony element o podanej
			 * nazwie.
			 *
			 * @param circuit			Obwód.
			 * @param element_id		Nazwa elementu.
			 * @return					Napięcie na elemencie lub @c 0 w przypadku błędu.
			 */
		SlFloat			sl_circuit_voltage_by_element (SlPointer circuit, SlString element_id);

			/**
			 * Zwraca prąd na elemencie.
			 *
			 * Zwraca prąd płynący przez element o podanej nazwie. W przypadku braku
			 * rozwiązania obwodu zwrócone zostaje @c 0; podobnie, jeśli nie zostanie
			 * znaleziony element o podanej nazwie.
			 *
			 * @param circuit			Obwód.
			 * @param element_id		Nazwa elementu.
			 * @return					Prąd płynący przez element lub @c 0 w przypadku błędu.
			 */
		SlFloat			sl_circuit_current_by_element (SlPointer circuit, SlString element_id);

			/**
			 * Zwraca liczbę elementów w obwodzie.
			 *
			 * Zwraca liczbę elementów znajdujących się w obwodzie.
			 *
			 * @param circuit		Obwód.
			 * @return				Liczba elementów w obwodzie.
			 */
		int				sl_circuit_elements_number (SlPointer circuit);

			/**
			 * Zwraca nazwę n-tego elementu.
			 *
			 * Pobiera nazwę n-tego elementu wstawionego do obwodu. Elementy
			 * liczone są od zera.
			 *
			 * @param circuit		Obwód.
			 * @param n				Numer elementu, którego nazwa ma zostać pobrana.
			 * @return				Nazwa elementu lub NULL w przypadku błędu.
			 */
		SlString			sl_circuit_get_element_name (SlPointer circuit, int n);

			/**
			 * Sprawdza, czy element jest częścią obwodu
			 *
			 * Sprawdza, czy element o podanej nazwie został już wcześniej wstawiony
			 * do obwodu.
			 *
			 * @param circuit			Obwód.
			 * @param element_id		Nazwa elementu.
			 * @return					@c true, jeśli element znajduje się w obwodzie.
			 */
		SlBool			sl_circuit_element_exists (SlPointer circuit, SlString element_id);

#endif