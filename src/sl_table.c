#include "slice_internal.h"

#include <stdlib.h>


		void
		sl_table_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlTableClass, class);

					sl_object_class_init (this);
					sl_object_class_add_type (this, SL_TABLE_TYPE);
					((SlObjectClass) this)->free = sl_table_free_virtual;
					this->resize = sl_table_resize_virtual;
				}
		}

		SlTableClass
		sl_table_class () {
			static SlTableClass		class = NULL;

				if (!class) {
					class = (SlTableClass) malloc (sizeof (SlSTableClass));
					sl_table_class_init (class);
				}
			return class;
		}


		SlType
		sl_table_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_table_init (SlPointer table, SlPointer *data, int element_size) {
				if (sl_assert (table && data && element_size > 0)) {
					sl_create_context (SlTable, table);

					sl_object_init (this);
					this->data = (char **) data;
					*this->data = NULL;
					this->block_size = SL_DEFAULT_BLOCK_SIZE;
					this->element_size = element_size;
					this->allocated = 0;
				}
		}


		SlTable
		_sl_table_new (SlPointer *data, int element_size) {
			SlTable		result = NULL;

				if (sl_assert (data && element_size > 0)) {
					result = (SlTable) malloc (sizeof (SlSTable));
					sl_object_set_class (result, SL_TABLE_CLASS);
					sl_table_init (result, data, element_size);
				}
			return result;
		}


		void
		sl_table_free_virtual (SlPointer table) {
			sl_table_resize (table, 0);
		}


		void
		sl_table_resize_virtual (SlPointer table, int size) {
			int allocate = 0;

				if (sl_assert (sl_object_is (table, SL_TABLE_TYPE) && size >= 0)) {
					sl_create_context (SlTable, table);

					allocate = ((size * this->element_size + this->block_size - 1) / this->block_size) * this->block_size;
					this->size = size;
						if (allocate != this->allocated) {
								if (this->size == 0) {
									free (*this->data);
									*this->data = NULL;
								} else
									if (*this->data == NULL)
										*this->data = malloc (allocate);
									else
										*this->data = realloc (*this->data, allocate);
								if (allocate > this->allocated)
									memset ((*this->data) + this->allocated, 0, allocate - this->allocated);
							this->allocated = allocate;
						}
				}
		}


		void
		sl_table_resize (SlPointer table, int size) {
				if (sl_assert (sl_object_is (table, SL_TABLE_TYPE)))
					((SlTableClass) sl_object_get_class (table))->resize (table, size);
		}


		void
		sl_table_set_block_size (SlPointer table, int block_size) {
				if (sl_assert (sl_object_is (table, SL_TABLE_TYPE) && block_size > 0)) {
					sl_create_context (SlTable, table);

					this->block_size = block_size;
					sl_table_resize (this, this->size);
				}
		}


		int
		sl_table_get_block_size (SlPointer table) {
			int		result = 0;

				if (sl_assert (sl_object_is (table, SL_TABLE_TYPE))) {
					sl_create_context (SlTable, table);

					result = this->block_size;
				}
			return result;
		}
