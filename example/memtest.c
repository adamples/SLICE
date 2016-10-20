#include <stdio.h>
#include "slice/slice.h"


	int
	main (void) {
		SlCircuit		C;
		SlResistor		R;
		SlVSource		V;
		SlCSource		I;
		SlDiode			D;
		int				i, j, k;
		char				buffer [256];

				/* Tworzy i usuwa z pamięci obwód 1000 razy */
			for (i = 0; i < 1000; i++) {
				C = sl_circuit_new ();
						/* Dodając do niego 400 elementów */
					for (j = 0; j < 100; j++) {
						sprintf (buffer, "R%d", j);
						R = sl_resistor_new (buffer, 1000);
						sl_circuit_add_element (C, R, "node_a", "node_b");

						sprintf (buffer, "V%d", j);
						V = sl_vsource_new (buffer, 1000);
						sl_circuit_add_element (C, V, "node_a", "node_b");

						sprintf (buffer, "I%d", j);
						I = sl_csource_new (buffer, 1000);
						sl_circuit_add_element (C, I, "node_a", "node_b");

						sprintf (buffer, "D%d", j);
						D = sl_diode_new (buffer, 1, 1, 1);
						sl_circuit_add_element (C, D, "node_a", "node_b");
					}
					/* Comment this to see the difference */
				sl_object_dispose (&C);
			}
		return 0;
	}