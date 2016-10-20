#include "slice_internal.h"

#include <stdlib.h>
#include <string.h>


	/* - Element liniowy ---------------------------------------------------- */

		void
		sl_lelement_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlLElementClass, class);

					sl_object_class_init (this);
					sl_object_class_add_type (this, SL_LELEMENT_TYPE);
					((SlObjectClass) this)->free = sl_lelement_free_virtual;
					this->get_g = sl_lelement_get_g_virtual;
					this->get_i = sl_lelement_get_i_virtual;
					this->get_current = sl_lelement_current_virtual;
				}
		}

		SlLElementClass
		sl_lelement_class () {
			static SlLElementClass		class = NULL;

				if (!class) {
					class = (SlLElementClass) malloc (sizeof (SlSLElementClass));
					sl_lelement_class_init (class);
				}
			return class;
		}


		SlType
		sl_lelement_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_lelement_init (SlPointer element, SlString id) {
				if (sl_assert (element)) {
					sl_create_context (SlLElement, element);

					sl_object_init (element);
					this->id = sl_string_clone (id);
					this->g = sl_matrix_new ();
					this->i = sl_matrix_new ();
				}
		}


		SlLElement
		sl_lelement_new (SlString id) {
			SlLElement		result;

			result = (SlLElement) malloc (sizeof (SlSLElement));
			sl_object_set_class (result, SL_LELEMENT_CLASS);
			sl_lelement_init (result, id);
			return result;
		}


		void
		sl_lelement_free_virtual (SlPointer element) {
				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE))) {
					sl_create_context (SlLElement, element);

					sl_object_free_virtual (this);
					sl_object_dispose (&this->g);
					sl_object_dispose (&this->i);
					free (this->id);
				}
		}


		SlMatrix
		sl_lelement_get_g (SlPointer element) {
				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE)))
					return ((SlLElementClass) sl_object_get_class (element))->get_g (element);
				else
					return NULL;
		}


		SlMatrix
		sl_lelement_get_g_virtual (SlPointer element) {
				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE))) {
					sl_create_context (SlLElement, element);

					return this->g;
				} else
					return NULL;
		}


		SlMatrix
		sl_lelement_get_i (SlPointer element) {
				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE)))
					return ((SlLElementClass) sl_object_get_class (element))->get_i (element);
				else
					return NULL;
		}


		SlMatrix
		sl_lelement_get_i_virtual (SlPointer element) {
				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE))) {
					sl_create_context (SlLElement, element);

					return this->i;
				} else
					return NULL;
		}


		SlFloat
		sl_lelement_current (SlPointer element, SlPointer v) {
				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE) && sl_object_is (v, SL_MATRIX_TYPE)))
					return ((SlLElementClass) sl_object_get_class (element))->get_current (element, v);
			return 0;
		}


		SlFloat
		sl_lelement_current_virtual (SlPointer element, SlPointer v) {
			return 0;
		}


		SlString
		sl_lelement_get_id (SlPointer element) {
			SlString		result = NULL;

				if (sl_assert (sl_object_is (element, SL_LELEMENT_TYPE))) {
					sl_create_context (SlLElement, element);

					result = this->id;
				}
			return result;
		}

	/* - Element liniowy koniec --------------------------------------------- */

	/* - Rezystor ----------------------------------------------------------- */

		void
		sl_resistor_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlResistorClass, class);

					sl_object_class_init (this);
					sl_lelement_class_init (this);
					sl_object_class_add_type (this, SL_RESISTOR_TYPE);
					((SlLElementClass) this)->get_current = sl_resistor_current_virtual;
				}
		}

		SlResistorClass
		sl_resistor_class () {
			static SlResistorClass		class = NULL;

				if (!class) {
					class = (SlResistorClass) malloc (sizeof (SlSResistorClass));
					sl_resistor_class_init (class);
				}
			return class;
		}


		SlType
		sl_resistor_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_resistor_init (SlPointer resistor, SlString id, SlFloat resistance) {

				if (sl_assert (resistor)) {
					sl_create_context (SlResistor, resistor);

					sl_lelement_init (this, id);
					sl_resistor_set_resistance (this, resistance);
				}
		}


		SlResistor
		sl_resistor_new (SlString id, SlFloat resistance) {
			SlResistor		result = NULL;

			result = (SlResistor) malloc (sizeof (SlSResistor));
			sl_object_set_class (result, SL_RESISTOR_CLASS);
			sl_resistor_init (result, id, resistance);
			return result;
		}


		SlFloat
		sl_resistor_current_virtual (SlPointer element, SlPointer v) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_RESISTOR_TYPE) && sl_object_is (v, SL_MATRIX_TYPE))) {
					sl_create_context (SlResistor, element);

					result = (sl_matrix_get (v, 1, 1) - sl_matrix_get (v, 2, 1)) / this->resistance;
				}
			return result;
		}


		SlFloat
		sl_resistor_get_resistance (SlPointer element) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_RESISTOR_TYPE))) {
					sl_create_context (SlResistor, element);

					result = this->resistance;
				}
			return result;
		}


		void
		sl_resistor_set_resistance (SlPointer element, SlFloat resistance) {
			SlMatrix		matrix;
			SlFloat		conductance;

				if (sl_assert (sl_object_is (element, SL_RESISTOR_TYPE))) {
					sl_create_context (SlResistor, element);

					this->resistance = resistance;
					conductance = 1 / this->resistance;
					matrix = sl_lelement_get_g (this);
					sl_matrix_resize (matrix, 2, 2);
					sl_matrix_set (matrix, 1, 1, conductance);
					sl_matrix_set (matrix, 2, 2, conductance);
					sl_matrix_set (matrix, 1, 2, -conductance);
					sl_matrix_set (matrix, 2, 1, -conductance);
					matrix = sl_lelement_get_i (this);
					sl_matrix_resize (matrix, 2, 1);
					sl_matrix_fill (matrix, 0);
				}
		}

	/* - Rezystor koniec ---------------------------------------------------- */

	/* - Wymuszenie napięciowe ---------------------------------------------- */

		SlType
		sl_vsource_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_vsource_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlVSourceClass, class);

					sl_lelement_class_init (this);
					sl_object_class_add_type (this, SL_VSOURCE_TYPE);
					((SlLElementClass) this)->get_current = sl_vsource_current_virtual;
				}
		}


		SlVSourceClass
		sl_vsource_class () {
			static SlVSourceClass		class = NULL;

				if (!class) {
					class = (SlVSourceClass) malloc (sizeof (SlSVSourceClass));
					sl_vsource_class_init (class);
				}
			return class;
		}


		void
		sl_vsource_init (SlPointer vsource, SlString id, SlFloat voltage) {
			SlMatrix		matrix;

				if (sl_assert (sl_object_is (vsource, SL_VSOURCE_TYPE))) {
					sl_create_context (SlVSource, vsource);

					sl_lelement_init (this, id);
					sl_vsource_set_voltage (this, voltage);
				}
		}


		SlVSource
		sl_vsource_new (SlString id, SlFloat voltage) {
			SlVSource		result = NULL;

			result = (SlVSource) malloc (sizeof (SlSVSource));
			sl_object_set_class (result, SL_VSOURCE_CLASS);
			sl_vsource_init (result, id, voltage);
			return result;
		}


		SlFloat
		sl_vsource_current_virtual (SlPointer element, SlPointer v) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_VSOURCE_TYPE) && sl_object_is (v, SL_MATRIX_TYPE))) {
					sl_create_context (SlVSource, element);

					result = sl_matrix_get (v, 3, 1);
				}
			return result;
		}


		SlFloat
		sl_vsource_get_voltage (SlPointer vsource) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (vsource, SL_VSOURCE_TYPE))) {
					sl_create_context (SlVSource, vsource);

					result = this->voltage;
				}
			return result;
		}


		void
		sl_vsource_set_voltage (SlPointer vsource, SlFloat voltage) {
			SlMatrix		matrix;

				if (sl_assert (sl_object_is (vsource, SL_VSOURCE_TYPE))) {
					sl_create_context (SlVSource, vsource);

					this->voltage = voltage;
					matrix = sl_lelement_get_g (this);
					sl_matrix_resize (matrix, 3, 3);
					sl_matrix_fill (matrix, 0);
					sl_matrix_set (matrix, 3, 1,  1.0);
					sl_matrix_set (matrix, 3, 2, -1.0);
					sl_matrix_set (matrix, 1, 3,  1.0);
					sl_matrix_set (matrix, 2, 3, -1.0);
					matrix = sl_lelement_get_i (this);
					sl_matrix_resize (matrix, 3, 1);
					sl_matrix_fill (matrix, 0);
					sl_matrix_set (matrix, 3, 1, voltage);
				}
		}

	/* - Wymuszenie napięciowe koniec --------------------------------------- */

	/* - Wymuszenie prądowe ------------------------------------------------- */

			/** Zwraca wewnętrzny identyfikator typu dla SPM */
		SlType
		sl_csource_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_csource_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlCSourceClass, class);

					sl_lelement_class_init (this);
					sl_object_class_add_type (this, SL_CSOURCE_TYPE);
					((SlLElementClass) this)->get_current = sl_csource_current_virtual;
				}
		}


		SlCSourceClass
		sl_csource_class () {
			static SlCSourceClass		class = NULL;

				if (!class) {
					class = (SlCSourceClass) malloc (sizeof (SlSCSourceClass));
					sl_csource_class_init (class);
				}
			return class;
		}


		void
		sl_csource_init (SlPointer csource, SlString id, SlFloat current) {
				if (sl_assert (sl_object_is (csource, SL_CSOURCE_TYPE))) {
					sl_create_context (SlCSource, csource);

					sl_lelement_init (this, id);
					sl_csource_set_current (this, current);
				}
		}


		SlCSource
		sl_csource_new (SlString id, SlFloat current) {
			SlCSource		result = NULL;

			result = (SlCSource) malloc (sizeof (SlSCSource));
			sl_object_set_class (result, SL_CSOURCE_CLASS);
			sl_csource_init (result, id, current);
			return result;
		}


		SlFloat
		sl_csource_current_virtual (SlPointer element, SlPointer v) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_CSOURCE_TYPE) && sl_object_is (v, SL_MATRIX_TYPE))) {
					sl_create_context (SlCSource, element);

					result = this->current;
				}
			return result;
		}

		SlFloat
		sl_csource_get_current (SlPointer csource) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (csource, SL_CSOURCE_TYPE))) {
					sl_create_context (SlCSource, csource);

					result = this->current;
				}
			return result;
		}


		void
		sl_csource_set_current (SlPointer csource, SlFloat current) {
			SlMatrix		matrix;

				if (sl_assert (sl_object_is (csource, SL_CSOURCE_TYPE))) {
					sl_create_context (SlCSource, csource);

					this->current = current;
					matrix = sl_lelement_get_g (this);
					sl_matrix_resize (matrix, 2, 2);
					sl_matrix_fill (matrix, 0);
					matrix = sl_lelement_get_i (this);
					sl_matrix_resize (matrix, 2, 1);
					sl_matrix_set (matrix, 1, 1, -current);
					sl_matrix_set (matrix, 2, 1, current);
				}
		}

	/* - Wymuszenie prądowe koniec ------------------------------------------ */
