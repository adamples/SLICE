#ifndef _SL_TABLE_INTERNAL_H_
#define _SL_TABLE_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSObjectClass		parent;
		void (*resize) (SlPointer, int);
	} SlSTableClass, *SlTableClass;

#define SL_TABLE_CLASS (sl_table_class ())

		void						sl_table_class_init (SlPointer class);
		SlTableClass		sl_table_class ();


	typedef struct _SlTable {
		SlSObject		parent;
		char				**data;
		int				block_size;		/* Rozmiar bloku w bajtach */
		int				size;				/* Ilość elementów */
		int				element_size;	/* Rozmiar elementu w bajtach */
		int				allocated;		/* Ilość zalokowanej pamięci w bajtach */
	} SlSTable;

		void		sl_table_init (SlPointer table, SlPointer *data, int element_size);
		void		sl_table_free_virtual (SlPointer table);
		void		sl_table_resize_virtual (SlPointer table, int size);

#endif
