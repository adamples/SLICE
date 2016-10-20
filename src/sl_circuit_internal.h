#ifndef _SL_CIRCUIT_INTERNAL_H_
#define _SL_CIRCUIT_INTERNAL_H_

#include "slice_internal.h"


	typedef struct {
		SlSObjectClass		parent;
	} SlSCircuitClass, *SlCircuitClass;

#define SL_CIRCUIT_CLASS (sl_circuit_class ())

		void					sl_circuit_class_init (SlPointer class);
		SlCircuitClass		sl_circuit_class ();

	typedef struct {
		SlLElement		element;
		int				p_node, n_node;
		SlFloat			current;
	} SlElementDesc;

	typedef struct _SlCircuit {
		SlSObject			parent;
		SlElementDesc		*elements;
		int					elements_number;
		SlMapSI				elements_map;
		SlTable				elements_table;
		SlNLElement			*nl_elements;
		int					nl_elements_number;
		SlTable				nl_elements_table;
		int					nodes_number;
		SlMapSI				nodes_map;
		SlFloat				*potentials;
		SlTable				potentials_table;
		SlBool				calculated;
	} SlSCircuit;

		void			sl_circuit_init (SlPointer circuit);
		void			sl_circuit_free_virtual (SlPointer circuit);


#endif