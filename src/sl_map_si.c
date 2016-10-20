#include "slice_internal.h"

#include <stdlib.h>
#include <string.h>


		int
		sl_map_si_compare (const char **a, const char **b) {
				if (sl_assert (a && b && *a && *b)) {
					return strcmp (*a, *b);
				} else
					return 0;
		}


		void
		sl_map_si_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlMapSIClass, class);

					sl_map_class_init (this);
					sl_object_class_add_type (this, SL_MAP_SI_TYPE);
					((SlObjectClass) this)->free = sl_map_si_free_virtual;
					this->parent.compare_callback = (int (*) (const void *, const void *)) sl_map_si_compare;
				}
		}


		SlMapSIClass
		sl_map_si_class () {
			static SlMapSIClass		class = NULL;

				if (class == NULL) {
					class = (SlMapSIClass) malloc (sizeof (SlSMapSIClass));
					sl_map_si_class_init (class);
				}
			return class;
		}


		SlType
		sl_map_si_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_map_si_init (SlPointer map) {
				if (sl_assert (map)) {
					sl_create_context (SlMapSI, map);

					sl_map_init (this);
				}
		}


		SlMapSI
		sl_map_si_new () {
			SlMapSI		result = NULL;

			result = (SlMapSI) malloc (sizeof (SlSMapSI));
			sl_object_set_class (result, SL_MAP_SI_CLASS);
			sl_map_si_init (result);
			return result;
		}


		void
		sl_map_si_free_virtual (SlPointer map) {
			int		i;

				if (sl_assert (sl_object_is (map, SL_MAP_SI_TYPE))) {
					sl_create_context (SlMap, map);

						for (i = 0; i < this->size; i++)
							free (this->data [i * 2]);
					sl_map_free_virtual (this);
				}
		}


		void
		sl_map_si_add (SlPointer map, SlString key, size_t value) {
				if (sl_assert (sl_object_is (map, SL_MAP_SI_TYPE) && key)) {
					sl_create_context (SlMapSI, map);

					sl_map_add (this, sl_string_clone (key), (SlPointer) value);
				}
		}


		int
		sl_map_si_get (SlPointer map, SlString key) {
			int		result = 0;

				if (sl_assert (sl_object_is (map, SL_MAP_SI_TYPE) && key)) {
					sl_create_context (SlMapSI, map);

					result = (size_t) sl_map_get (this, key);
				}
			return result;
		}


		void
		sl_map_si_print (SlPointer map) {
			int		i;

				if (sl_assert (sl_object_is (map, SL_MAP_TYPE))) {
					sl_create_context (SlMap, map);

						for (i = 0; i < this->size; i++) {
							printf ("\"%s\" => %d\n", this->data [i * 2], (size_t) this->data [i * 2 + 1]);
						}
				}
		}

