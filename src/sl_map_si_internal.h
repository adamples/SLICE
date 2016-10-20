#ifndef _SL_MAP_SI_INTERNAL_H_
#define _SL_MAP_SI_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSMapClass		parent;
	} SlSMapSIClass, *SlMapSIClass;

#define SL_MAP_SI_CLASS (sl_map_si_class ())

		void					sl_map_si_class_init (SlPointer class);
		SlMapSIClass		sl_map_si_class ();


	typedef struct _SlMapSI {
		SlSMap		parent;
	} SlSMapSI;

		void		sl_map_si_init (SlPointer map);
		void		sl_map_si_free_virtual (SlPointer map);


#endif