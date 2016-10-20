#ifndef _SL_TYPES_INTERNAL_H_
#define _SL_TYPES_INTERNAL_H_

#include "slice_internal.h"


#define SL_INT_BITS		(8 * sizeof (int))

#ifndef SL_TYPE_TAB_SIZE
#define SL_TYPE_TAB_SIZE	1
#endif

#define SL_MAX_TYPES		(SL_INT_BITS * SL_TYPE_TAB_SIZE - 1)

	typedef unsigned int		SlTypeTab [SL_TYPE_TAB_SIZE];

		SlType				sl_type_new ();


	typedef struct {
		SlTypeTab		type_tab;
		void (*free) (SlPointer object);
	} SlSObjectClass, *SlObjectClass;

#define SL_OBJECT_CLASS (sl_object_class ())

		void					sl_object_class_init (SlPointer class);
		SlObjectClass		sl_object_class ();
		void					sl_object_class_add_type (SlPointer class, SlType type);


	typedef struct _SlObject {
		SlObjectClass		class;
	} SlSObject;


		void					sl_object_init (SlPointer object);
		void					sl_object_free_virtual (SlPointer object);
		void					sl_object_set_class (SlPointer object, SlPointer class);
		SlObjectClass		sl_object_get_class (SlPointer object);


#endif
