#include "slice_internal.h"


		void
		sl_nlelement_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlNLElementClass, class);

					sl_lelement_class_init (this);
					sl_object_class_add_type (this, SL_NLELEMENT_TYPE);
					((SlObjectClass) this)->free = sl_nlelement_free_virtual;
					((SlLElementClass) this)->get_current = sl_nlelement_current_virtual;
				}
		}


		SlNLElementClass
		sl_nlelement_class (void) {
			static SlNLElementClass		class = NULL;

				if (class == NULL) {
					class = (SlNLElementClass) malloc (sizeof (SlSNLElementClass));
					sl_nlelement_class_init (class);
				}
			return class;
		}


		SlType
		sl_nlelement_type (void) {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_nlelement_init (SlPointer element, SlString id) {
				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE) && id)) {
					sl_create_context (SlNLElement, element);

					sl_lelement_init (this, id);
					this->segments_number = 0;
					this->segments_table = sl_table_new (&this->segments, sizeof (SlNLSegment));
					this->current_segment = 0;
				}
		}


		SlNLElement
		sl_nlelement_new (SlString id) {
			SlNLElement		result = NULL;

			result = (SlNLElement) malloc (sizeof (SlSNLElement));
			sl_object_set_class (result, SL_NLELEMENT_CLASS);
			sl_nlelement_init (result, id);
			return result;
		}


		void
		sl_nlelement_free_virtual (SlPointer element) {
				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

					sl_lelement_free_virtual (this);
					sl_object_dispose (&this->segments_table);
				}
		}


		SlFloat
		sl_nlelement_current_virtual (SlPointer element, SlPointer v) {
			SlFloat			result = 0;
			SlNLSegment		*segment;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE) && sl_object_is (v, SL_MATRIX_TYPE))) {
					sl_create_context (SlNLElement, element);

						if (sl_assert (this->current_segment < this->segments_number)) {
							segment = &this->segments [this->current_segment];
								if (segment->use_thevenin && sl_assert (sl_matrix_height (v) == 3)) {
									result = sl_matrix_get (v, 3, 1);
								} else
									if (sl_assert (sl_matrix_height (v) == 2)) {
										result = segment->source.northon.current + segment->source.northon.conductance * (sl_matrix_get (v, 1, 1) - sl_matrix_get (v, 2, 1));
									}
						}
				}
			return result;
		}


		int
		sl_nlelement_segments_number (SlPointer element) {
			int		result = 0;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

					result = this->segments_number;
				}
			return result;
		}


		void
		sl_nlelement_set_segment (SlPointer element, int segment_id) {
			SlMatrix			g, i;
			SlNLSegment		*segment;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

						if (sl_assert (segment_id >= 0 && segment_id < this->segments_number)) {
							this->current_segment = segment_id;

							segment = &this->segments [this->current_segment];

							g = sl_lelement_get_g (this);
							i = sl_lelement_get_i (this);
								if (segment->use_thevenin) {
									sl_matrix_resize (g, 3, 3);
									sl_matrix_fill (g, 0);
									sl_matrix_set (g, 1, 3, 1);
									sl_matrix_set (g, 2, 3, -1);
									sl_matrix_set (g, 3, 1, 1);
									sl_matrix_set (g, 3, 2, -1);
									sl_matrix_resize (i, 3, 1);
									sl_matrix_fill (i, 0);
									sl_matrix_set (i, 3, 1, segment->source.thevenin.voltage);
								} else {
									sl_matrix_resize (g, 2, 2);
									sl_matrix_set (g, 1, 1, segment->source.northon.conductance);
									sl_matrix_set (g, 2, 2, segment->source.northon.conductance);
									sl_matrix_set (g, 1, 2, -segment->source.northon.conductance);
									sl_matrix_set (g, 2, 1, -segment->source.northon.conductance);
									sl_matrix_resize (i, 2, 1);
									sl_matrix_set (i, 1, 1, -segment->source.northon.current);
									sl_matrix_set (i, 2, 1, segment->source.northon.current);
								}
						}
				}
		}


		int
		sl_nlelement_get_segment (SlPointer element) {
			int		result = 0;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

					result = this->current_segment;
				}
			return result;
		}


		void
		sl_nlelement_add_segment (SlPointer element, SlFloat av, SlFloat ai, SlFloat bv, SlFloat bi) {
			SlNLSegment		*segment;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE) && !(av == bv && ai == bi))) {
					sl_create_context (SlNLElement, element);

					this->segments_number++;
					sl_table_resize (this->segments_table, this->segments_number);
					segment = &this->segments [this->segments_number - 1];
					segment->vmin = sl_float_min (av, bv);
					segment->vmax = sl_float_max (av, bv);
					segment->imin = sl_float_min (ai, bi);
					segment->imax = sl_float_max (ai, bi);
						if (av == bv) { /* Dwójnik Thevenina */
							segment->use_thevenin = true;
							segment->source.thevenin.voltage = av;
						} else { /* Dwójnik Nortona */
							segment->use_thevenin = false;
							segment->source.northon.conductance = (bi - ai) / (bv - av);
							segment->source.northon.current = - segment->source.northon.conductance * av + ai;
						}
						if (this->segments_number == 1)
							sl_nlelement_set_segment (this, 0);
				}
		}


		SlFloat
		sl_nlelement_get_segment_vmin (SlPointer element) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

						if (sl_assert (this->current_segment < this->segments_number))
							result = this->segments [this->current_segment].vmin;
				}
			return result;
		}


		SlFloat
		sl_nlelement_get_segment_vmax (SlPointer element) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

						if (sl_assert (this->current_segment < this->segments_number))
							result = this->segments [this->current_segment].vmax;
				}
			return result;
		}


		SlFloat
		sl_nlelement_get_segment_imin (SlPointer element) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

						if (sl_assert (this->current_segment < this->segments_number))
							result = this->segments [this->current_segment].imin;
				}
			return result;
		}


		SlFloat
		sl_nlelement_get_segment_imax (SlPointer element) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (element, SL_NLELEMENT_TYPE))) {
					sl_create_context (SlNLElement, element);

						if (sl_assert (this->current_segment < this->segments_number))
							result = this->segments [this->current_segment].imax;
				}
			return result;
		}

	/* - Dioda -------------------------------------------------------------- */

		SlType
		sl_diode_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_diode_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlDiodeClass, class);

					sl_nlelement_class_init (this);
					sl_object_class_add_type (this, SL_DIODE_TYPE);
				}
		}


		SlDiodeClass
		sl_diode_class () {
			static SlDiodeClass		class = NULL;

				if (!class) {
					class = (SlDiodeClass) malloc (sizeof (SlSDiodeClass));
					sl_diode_class_init (class);
				}
			return class;
		}


		void
		sl_diode_init (SlPointer diode, SlString id, SlFloat Vd, SlFloat Vbr, SlFloat Ir) {
				if (sl_assert (diode && id && Vd >= 0 && Ir >= 0 && Vbr > 0)) {
					sl_create_context (SlDiode, diode);

					sl_nlelement_init (this, id);
					sl_nlelement_add_segment (this, -Vbr, -Ir, -Vbr, -DBL_MAX);
					sl_nlelement_add_segment (this, -Vbr, -Ir, 0, 0);
					sl_nlelement_add_segment (this, 0, 0, Vd, 0);
					sl_nlelement_add_segment (this, Vd, 0, Vd, DBL_MAX);
				}
		}


		SlDiode
		sl_diode_new (SlString id, SlFloat Vd, SlFloat Vbr, SlFloat Ir) {
			SlDiode		result = NULL;

				if (sl_assert (id && Vd >= 0 && Ir >= 0 && Vbr > 0)) {
					result = (SlDiode) malloc (sizeof (SlSDiode));
					sl_object_set_class (result, SL_DIODE_CLASS);
					sl_diode_init (result, id, Vd, Vbr, Ir);
				}
			return result;
		}
