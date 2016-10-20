#include "slice_internal.h"

#include <stdlib.h>
#include <string.h>


		SlType
		sl_table_2d_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_table_2d_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlTable2DClass, class);

					sl_table_class_init (this);
					sl_object_class_add_type (this, SL_TABLE_2D_TYPE);
					((SlTableClass) this)->resize = sl_table_2d_resize_virtual;
				}
		}


		SlTable2DClass
		sl_table_2d_class () {
			static SlTable2DClass		class = NULL;

				if (class == NULL) {
					class = (SlTable2DClass) malloc (sizeof (SlTable2DClass));
					sl_table_2d_class_init (class);
				}
			return class;
		}


		void
		sl_table_2d_init (SlPointer table, SlPointer *data, size_t element_size) {
				if (sl_assert (table)) {
					sl_create_context (SlTable2D, table);

					sl_table_init (table, data, element_size);
					this->width = 0;
					this->height = 0;
				}
		}


		SlTable2D
		_sl_table_2d_new (SlPointer *data, size_t element_size) {
			SlTable2D		result = NULL;

				if (sl_assert (data && element_size > 0)) {
					result = (SlTable2D) malloc (sizeof (SlSTable2D));
					sl_object_set_class (result, SL_TABLE_2D_CLASS);
					sl_table_2d_init (result, data, element_size);
				}
			return result;
		}


		int min (int a, int b) { if (a < b) return a; else return b; }

		void
		sl_table_2d_resize (SlPointer table, int width, int height) {
			int			i;
			SlTable		a;

				if (sl_assert (sl_object_is (table, SL_TABLE_2D_TYPE)) && ((width >= 0 && height >= 0) || (!width && !height))) {
					sl_create_context (SlTable2D, table);

					a = (SlTable) table;
						/* Przesuwanie elementów w zależności, czy zwiększamy, czy zmniejszamy */
						if (width > this->width) { /* Zwiększamy szerokość, więc przesuwamy po i w przód */
							sl_table_resize_virtual (this, width * height);
								for (i = min (this->height, height); i >= 1 ; i--)
									memmove (*a->data + width * i * a->element_size, *a->data + this->width * i * a->element_size, this->width * a->element_size);
								for (i = 0; i < min (this->height, height); i++)
									memset (*a->data + (width * i + this->width) * a->element_size, 0, (width - this->width) * a->element_size);
						} else
							if (width < this->width) {
									for (i = 1; i <= min (this->height, height); i++)
										memmove (*a->data + width * i * a->element_size, *a->data + this->width * i * a->element_size, width * a->element_size);
								sl_table_resize_virtual (this, width * height);
							} else
								if (height != this->height)
									sl_table_resize_virtual (this, width * height);
							/* Czyszczenie nowo wstawionych wierszy */
						for (i = this->height; i < height; i++)
							memset (*a->data + width * i * a->element_size, 0, width * a->element_size);
					this->width = width;
					this->height = height;
				}
		}


		void
		sl_table_2d_resize_virtual (SlPointer table, int size) {
			sl_table_2d_resize (table, size, 1);
		}
