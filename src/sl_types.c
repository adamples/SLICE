#include "slice_internal.h"

#include <stdlib.h>
#include <string.h>


		SlFloat
		sl_float_min (SlFloat a, SlFloat b) {
				if (a <= b)
					return a;
				else
					return b;
		}


		SlFloat
		sl_float_max (SlFloat a, SlFloat b) {
				if (a >= b)
					return a;
				else
					return b;
		}


		SlString
		sl_string_clone (SlString source) {
			SlString		result = NULL;
			size_t		l;

				if (sl_assert (source)) {
					l = strlen (source);
					result = (SlString) malloc (l + 1);
						if (sl_assert (result)) { /* W razie kłopotów (z długością) */
							memcpy (result, source, l);
							result [l] = 0;
						}
				}
			return result;
		}


	/* - Obiekty ------------------------------------------------------------ */

		SlType
		sl_type_new () {
			static SlType		sl_types_number = 0;
			SlType				result = sl_types_number;

				if (sl_assert (sl_types_number <= SL_MAX_TYPES)) {
					sl_types_number++;
				} else {
					exit (1);
				}
			return result;
		}


		void
		sl_object_class_add_type (SlPointer class, SlType type) {
				if (sl_assert (class && type >= 0)) {
					sl_create_context (SlObjectClass, class);

					this->type_tab [type / SL_INT_BITS] |= 1 << (type % SL_INT_BITS);
				}
		}


		void
		sl_object_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlObjectClass, class);

					memset (this->type_tab, 0, SL_TYPE_TAB_SIZE * sizeof (unsigned int));
					sl_object_class_add_type (this, SL_OBJECT_TYPE);
					this->free = sl_object_free_virtual;
				}
		}


		SlObjectClass
		sl_object_class () {
			static SlObjectClass		class = NULL;

				if (!class) {
					class = (SlObjectClass) malloc (sizeof (SlSObjectClass));
					sl_object_class_init (class);
				}
			return class;
		}


		SlType
		sl_object_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_object_init (SlPointer object) {
				if (sl_assert (object)) {
					sl_create_context (SlObject, object);
				}
		}


		SlObject
		sl_object_new (void) {
			SlObject		result = NULL;

			result = (SlObject) malloc (sizeof (SlSObject));
			sl_object_set_class (result, SL_OBJECT_CLASS);
			sl_object_init (result);
			return result;
		}


		void
		sl_object_free_virtual (SlPointer object) {
		}


		void
		sl_object_free (SlPointer object) {
				if (sl_assert (sl_object_is (object, SL_OBJECT_TYPE))) {
					sl_create_context (SlObject, object);

						if (sl_assert (sl_object_get_class (this)->free))
							sl_object_get_class (this)->free (this);
				}
		}


		void
		_sl_object_dispose (SlPointer *object) {
				if (sl_assert (object && sl_object_is (*object, SL_OBJECT_TYPE))) {
					sl_object_free (*object);
					free (*object);
					*object = NULL;
				}
		}


		SlBool
		sl_object_is (SlPointer object, SlType type) {
				if (object && type != SL_TYPE_UNDEFINED) {
					sl_create_context (SlObject, object);

						if (this->class)
							return (this->class->type_tab [type / SL_INT_BITS] & (1 << (type % SL_INT_BITS))) != 0;
						else
							return false;
				} else
					return false;
		}


		void
		sl_object_set_class (SlPointer object, SlPointer class) {
				if (sl_assert (object && class)) {
					sl_create_context (SlObject, object);

					this->class = class;
				}
		}


		SlObjectClass
		sl_object_get_class (SlPointer object) {
				if (sl_assert (sl_object_is (object, SL_OBJECT_TYPE))) {
					sl_create_context (SlObject, object);

					return this->class;
				} else
					return NULL;
		}

	/* - Obiekty koniec ----------------------------------------------------- */
