#include "slice_internal.h"

#include <stdlib.h>


		void
		sl_map_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlMapClass, class);

					sl_object_class_init (this);
					sl_object_class_add_type (this, SL_MAP_TYPE);
					((SlObjectClass) this)->free = sl_map_free_virtual;
					this->compare_callback = NULL;
				}
		}


		SlMapClass
		sl_map_class () {
			static SlMapClass		result = NULL;

				if (!result) {
					result = (SlMapClass) malloc (sizeof (SlSMapClass));
					sl_map_class_init (result);
				}
			return result;
		}


		SlType
		sl_map_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_map_init (SlPointer map) {
				if (sl_assert (map)) {
					sl_create_context (SlMap, map);

					sl_object_init (this);
					this->size = 0;
					this->data_table = sl_table_new (&this->data, 2 * sizeof (SlPointer));
					this->sorted = true;
				}
		}


		SlMap
		sl_map_new () {
			SlMap		result = NULL;

			result = (SlMap) malloc (sizeof (SlSMap));
			sl_object_set_class (result, SL_MAP_CLASS);
			sl_map_init (result);
			return result;
		}


		void
		sl_map_free_virtual (SlPointer map) {
				if (sl_assert (sl_object_is (map, SL_MAP_TYPE))) {
					sl_create_context (SlMap, map);

					sl_object_dispose (&this->data_table);
					sl_object_free_virtual (this);
				}
		}


		void
		sl_map_sort (SlPointer map) {
				if (sl_assert (sl_object_is (map, SL_MAP_TYPE))) {
					sl_create_context (SlMap, map);

						if (sl_assert (((SlMapClass) sl_object_get_class (this))->compare_callback))
							qsort (this->data, this->size, 2 * sizeof (SlPointer), ((SlMapClass) sl_object_get_class (this))->compare_callback);
					this->sorted = true;
				}
		}


		SlBool
		sl_map_isset (SlPointer map, SlPointer key) {
			SlBool			result = false;

				if (sl_assert (sl_object_is (map, SL_MAP_TYPE) && key)) {
					sl_create_context (SlMap, map);

						if (!this->sorted)
							sl_map_sort (map);
					result = NULL != bsearch (&key, this->data, this->size, 2 * sizeof (SlPointer), ((SlMapClass) sl_object_get_class (this))->compare_callback);
				}
			return result;
		}


		void
		sl_map_add (SlPointer map, SlPointer key, SlPointer value) {
				if (sl_assert (sl_object_is (map, SL_MAP_TYPE) && key)) {
					sl_create_context (SlMap, map);

					this->sorted = false;
					this->size++;
					sl_table_resize (this->data_table, this->size);
					this->data [this->size * 2 - 2] = key;
					this->data [this->size * 2 - 1] = value;
				}
		}


		SlPointer
		sl_map_get (SlPointer map, SlPointer key) {
			SlPointer		result = NULL;
			SlPointer		*h_result = NULL;

				if (sl_assert (sl_object_is (map, SL_MAP_TYPE) && key)) {
					sl_create_context (SlMap, map);

						if (!this->sorted)
							sl_map_sort (map);
					h_result = bsearch (&key, this->data, this->size, 2 * sizeof (SlPointer), ((SlMapClass) sl_object_get_class (this))->compare_callback);
						if (h_result)
							result = *(h_result + 1);
				}
			return result;
		}
