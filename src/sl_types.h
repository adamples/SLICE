#ifndef _SL_TYPES_H_
#define _SL_TYPES_H_

#include "slice.h"


	/**
	 * @file
	 * Podstawowy system typów
	 *
	 * Nagłówek zawiera definicje typów prostych (@ref SlFloat, @ref SlString, @ref SlBool)
	 * używanych przez bibliotekę, jak również bazowy typ obiektowy @ref SlObject.
	 *
	 * Biblioteka używa własnego prostego systemu obiektowego, którego głównymi zadaniami
	 * są uzyskanie enkapsulacji oraz dynamicznego poliformizmu. Niektóre pojęcia
	 * używane w jego kontekście mogą mieć nieco inne znaczenie niż ogólnie przyjęte.
	 *
	 * Enkapsulacja polega na ukryciu definicji struktur danych oraz metod prywatnych
	 * w oddzielnych nagłówkach, a udostępnieniu jedynie typów wskaźnikowych związanych
	 * z konkretnymi obiektami. Nazwy wewnętrznych nagłówków posiadają sufiks "_internal".
	 * Powinny być włączane wyłącznie w innych prywatnych nagłówkach, za pomocą
	 * <tt>#</tt><tt>include "slice_internal.h"</tt>.
	 *
	 * Każda instancja typu obiektowego jest wskaźnikiem na strukturę. Struktura ta
	 * zawsze jako pierwsze pole zawiera strukturę typu rodzica. Przodkiem każdego
	 * obiektu musi być SlObject. W nagłówku prywatnym powinna więc znaleźć się np. taka
	 * definicja:
	 *
	 * @code
	 * 	typedef struct _SlFoo {
	 * 		SlSObject	parent;			// Struktura typu obiektowego rodzica
	 * 		int			int_prop;		// Właściwości
	 * 		SlFloat		float_prop;
	 * 	} SlSFoo;
	 * @endcode
	 *
	 * Prefiks "SlS" zawsze oznacza alias dla definicji struktury; definicja <tt>struct _Sl*</tt>
	 * natomiast powinna być użyta w nagłówku publicznym do zdefiniowania właściwego
	 * typu obiektu:
	 *
	 * @code
	 * 	typedef struct _SlFoo		*SlFoo;
	 * @endcode
	 *
	 * Typ bazowy @c SlObject zawiera wskaźnik na strukturę wspólną dla wszystkich
	 * obiektów danego typu, nazywaną w tym przypadku <em>klasą</em>. Klasa zawiera
	 * tablicę metod wirtualnych oraz macierz identyfikatorów typów, do których należy.
	 * Struktury klas potomnych tworzy się w podobny sposób, jak struktury obiektów:
	 *
	 * @code
	 * 	typedef struct _SlFooClass {		// _SlFooClass tylko dla kompatybilności
	 * 		SlSObjectClass		parent;		// Struktura klasy rodzica
	 * 		void (*virtual_method) (SlPointer foo);
	 * 	} SlSFooClass, *SlFooClass;
	 * @endcode
	 *
	 * Klasa bazowa @c SlObjectClass zawiera wspomnianą wcześniej macierz identyfikatorów
	 * typów, której konstrukcja nie jest istotna, gdyż dostęp odbywa się wyłącznie
	 * za pomocą odpowiednich funkcji.
	 *
	 * Klasy są tworzone dynamicznie, razem z pierwszym obiektem danego typu. Podobnie
	 * wewnętrzne identyfikatory typów.
	 *
	 * Aby utworzyć nowy typ obiektowy, w nagłówku publicznym trzeba umieścić jeszcze
	 * następujące deklaracje:
	 *
	 * @code
	 * 	// Wewnętrzny identyfikator typu SlFoo
	 * 	// Makro to ma imitować zwykłą stałą preprocesora, ukrywając sposób
	 * 	// tworzenia identyfikatora
	 * #define SL_FOO_TYPE (sl_foo_type ())
	 *  
	 * 		// Funkcja dynamicznie tworząca wewnętrzny identyfikator typu
	 * 	SlType		sl_foo_type (void);
	 * 		// Funkcja tworząca nowy obiekt
	 * 	SlFoo			sl_foo_new (int p1, SlFloat p2);
	 * @endcode
	 *
	 * Tworzenie identyfikatora typu przebiega zawsze tak samo:
	 *
	 * @code
	 * 	SlType
	 * 	sl_foo_type () {
	 * 		static SlType		type = SL_TYPE_UNDEFINED;
	 *  
	 * 			if (type == SL_TYPE_UNDEFINED)
	 * 				type = sl_type_new ();
	 * 		return type;
	 * 	}
	 * @endcode
	 *
	 * Funkcja @c sl_foo_new ma za zadanie zalokować pamięć dla obiektu, ustawić
	 * jego klasę oraz wywołać konstruktor -- @c sl_foo_init:
	 *
	 * @code
	 * 	void
	 * 	sl_foo_init (SlPointer foo, int p1, SlFloat p2) {
	 * 			// Konstruktor może być wywołany na obiektu typu potomnego, więc tu
	 * 			// również trzeba sprawdzić parametry
	 * 		if (sl_assert (p1 > 0 && p2 != 0)) {
	 * 				// Tworzymy kontekst (zmienną this)
	 * 			sl_create_context (SlFoo, diode);
	 *  
	 * 				// Wywołujemy konstruktor rodzica
	 * 			sl_object_init (this);
	 * 				// Robimy wszystkie inne rzeczy
	 * 			this->int_prop = p1;
	 * 			this->float_prop = p2;
	 * 		}
	 * 	}
	 *  
	 * 	SlFoo
	 * 	sl_foo_new (int p1, SlFloat p2) {
	 * 		SlFoo		result = NULL;
	 *  
	 * 				// Sprawdzamy parametry; w przypadku niepowodzenia zwracamy NULL
	 * 			if (sl_assert (p1 > 0 && p2 != 0)) {
	 * 					// Alokujemy pamięć -- sizeof zawsze z SlS!
	 * 				result = (SlFoo) malloc (sizeof (SlSFoo));
	 * 					// Ustawiamy klasę za pomocą metody typu SlObject
	 * 					// Struktura klasy jest tworzona w tym momencie
	 * 				sl_object_set_class (result, SL_FOO_CLASS);
	 * 					// Wywołujemy konstruktor
	 * 				sl_foo_init (result, p1, p2);
	 * 			}
	 * 			// Zwracamy nowo utworzony obiekt
	 * 		return result;
	 * 	}
	 * @endcode
	 *
	 * Tworzenie klasy przebiega w podobny sposób -- najpierw alokowana jest pamięć,
	 * a następnie klasa jest inicjowana:
	 *
	 * @code
	 * 	void
	 * 	sl_foo_class_init (SlPointer class) {
	 * 			// W razie problemów z pamięcią...
	 * 		if (sl_assert (class)) {
	 * 				// Tworzenie kontekstu jest dosyć uniwersalne
	 * 			sl_create_context (SlFooClass, class);
	 *  
	 * 				// Dziedziczymy po klasie rodzica
	 * 			sl_object_class_init (this);
	 * 				// Dodajemy SL_FOO_TYPE do macierzy identyfikatorów typów; identyfikator
	 * 				// typu jest tworzony w tym momencie
	 * 			sl_object_class_add_type (this, SL_FOO_TYPE);
	 * 				// Ustawiamy wskaźniki na metody wirtualne
	 * 			((SlObjectClass) this)->free = sl_foo_free_virtual;
	 * 			this->virtual_method = sl_foo_virtual_method_virtual;
	 * 		}
	 * 	}
	 *  
	 * 	SlFooClass
	 * 	sl_foo_class (void) {
	 * 		static SlFooClass		class = NULL;
	 *  
	 * 			if (class == NULL) {
	 * 				class = (SlFooClass) malloc (sizeof (SlSFooClass));
	 * 					// Inicjujemy nowo utworzoną klasę
	 * 				sl_foo_class_init (class);
	 * 			}
	 * 		return class;
	 * 	}
	 * @endcode
	 *
	 * Dodatkowo w nagłówku wewnętrznym znajduje się makro, ukrywające przed użytkownikiem
	 * fakt, że klasy są tworzone dynamicznie:
	 *
	 * @code
	 * #define SL_FOO_CLASS		(sl_foo_class ())
	 * @endcode
	 *
	 * Metoda wirtualna składa się z dwóch funkcji: publicznej, np. @c sl_foo_virtual_method,
	 * oraz prywatnej, z sufiksem "_virtual" (w tym przypadku @c sl_foo_virtual_method_virtual).
	 * Ta pierwsza ma za zadanie pobrać odpowiedni wskaźnik z tablicy metod wirtualnych
	 * w klasie, a następnie wywołać drugą, zawierającą właściwą treść:
	 *
	 * @code
	 * 		// Metoda free została zadeklarowana jako wirtualna w SlObject
	 * 	void
	 * 	sl_foo_free_virtual (SlPointer foo) {
	 * 			// Sprawdzamy, czy nie podano obiektu niewłaściwego typu
	 * 		if (sl_assert (sl_object_is (foo, SL_TYPE_FOO))) {
	 * 			sl_create_context (SlFoo, foo);
	 *  
	 * 				// Dziedziczymy po SlObject
	 * 			sl_object_free_virtual (this);
	 * 				// Niestety w tym przypadku nie mamy nic więcej do zrobienia
	 * 		}
	 * 	}
	 *  
	 * 	void
	 * 	sl_foo_virtual_method_virtual (SlPointer foo) {
	 * 		if (sl_assert (sl_object_is (foo, SL_TYPE_FOO))) {
	 * 			sl_create_context (SlFoo, foo);
	 *  
	 * 				// Robimy co trzeba
	 * 			this->int_prop++;
	 * 			this->float_prop = sin(this->int_prop);
	 * 		}
	 * 	}
	 *
	 * 	void
	 * 	sl_foo_virtual_method (SlPointer foo) {
	 * 			if (sl_assert (sl_object_is (foo, SL_TYPE_FOO)))
	 * 					// Na pozyskanie wskaźnika na klasę pozwala funkcja sl_object_get_class
	 * 				((SlFooClass) sl_object_get_class (foo))->virtual_method (foo);
	 * 	}
	 * @endcode
	 *
	 * W tym przypadku mogliśmy umieścić w tablicy metod wirtualnych bezpośrednie
	 * odniesienie do @c sl_object_free (a właściwie pozostawiając ten wskaźnik niezmieniony),
	 * uzyskując ten sam skutek. Utrudniłoby to jednak dziedziczenie, gdyż użytkownik
	 * chcąc utworzyć obiekt potomny musiałby sprawdzać za każdym razem, kiedy w
	 * hierarchii obiektów dana metoda została zdefiniowana.
	 *
	 * Jeżeli nie zostanie zdefiniowana treść metody (funkcja sl_*_virtual), otrzymamy
	 * abstarcyjny typ obiektowy. Aby zapobiec utworzeniu instancji takiego typu,
	 * należy zrezygnować z tworzenia funkcji @c sl_*_new, pozostawiając jedynie
	 * @c sl_*_init (tak jest np. w przypadku SlObject).
	 *
	 * Dostęp do właściwości jest realizowany za pomocą metod sl_obiekt_wlasciwosc_get/set.
	 * Brak odpowiedniej metody oznacza, że właściwość jest tylko do odczytu/tylko do zapisu.
	 */

		/** Podstawowy typ zmiennoprzecinkowy */
	typedef double SlFloat;

			/**
			 * Minimum liczb rzeczywistych
			 *
			 * Zwraca wartość mniejszej liczby spośród @c a i @c b.
			 *
			 * @param a		Liczba zmiennoprzecinkowa
			 * @param b		Takaż
			 * @return		Wartość minimalna argumentów
			 */
		SlFloat		sl_float_min (SlFloat a, SlFloat b);

			/**
			 * Maksimum liczb rzeczywistych
			 *
			 * Zwraca wartość większej liczby spośród @c a i @c b.
			 *
			 * @param a		Liczba zmiennoprzecinkowa
			 * @param b		Takaż
			 * @return		Wartość maksymalna argumentów
			 */
		SlFloat		sl_float_max (SlFloat a, SlFloat b);


		/**
		 * Typ logiczny
		 *
		 * Zdefiniowany dla kompatybilności z C89.
		 */
	typedef enum {
		false,
		true
	} SlBool;

		/**
		 * Wskaźnik amorficzny
		 *
		 * Wskaźnik jest używany w celu uzyskania dynamicznego polimorfizmu, bez
		 * konieczności częstego rzutowania typów podczas wywołań metod.
		 *
		 * Każda metoda jako pierwszy argument przyjmuje obiekt, będący podmiotem
		 * operacji. Użycie w deklaracji metody konkretnego typu spowodowałaby
		 * konieczność rzutowania przy każdej próbie wywołania na typie potomnym.
		 * Użycie @c SlPointer pozwala przekazać do funkcji dowolny obiekt, a kontrola
		 * typów odbywa się dynamicznie, za pomocą wewnętrznych mechanizmów modelu
		 * obiektowego (identyfikatorów typów).
		 */
	typedef void		*SlPointer;

		/**
		 * Łańcuch znaków
		 *
		 * Jest to alias dla typu @c char*. Łańcuchy są zakończone znakiem ASCII NULL.
		 * Zarządzanie pamięcią leży w gestii użyszko^Wużytkownika biblioteki.
		 */
	typedef char		*SlString;

			/**
			 * Klonuje łańcuch znaków.
			 *
			 * Alokuje pamięć na nowy łańcuch znaków, a następnie kopiuje do niej
			 * zawartość @c source i zwraca wskaźnik na nowo utworzony łańcuch.
			 *
			 * @param source		Klonowany łańcuch znaków
			 * @return				Wskaźnik na nowo utworzony łańcuch
			 */
		SlString		sl_string_clone (SlString source);

		/**
		 * Wewnętrzny identyfikator typu obiektowego
		 *
		 * Wewnętrzne identyfikatory typów wspomogają polimorfizm, pozwalając na
		 * dynamiczną kontrolę typów obiektów przekazywanych do metod. Identyfikator
		 * konkretnego typu można pozyskać korzystając z makr SL_nazwa_TYPE. Aby sprawdzić,
		 * czy dany obiekt należy do typu, należy skorzystać z metody @ref sl_object_is.
		 */
	typedef int			SlType;


	/**
	 * Identyfikator niezdefiniowanego typu
	 *
	 * Używany przy tworzeniu nowego typu. Zwykle nie ma konieczności bezpośredniego
	 * korzystania z tego makra.
	 */
#define SL_TYPE_UNDEFINED (-1)

	/**
	 * Makro tworzenia kontekstu
	 *
	 * Makro definiuje zmienną wskaźnikową @c this typu @c type i wskazującą
	 * na ten sam obszar pamięci co @c var. Przykładowa konstrukcja:
	 *
	 * @code
	 * 	function foo (SlPointer p) {
	 * 		sl_create_context (SlObject, p);
	 *
	 * 		this->bar = NULL;
	 * 	}
	 * @endcode
	 *
	 * zostanie rozwinięta do następującej postaci:
	 *
	 * @code
	 * 	function foo (SlPointer p) {
	 * 		SlObject		this = (SlObject) p;
	 *
	 * 		this->bar = NULL;
	 * 	}
	 * @endcode
	 *
	 * Dlatego też należy pamiętać, aby zawsze umieszczać makro na początku zakresu.
	 */
#define sl_create_context(type,var) type		this = (type) var;


		/** Typ bazowy dla wszystkich obiektów */
	typedef struct _SlObject *SlObject;

	/** Wewnętrzny identyfikator typu SlObject */
#define SL_OBJECT_TYPE (sl_object_type ())

			/** Tworzy wewnętrzny identyfikator typu SlObject */
		SlType				sl_object_type ();
			/** Uwalnia zasoby zalokowane przez obiekt SlObject */
		void					sl_object_free (SlPointer object);

	/**
	 * Niszczy obiekt
	 *
	 * Niszczy obiekt razem z należącymi do niego zasobami.
	 *
	 * @param object		Wskaźnik na niszczony obiekt, po wykonaniu zostaje ustawiony
	 * 						na NULL.
	 */
#define sl_object_dispose(object) (_sl_object_dispose((void **) object))
		void					_sl_object_dispose (SlPointer *object);

			/**
			 * Sprawdza typ obiektu
			 *
			 * Sprawdza, czy poadny obiekt jest typu @c type lub typu potomnego.
			 *
			 * @param object		Sprawdzany obiekt
			 * @param type			Wewnętrzny identyfikator typu
			 * @return				@c true, jeśli obiekt jest zgodny z podanym typem
			 */
		SlBool				sl_object_is (SlPointer object, SlType type);


#endif
