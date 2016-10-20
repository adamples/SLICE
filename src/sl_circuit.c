#include "slice_internal.h"

#include <stdlib.h>
#include <math.h>


		void
		sl_circuit_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlCircuitClass, class);

					sl_object_class_init (this);
					sl_object_class_add_type (this, SL_CIRCUIT_TYPE);
					((SlObjectClass) this)->free = sl_circuit_free_virtual;
				}
		}


		SlCircuitClass
		sl_circuit_class (void) {
			static SlCircuitClass		class = NULL;

				if (!class) {
					class = (SlCircuitClass) malloc (sizeof (SlSCircuitClass));
					sl_circuit_class_init (class);
				}
			return class;
		}


		SlType
		sl_circuit_type (void) {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_circuit_init (SlPointer circuit) {
				if (sl_assert (circuit)) {
					sl_create_context (SlCircuit, circuit);

					sl_object_init (this);
					this->elements_number = 0;
					this->elements_map = sl_map_si_new ();
					this->elements_table = sl_table_new (&this->elements, sizeof (SlElementDesc));
					this->nodes_number = 0;
					this->nodes_map = sl_map_si_new ();
					this->calculated = false;
					this->potentials_table = sl_table_new (&this->potentials, sizeof (SlFloat));
					this->nl_elements_number = 0;
					this->nl_elements_table = sl_table_new (&this->nl_elements, sizeof (SlNLElement));
				}
		}


		SlCircuit
		sl_circuit_new (void) {
			SlCircuit		result = NULL;

			result = (SlCircuit) malloc (sizeof (SlSCircuit));
			sl_object_set_class (result, SL_CIRCUIT_CLASS);
			sl_circuit_init (result);
			return result;
		}


		void
		sl_circuit_free_virtual (SlPointer circuit) {
			int		i;

				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

						for (i = 0; i < this->elements_number; i++)
							sl_object_dispose (&this->elements [i].element);
					sl_object_dispose (&this->elements_map);
					sl_object_dispose (&this->nodes_map);
					sl_object_dispose (&this->elements_table);
					sl_object_dispose (&this->nl_elements_table);
					sl_object_dispose (&this->potentials_table);
				}
		}


		void
		sl_circuit_add_element (SlPointer circuit, SlPointer element, SlString p_node, SlString n_node) {
				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE) && sl_object_is (element, SL_LELEMENT_TYPE) && p_node && n_node)) {
					sl_create_context (SlCircuit, circuit);

						if (sl_assert (!sl_map_isset (this->elements_map, sl_lelement_get_id (element)))) {
								if (!sl_map_isset (this->nodes_map, p_node)) {
									sl_map_si_add (this->nodes_map, p_node, this->nodes_number + 1);
									this->nodes_number++;
								}
								if (!sl_map_isset (this->nodes_map, n_node)) {
									sl_map_si_add (this->nodes_map, n_node, this->nodes_number + 1);
									this->nodes_number++;
								}
							this->elements_number++;
							sl_map_si_add (this->elements_map, sl_lelement_get_id (element), this->elements_number - 1);
							sl_table_resize (this->elements_table, this->elements_number);
							this->elements [this->elements_number - 1].element = element;
							this->elements [this->elements_number - 1].p_node = sl_map_si_get (this->nodes_map, p_node);
							this->elements [this->elements_number - 1].n_node = sl_map_si_get (this->nodes_map, n_node);
								if (sl_object_is (element, SL_NLELEMENT_TYPE)) {
									this->nl_elements_number++;
									sl_table_resize (this->nl_elements_table, this->nl_elements_number);
									this->nl_elements [this->nl_elements_number - 1] = (SlNLElement) element;
								}
						}
				}
		}


		SlFloat
		sl_circuit_potential (SlCircuit circuit, int node) {
			SlFloat		result = 0;

				if (sl_assert (node > 0 &&  node <= circuit->nodes_number))
					result = circuit->potentials [node - 1];
			return result;
		}


		void
		sl_circuit_add_gi (SlCircuit circuit, SlMatrix tg, SlMatrix ti, int e) {
			SlMatrix		g, i;
			int			p, n;
			int			add = 0;
			int			j, k;
			int			tn;

			g = sl_lelement_get_g (circuit->elements [e].element);
			i = sl_lelement_get_i (circuit->elements [e].element);
			p = circuit->elements [e].p_node;
			n = circuit->elements [e].n_node;
			tn = sl_matrix_width (tg);
				if (sl_matrix_height (g) > 2) {
					add = sl_matrix_height (g) - 2;
					sl_matrix_resize (tg, tn + add, tn + add);
					sl_matrix_resize (ti, tn + add, 1);
				}
				/* Wstawianie p_node i n_node */
			sl_matrix_set (tg, p, p, sl_matrix_get (tg, p, p) + sl_matrix_get (g, 1, 1));
			sl_matrix_set (tg, p, n, sl_matrix_get (tg, p, n) + sl_matrix_get (g, 1, 2));
			sl_matrix_set (tg, n, p, sl_matrix_get (tg, n, p) + sl_matrix_get (g, 2, 1));
			sl_matrix_set (tg, n, n, sl_matrix_get (tg, n, n) + sl_matrix_get (g, 2, 2));
			sl_matrix_set (ti, p, 1, sl_matrix_get (ti, p, 1) + sl_matrix_get (i, 1, 1));
			sl_matrix_set (ti, n, 1, sl_matrix_get (ti, n, 1) + sl_matrix_get (i, 2, 1));
				/* Wstawianie dodatkowych węzłów - nie trzeba dodawać, bo nic innego nie zostanie tam wstawione */
				for (j = 1; j <= add; j++) {
					sl_matrix_set (tg, p, tn + j, sl_matrix_get (g, 1, 2 + j));
					sl_matrix_set (tg, n, tn + j, sl_matrix_get (g, 2, 2 + j));
					sl_matrix_set (tg, tn + j, p, sl_matrix_get (g, 2 + j, 1));
					sl_matrix_set (tg, tn + j, n, sl_matrix_get (g, 2 + j, 2));
						for (k = 1; k <= add; k++)
							sl_matrix_set (tg, tn + j, tn + k, sl_matrix_get (g, 2 + j, 2 + k));
					sl_matrix_set (ti, tn + j, 1, sl_matrix_get (i, 2 + j, 1));
				}
		}


		void
		sl_circuit_recalculate_linear (SlCircuit this) {
			SlMatrix		g, i, v, ev;
			int			j, k;
			int			tadd, cadd;

			this->calculated = false;
			g = sl_matrix_new ();
			sl_matrix_resize (g, this->nodes_number, this->nodes_number);
			i = sl_matrix_new ();
			sl_matrix_resize (i, this->nodes_number, 1);
				for (j = 0; j < this->elements_number; j++)
					sl_circuit_add_gi (this, g, i, j);
			sl_matrix_delete_column (g, 1);
			sl_matrix_delete_row (i, 1);
			sl_matrix_delete_row (g, 1);
				if (sl_matrix_invert (g)) {
					v = sl_matrix_multiply (g, i);
					sl_table_resize (this->potentials_table, this->nodes_number);
					this->potentials [0] = 0;
						for (j = 1; j < this->nodes_number; j++)
							this->potentials [j] = sl_matrix_get (v, j, 1);
					tadd = 0;
					ev = sl_matrix_new ();
							/* Obliczanie prądów - w tym celu elementom jest zwracany wynik analizy */
						for (j = 0; j < this->elements_number; j++) {
							sl_matrix_resize (ev, sl_matrix_height (sl_lelement_get_g (this->elements [j].element)), 1);
							cadd = 0;
								if (sl_matrix_height (ev) > 2)
									cadd = sl_matrix_height (ev) - 2;
							sl_matrix_set (ev, 1, 1, sl_circuit_potential (this, this->elements [j].p_node));
							sl_matrix_set (ev, 2, 1, sl_circuit_potential (this, this->elements [j].n_node));
								for (k = 0; k < cadd; k++)
									sl_matrix_set (ev, 3 + k, 1, sl_matrix_get (v, this->nodes_number + tadd + k, 1));
							tadd += cadd;
							this->elements [j].current = sl_lelement_current (this->elements	 [j].element, ev);
						}
					this->calculated = true;
					sl_object_dispose (&ev);
					sl_object_dispose (&v);
				}
			sl_object_dispose (&g);
			sl_object_dispose (&i);
		}


		void
		sl_circuit_first_permutation (SlCircuit this) {
			int		i;

				for (i = 0; i < this->nl_elements_number; i++)
					sl_nlelement_set_segment (this->nl_elements [i], 0);
		}


		SlBool
		sl_circuit_next_permutation (SlCircuit this) {
			int		i;

				for (i = 0; i < this->nl_elements_number && sl_nlelement_get_segment (this->nl_elements [i]) == sl_nlelement_segments_number (this->nl_elements [i]) - 1; i++)
					sl_nlelement_set_segment (this->nl_elements [i], 0);
				if (i < this->nl_elements_number)
					sl_nlelement_set_segment (this->nl_elements [i], sl_nlelement_get_segment (this->nl_elements [i]) + 1);
			return (i < this->nl_elements_number);
		}


		SlBool
		sl_circuit_check_bounds (SlCircuit this) {
			SlBool		result = true;
			SlFloat		current, voltage;
			SlFloat		vmin, vmax, imin, imax;
			int			i;

				if (this->calculated)
					for (i = 0; result && i < this->nl_elements_number; i++) {
						voltage = sl_circuit_voltage_by_element (this, sl_lelement_get_id (this->nl_elements [i]));
						current = sl_circuit_current_by_element (this, sl_lelement_get_id (this->nl_elements [i]));
						vmin = sl_nlelement_get_segment_vmin (this->nl_elements [i]);
						vmax = sl_nlelement_get_segment_vmax (this->nl_elements [i]);
						imin = sl_nlelement_get_segment_imin (this->nl_elements [i]);
						imax = sl_nlelement_get_segment_imax (this->nl_elements [i]);
							if (voltage - vmin <= -1e-9 || vmax - voltage <= -1e-9 || current - imin <= -1e-9 || imax - current <= -1e-9)
								result = false;
					}
				else
					result = false;
			return result;
		}


		SlBool
		sl_circuit_recalculate (SlPointer circuit) {
			int			i;
			SlBool		result = false;

				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

						if (sl_assert (this->elements_number > 0)) {
							sl_circuit_first_permutation (this);
								do {
									sl_circuit_recalculate_linear (this);
								} while (!(result = sl_circuit_check_bounds(this)) && sl_circuit_next_permutation (this));
							this->calculated = result;
						}
				}
			return result;
		}


		void
		sl_circuit_check_if_calculated (SlCircuit circuit) {
				if (!circuit->calculated)
					sl_circuit_recalculate (circuit);
		}


		SlFloat
		sl_circuit_voltage_by_nodes (SlPointer circuit, SlString p_node, SlString n_node) {
			int			p, n;
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

					p = sl_map_si_get (this->nodes_map, p_node);
					n = sl_map_si_get (this->nodes_map, n_node);
					sl_circuit_check_if_calculated (this);
						if (sl_assert (p > 0 && p <= this->nodes_number && n > 0  && n <= this->nodes_number && this->calculated))
							result = sl_circuit_potential (this, p) - sl_circuit_potential (this, n);
				}
			return result;
		}


		SlFloat
		sl_circuit_voltage_by_element (SlPointer circuit, SlString element_id) {
			int			e, p, n;
			SlFloat		result = 0;

				if (sl_assert (element_id && sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

						if (sl_assert (sl_map_isset (this->elements_map, element_id))) {
							e = sl_map_si_get (this->elements_map, element_id);
							p = this->elements [e].p_node;
							n = this->elements [e].n_node;
							sl_circuit_check_if_calculated (this);
								if (sl_assert (this->calculated))
									result = sl_circuit_potential (this, p) - sl_circuit_potential (this, n);
						}
				}
			return result;
		}


		SlFloat
		sl_circuit_current_by_element (SlPointer circuit, SlString element_id) {
			int			e, p, n;
			SlFloat		result = 0;

				if (sl_assert (element_id && sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

					sl_circuit_check_if_calculated (this);
						if (sl_assert (sl_map_isset (this->elements_map, element_id) && this->calculated))
							result = this->elements [sl_map_si_get (this->elements_map, element_id)].current;
				}
			return result;
		}


		int
		sl_circuit_elements_number (SlPointer circuit) {
			int		result = 0;

				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

					result = this->elements_number;
				}
			return result;
		}


		SlString
		sl_circuit_get_element_name (SlPointer circuit, int n) {
			SlString		result = NULL;

				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE))) {
					sl_create_context (SlCircuit, circuit);

						if (sl_assert (n >= 0 && n < this->elements_number))
							result = sl_lelement_get_id (this->elements [n].element);
				}
			return result;
		}


		SlBool
		sl_circuit_element_exists (SlPointer circuit, SlString element_id) {
			SlBool		result = false;

				if (sl_assert (sl_object_is (circuit, SL_CIRCUIT_TYPE) && element_id)) {
					sl_create_context (SlCircuit, circuit);

					result = sl_map_isset (this->elements_map, element_id);
				}
		}
