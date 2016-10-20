#ifndef _SL_NLELEMENT_INTERNAL_H_
#define _SL_NLELEMENT_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSLElementClass		parent;
	} SlSNLElementClass, *SlNLElementClass;

#define SL_NLELEMENT_CLASS (sl_nlelement_class ())

		void						sl_nlelement_class_init (SlPointer class);
		SlNLElementClass		sl_nlelement_class (void);


	typedef struct {
		SlFloat		vmin, vmax;
		SlFloat		imin, imax;
		SlBool		use_thevenin;
		union {
			struct {
				SlFloat		voltage; /* Thevenin jest używany tylko, gdy Rw = 0 */
			} thevenin;
			struct {
				SlFloat		current;
				SlFloat		conductance;
			} northon;
		} source;
	} SlNLSegment;

	typedef struct _SlNLElement {
		SlSLElement		parent;
		SlNLSegment		*segments;
		SlTable			segments_table;
		int				segments_number;
		int				current_segment;
	} SlSNLElement;

		void			sl_nlelement_init (SlPointer element, SlString id);

		void			sl_nlelement_free_virtual (SlPointer element);
		SlFloat		sl_nlelement_current_virtual (SlPointer element, SlPointer v);

		void			sl_nlelement_add_segment (SlPointer element, SlFloat av, SlFloat ai, SlFloat bv, SlFloat bi);


	typedef struct {
		SlSNLElementClass		parent;
	} SlSDiodeClass, *SlDiodeClass;

#define SL_DIODE_CLASS (sl_diode_class ())

		void				sl_diode_class_init (SlPointer class);
		SlDiodeClass	sl_diode_class ();

	typedef struct _SlDiode {
		SlSNLElement		parent;
	} SlSDiode;

		void		sl_diode_init (SlPointer diode, SlString id, SlFloat Vd, SlFloat Vbr, SlFloat Ir);

#endif