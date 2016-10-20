#ifndef _SL_LELEMENT_INTERNAL_H_
#define _SL_LELEMENT_INTERNAL_H_

#include "slice_internal.h"


	/* - Ogólny element liniowy --------------------------------------------- */

	typedef struct _SlLElementClass {
		SlSObjectClass		parent;
		SlMatrix (*get_g) (SlPointer element);
		SlMatrix (*get_i) (SlPointer element);
		SlFloat (*get_current) (SlPointer element, SlPointer v);
	} SlSLElementClass, *SlLElementClass;

#define SL_LELEMENT_CLASS (sl_lelement_class ())

		void						sl_lelement_class_init (SlPointer class);
		SlLElementClass		sl_lelement_class ();


	typedef struct _SlLElement {
		SlSObject		parent;
		SlString			id;
		SlMatrix		g;
		SlMatrix		i;
	} SlSLElement;

		void				sl_lelement_init (SlPointer element, SlString id);
		SlLElement		sl_lelement_new (SlString id);

		SlFloat			sl_lelement_current (SlPointer element, SlPointer v);

		void				sl_lelement_free_virtual (SlPointer element);
		SlMatrix			sl_lelement_get_g_virtual (SlPointer element);
		SlMatrix			sl_lelement_get_i_virtual (SlPointer element);
		SlFloat			sl_lelement_current_virtual (SlPointer element, SlPointer v);

	/* - Rezystor ----------------------------------------------------------- */

	typedef struct _SlResistorClass {
		SlSLElementClass		parent;
	} SlSResistorClass, *SlResistorClass;

#define SL_RESISTOR_CLASS (sl_resistor_class ())

		void						sl_resistor_class_init (SlPointer class);
		SlResistorClass		sl_resistor_class ();


	typedef struct _SlResistor {
		SlSLElement		parent;
		SlFloat			resistance;
	} SlSResistor;

		void			sl_resistor_init (SlPointer element, SlString id, SlFloat resistance);
		SlFloat		sl_resistor_current_virtual (SlPointer element, SlPointer v);

	/* - Wymuszenie napięciowe ---------------------------------------------- */

	typedef struct _SlVSourceClass {
		SlSLElementClass		parent;
	} SlSVSourceClass, *SlVSourceClass;

#define SL_VSOURCE_CLASS (sl_vsource_class ())

		void					sl_vsource_class_init (SlPointer class);
		SlVSourceClass		sl_vsource_class ();

	typedef struct _SlVSource {
		SlSLElement		parent;
		SlFloat			voltage;
	} SlSVSource;

		void			sl_vsource_init (SlPointer element, SlString id, SlFloat voltage);
		SlFloat		sl_vsource_current_virtual (SlPointer element, SlPointer v);

	/* - Wymuszenie prądowe ------------------------------------------------- */

	typedef struct _SlCSourceClass {
		SlSLElementClass		parent;
	} SlSCSourceClass, *SlCSourceClass;

#define SL_CSOURCE_CLASS (sl_csource_class ())

		void					sl_csource_class_init (SlPointer class);
		SlCSourceClass		sl_csource_class ();

	typedef struct _SlCSource {
		SlSLElement		parent;
		SlFloat			current;
	} SlSCSource;

		void			sl_csource_init (SlPointer element, SlString id, SlFloat current);
		SlFloat		sl_csource_current_virtual (SlPointer element, SlPointer v);

#endif
