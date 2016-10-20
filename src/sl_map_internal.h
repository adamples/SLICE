#ifndef _SL_MAP_INTERNAL_H_
#define _SL_MAP_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSObjectClass		parent;
		int (*compare_callback) (const void *a, const void *b);
	} SlSMapClass, *SlMapClass;

#define SL_MAP_CLASS (sl_map_class ())

		void				sl_map_class_init (SlPointer class);
		SlMapClass		sl_map_class ();


	typedef struct _SlMap {
		SlSObject		parent;
		int				size;
		SlPointer		*data;
		SlTable			data_table;
		SlBool			sorted;
	} SlSMap;

		void		sl_map_init (SlPointer map);
		void		sl_map_free_virtual (SlPointer map);

#endif
