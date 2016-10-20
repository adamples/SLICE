#ifndef _SL_TABLE_2D_INTERNAL_H_
#define _SL_TABLE_2D_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSTableClass		parent;
	} SlSTable2DClass, *SlTable2DClass;

#define SL_TABLE_2D_CLASS (sl_table_2d_class ())

		void							sl_table_2d_class_init (SlPointer class);
		SlTable2DClass		sl_table_2d_class ();


	typedef struct _SlTable2D {
		SlSTable		parent;
		int					width, height;
	} SlSTable2D;

		void		sl_table_2d_init (SlPointer table, SlPointer *data, size_t element_size);
		void		sl_table_2d_resize_virtual (SlPointer table, int size);

#endif