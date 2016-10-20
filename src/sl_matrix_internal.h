#ifndef _SL_MATRIX_INTERNAL_H_
#define _SL_MATRIX_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSObjectClass		parent;
	} SlSMatrixClass, *SlMatrixClass;

#define SL_MATRIX_CLASS (sl_matrix_class ())

		void					sl_matrix_class_init (SlPointer class);
		SlMatrixClass		sl_matrix_class ();


	typedef struct _SlMatrix {
		SlSObject			parent;
		int					rows;
		int					columns;
		SlFloat				*elements;
		SlTable2D		elements_table;
	} SlSMatrix;

		void		sl_matrix_init (SlPointer matrix);
		void		sl_matrix_free_virtual (SlPointer matrix);

#endif