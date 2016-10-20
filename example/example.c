#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <slice/slice.h>


	SlFloat
	str_to_float (char *s) {
		SlFloat		result = 0;
		SlFloat		m = 1;
		char			*u;

		result = strtod (s, &u);
			if (u == s) {
				printf ("example: błąd: nieprawidłowy format liczby \"%s\".\n", s);
				exit (4);
			}
			switch (*u) {
				case 'n'	: m = 1e-9; break;
				case 'u'	: m = 1e-6; break;
				case 'm'	: m = 1e-3; break;
				case 'k'	: m = 1e+3; break;
				case 'M'	: m = 1e+6; break;
				case 0	: break;
				default	:
					printf ("example: błąd: nieprawidłowy format liczby \"%s\".\n", s);
					exit (4);
			}
		result *= m;
		return result;
	}


	void
	print_results (SlCircuit circuit) {
		int			n, i;
		SlString		name;

			if (sl_circuit_recalculate (circuit)) {
				n = sl_circuit_elements_number (circuit);
					for (i = 0; i < n; i++) {
						name = sl_circuit_get_element_name (circuit, i);
						printf ("U(%s) = %14g,  I(%s) = %14g;\n", name, sl_circuit_voltage_by_element (circuit, name), name, sl_circuit_current_by_element (circuit, name));
					}
				printf ("Wszystkich elementów: %d.\n", n);
			} else
				printf ("example: błąd: z bliżej nieokreślonych przyczyn nie udało się wykonać obliczeń.\n");
	}


	void
	parse_input (void) {
		SlCircuit		circuit;
		SlPointer		element;
		char				*name, *p_node, *n_node;
		char				*vbuffer;
		SlFloat			v1, v2, v3;

		circuit = sl_circuit_new ();
			while (scanf ("%ms ", &name) == 1) {
					switch (name [0]) {
						case '.' :
							printf ("example: nie-wiadomo: jeszcze nic tu nie ma (w odpowiedzi na \"%s\").\n", name);
							getline (NULL, NULL, stdin);
							break;
						case '*' :
							printf ("example: nie-wiadomo: jeszcze nic tu nie ma (w odpowiedzi na \"%s\").\n", name);
							getline (NULL, NULL, stdin);
							break;
						default : /* Tutaj to już tylko elementy */
							scanf ("%ms %ms ", &p_node, &n_node);
								switch (name [0]) {
									case 'R' :
										scanf ("%ms ", &vbuffer);
										v1 = str_to_float (vbuffer);
										free (vbuffer);
										element = sl_resistor_new (name, v1);
										break;
									case 'V' :
										scanf ("%ms ", &vbuffer);
											if (strcmp (vbuffer, "DC")) {
												printf ("example: błąd: tylko źrodła DC są wspierane.\n");
												exit (5);
											}
										free (vbuffer);
										scanf ("%ms ", &vbuffer);
										v1 = str_to_float (vbuffer);
										free (vbuffer);
										element = sl_vsource_new (name, v1);
										break;
									case 'I' :
										scanf ("%ms ", &vbuffer);
											if (strcmp (vbuffer, "DC")) {
												printf ("example: błąd: tylko źrodła DC są wspierane.\n");
												exit (5);
											}
										free (vbuffer);
										scanf ("%ms ", &vbuffer);
										v1 = str_to_float (vbuffer);
										free (vbuffer);
										element = sl_csource_new (name, v1);
										break;
									case 'D' :
										scanf ("%ms ", &vbuffer);
										v1 = str_to_float (vbuffer);
											if (v1 < 0) {
												printf ("example: błąd: podano Vf < 0 (\"%s\").\n", name);
												exit (6);
											}
										free (vbuffer);
										scanf ("%ms ", &vbuffer);
										v2 = str_to_float (vbuffer);
											if (v2 <= 0) {
												printf ("example: błąd: podano Vbr <= 0 (\"%s\").\n", name);
												exit (7);
											}
										free (vbuffer);
										scanf ("%ms ", &vbuffer);
										v3 = str_to_float (vbuffer);
											if (v3 < 0) {
												printf ("example: błąd: podano Ir < 0 (\"%s\").\n", name);
												exit (8);
											}
										free (vbuffer);
										element = sl_diode_new (name, v1, v2, v3);
										break;
									default :
										printf ("example: błąd: nieznany element \"%s\".\n", name);
										exit (3);
								}
								if (sl_circuit_element_exists (circuit, name)) {
									printf ("example: błąd: dwa elementy o tej samej nazwie \"%s\".\n", name);
									exit (9);
								}
							sl_circuit_add_element (circuit, element, p_node, n_node);
							free (p_node);
							free (n_node);
					}
				free (name);
			}
		print_results (circuit);
		sl_object_dispose (&circuit);
	}


	int
	main (int argc, char **argv) {

			switch (argc) {
				case 2 :
					fclose (stdin);
					stdin = fopen (argv [1], "r");
						if (!stdin) {
							printf ("example: błąd: nie można otworzyć pliku \"%s\".\n", argv [1]);
							exit (2);
						}
					printf ("Przykładowy program libslice (%s)\n", argv [1]);
					break;
				case 1 :		/* Używane jest stdin */
					printf ("Przykładowy program libslice (stdin)\n");
					break;
				default :
					printf ("example: błąd: nieprawidłowe argumenty.\n");
					exit (1);
					break;
			}
		parse_input ();

		return 0;
	}
