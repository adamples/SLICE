#include "slice_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


		SlType
		sl_matrix_type () {
			static SlType		type = SL_TYPE_UNDEFINED;

				if (type == SL_TYPE_UNDEFINED)
					type = sl_type_new ();
			return type;
		}


		void
		sl_matrix_class_init (SlPointer class) {
				if (sl_assert (class)) {
					sl_create_context (SlMatrixClass, class);

					sl_object_class_init (this);
					sl_object_class_add_type (this, SL_MATRIX_TYPE);
					((SlObjectClass) this)->free = sl_matrix_free_virtual;
				}
		}


		SlMatrixClass
		sl_matrix_class () {
			static SlMatrixClass		class = NULL;

				if (!class) {
					class = (SlMatrixClass) malloc (sizeof (SlSMatrixClass));
					sl_matrix_class_init (class);
				}
			return class;
		}


		void
		sl_matrix_init (SlPointer matrix) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					sl_object_init (this);
					this->elements_table = sl_table_2d_new ((void **) &this->elements, sizeof (SlFloat));
					this->rows = 0;
					this->columns = 0;
				}
		}


		SlMatrix
		sl_matrix_new (void) {
			SlMatrix		result;

			result = (SlMatrix) malloc (sizeof (SlSMatrix));
			sl_object_set_class (result, SL_MATRIX_CLASS);
			sl_matrix_init (result);
			return result;
		}


		SlMatrix
		sl_matrix_new_identity (int size) {
			SlMatrix		result;

			result = sl_matrix_new ();
				if (sl_assert (result))
					sl_matrix_setup_identity (result, size);
			return result;
		}


		void
		sl_matrix_free_virtual (SlPointer matrix) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					sl_object_dispose (&this->elements_table);
					this->rows = 0;
					this->columns = 0;
				}
		}


			/* Dostęp do danych */

		SlFloat
		sl_matrix_get (SlPointer matrix, int row, int column) {
			SlFloat		result = 0;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (sl_assert (row >= 1 && row <= this->rows && column >= 1 && column <= this->columns))
							result = this->elements [column - 1 + (row - 1) * this->columns];
				}
			return result;
		}


		void
		sl_matrix_set (SlPointer matrix, int row, int column, SlFloat element) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (sl_assert (row >= 1 && row <= this->rows && column >= 1 && column <= this->columns))
							this->elements [column - 1 + (row - 1) * this->columns] = element;
				}
		}


		void
		sl_matrix_fill (SlPointer matrix, SlFloat element) {
			int		i;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						for (i = 0; i < this->rows * this->columns; i++)
							this->elements [i] = element;
				}
		}


		void
		sl_matrix_copy (SlPointer source, SlPointer destination) {
			SlMatrix		s, d;

				if (sl_assert (sl_object_is (source, SL_MATRIX_TYPE) && sl_object_is (destination, SL_MATRIX_TYPE))) {
					s = (SlMatrix) source;
					d = (SlMatrix) destination;
					sl_matrix_resize (destination, s->rows, s->columns);
						if (s->rows && s->columns)
							memcpy (d->elements, s->elements, s->rows * s->columns * sizeof (SlFloat));
				}
		}


		SlMatrix
		sl_matrix_clone (SlPointer source) {
			SlMatrix		result = NULL;

				if (sl_assert (sl_object_is (source, SL_MATRIX_TYPE))) {
					result = sl_matrix_new ();
					sl_matrix_copy (source, result);
				}
			return result;
		}


			/* Manipulacja rozmiarem */

		int
		sl_matrix_width (SlPointer matrix) {
			int		result = 0;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					result = this->columns;
				}
			return result;
		}


		int
		sl_matrix_height (SlPointer matrix) {
			int		result = 0;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					result = this->rows;
				}
			return result;
		}


		void
		sl_matrix_resize (SlPointer matrix, int rows, int columns) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE) && ((rows > 0 && columns > 0) || (rows == 0 && columns == 0)))) {
					sl_create_context (SlMatrix, matrix);

					this->rows = rows;
					this->columns = columns;
					sl_table_2d_resize (this->elements_table, columns, rows);
				}
		}


		void
		sl_matrix_swap (SlMatrix a, SlMatrix b) {
			SlFloat		*ftmp;
			int			itmp;
			SlMatrix	am, bm;

				if (sl_assert (sl_object_is (a, SL_MATRIX_TYPE) && sl_object_is (b, SL_MATRIX_TYPE))) {
					am = (SlMatrix) a;
					bm = (SlMatrix) b;

					ftmp = am->elements;
					am->elements = bm->elements;
					bm->elements = ftmp;
					itmp = am->rows;
					am->rows = bm->rows;
					bm->rows = itmp;
					itmp = am->columns;
					am->columns = bm->columns;
					bm->columns = itmp;
				}
		}


			/* Tworzenie macierzy */

		void
		sl_matrix_setup_identity (SlPointer matrix, int size) {
			int		r, c;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE) && size > 0)) {
					sl_create_context (SlMatrix, matrix);

					sl_matrix_resize (this, size, size);
						for (r = 1; r <= this->rows; r++)
							for (c = 1; c <= this->columns; c++)
								sl_matrix_set (matrix, r, c, (r == c) ? (1) : (0));
				}
		}

			/* Operacje elementarne */

		void
		sl_matrix_ero_switch (SlPointer matrix, int row_a, int row_b) {
			SlFloat		tmp;
			int			i;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (sl_assert (row_a != row_b && row_a > 0 && row_a <= this->rows && row_b > 0 && row_b <= this->rows))
							for (i = 1; i <= this->columns; i++) {
								tmp = sl_matrix_get (this, row_a, i);
								sl_matrix_set (this, row_a, i, sl_matrix_get (this, row_b, i));
								sl_matrix_set (this, row_b, i, tmp);
							}
					}
		}


		void
		sl_matrix_ero_multiply (SlPointer matrix, int row, SlFloat k) {
			int		i;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (sl_assert (row > 0 && row <= this->rows) && k != 1)
							for (i = 1; i <= this->columns; i++)
								sl_matrix_set (matrix, row, i, k * sl_matrix_get (matrix, row, i));
				}
		}


		void
		sl_matrix_ero_add (SlPointer matrix, int row_a, int row_b, SlFloat k) {
			int		i;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (k != 0 && row_a > 0 && row_a <= this->rows && row_b > 0 && row_b <= this->rows)
							for (i = 1; i <= this->columns; i++)
								sl_matrix_set (matrix, row_b, i, k * sl_matrix_get (matrix, row_a, i) + sl_matrix_get (matrix, row_b, i));
				}
		}


		void
		sl_matrix_eco_switch (SlPointer matrix, int col_a, int col_b) {
			SlFloat		tmp;
			int			i;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (col_a != col_b && col_a > 0 && col_a <= this->columns && col_b > 0 && col_b <= this->columns)
							for (i = 1; i <= this->rows; i++) {
								tmp = sl_matrix_get (matrix, i, col_a);
								sl_matrix_set (matrix, i, col_a, sl_matrix_get (matrix, i, col_b));
								sl_matrix_set (matrix, i, col_b, tmp);
							}
				}
		}


		void
		sl_matrix_delete_row (SlPointer matrix, int r) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (sl_assert (r > 0 && r <= this->rows)) {
								if (r != this->rows)
									memcpy (this->elements + (r - 1) * this->columns, this->elements + r * this->columns, this->columns * (this->rows - r) * sizeof (SlFloat));
							sl_matrix_resize (this, this->rows - 1, this->columns);
						}
				}
		}


		void
		sl_matrix_delete_column (SlPointer matrix, int c) {
			int		i, j;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

						if (sl_assert (c > 0 && c <= this->columns)) {
								if (c != this->columns)
										for (i = 1; i <= this->rows; i++)
											for (j = c + 1; j <= this->columns; j++)
												sl_matrix_set (this, i, j - 1, sl_matrix_get (this, i, j));
							sl_matrix_resize (this, this->rows, this->columns - 1);
						}
				}
		}



			/* Arytmetyka */

		SlBool
		sl_matrix_can_multiply (SlPointer multiplicant, SlPointer multiplier) {
			int			w, h;
			SlBool		result = false;

				if (sl_assert (sl_object_is (multiplicant, SL_MATRIX_TYPE) && sl_object_is (multiplier, SL_MATRIX_TYPE))) {
					w = sl_matrix_width (multiplicant);
					h = sl_matrix_height (multiplier);
					result = w && (w == h);
				}
			return result;
		}


		SlMatrix
		sl_matrix_multiply (SlPointer multiplicant, SlPointer multiplier) {
			SlMatrix		result = NULL;
			int			m, n, k;
			int			r, c, i;
			SlFloat		v;

				if (sl_assert (sl_object_is (multiplicant, SL_MATRIX_TYPE) && sl_object_is (multiplier, SL_MATRIX_TYPE) && sl_matrix_can_multiply (multiplicant, multiplier))) {
					m = sl_matrix_height (multiplicant);
					n = sl_matrix_width (multiplier);
					k = sl_matrix_height(multiplier);
					result = sl_matrix_new ();
					sl_matrix_resize (result, m, n);
						for (r = 1; r <= m; r++)
							for (c = 1; c <= n; c++) {
								v = 0;
									for (i = 1; i <= k; i++)
										v += sl_matrix_get (multiplicant, r, i) * sl_matrix_get (multiplier, i, c);
								sl_matrix_set (result, r, c, v);
							}
				}
			return result;
		}

			/* Obliczenia */

		SlBool
		sl_matrix_is_square (SlPointer matrix) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					return (this->rows > 0 && this->rows == this->columns);
				} else
					return false;
		}


		SlBool
		sl_matrix_is_invertible (SlPointer matrix) {
				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					return (this->rows > 0 && this->columns > 0 && sl_matrix_determinant (this) != 0);
				} else
					return false;
		}


		void
		sl_matrix_print (SlPointer matrix, SlString name) {
			int		r, c;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE))) {
					sl_create_context (SlMatrix, matrix);

					printf ("%s = [\n", name);
						for (r = 1; r <= sl_matrix_height (this); r++) {
								for (c = 1; c <= sl_matrix_width (this); c++) {
									printf ("%10.6f", sl_matrix_get (this, r, c));
										if (c < sl_matrix_width (this))
											printf (",");
								}
								if (r < sl_matrix_height (this))
									printf (";");
							printf ("\n");
						}
					printf ("]\n");
				}
		}


		SlFloat /* Algorytm Bareissa, http://en.wikipedia.org/wiki/Bareiss_Algorithm */
		sl_matrix_determinant (SlPointer matrix) {
			int				i, j, k;
			SlBool			singular = false;
			SlMatrix		t;
			SlFloat			result = 0;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE) && sl_matrix_is_square (matrix))) {
					t = sl_matrix_clone (matrix);
							/* Dla k = 1 (zamiana kolumn nigdy nie jest potrzebna): */
						for (i = 2; i <= t->rows; i++)
							for (j = 2; j <= t->rows; j++)
								sl_matrix_set (t, i, j,
									sl_matrix_get (t, i, j) * sl_matrix_get (t, 1, 1) -
									sl_matrix_get (t, i, 1) * sl_matrix_get (t, 1, j)
								);
							/* Dla k > 1: */
						for (k = 2; !singular && k <= t->rows; k++) {
								/* Zamiana kolumn, jeśli potrzebna */
							if (!sl_matrix_get (t, k - 1, k - 1)) {
								for (i = k; i <= t->rows && !sl_matrix_get (t, k - 1, i); i++);
								if (i <= t->rows)
									sl_matrix_eco_switch (t, k - 1, i);
								else
									singular = true;
							}
								/* Właściwe obliczenia */
							if (!singular)
								for (i = k + 1; i <= t->rows; i++)
									for (j = k + 1; j <= t->rows; j++)
										sl_matrix_set (t, i, j,
											(sl_matrix_get (t, i, j) * sl_matrix_get (t, k, k) -
											sl_matrix_get (t, i, k) * sl_matrix_get (t, k, j)) /
											sl_matrix_get (t, k - 1, k - 1)
										);
						}
						if (singular)
							result = 0;
						else
							result = sl_matrix_get (t, t->rows, t->rows);
					sl_object_dispose (&t);
				}
			return result;
		}


		SlBool /* Najprostsza eliminacja Gaussa-Jordana */
		sl_matrix_invert (SlPointer matrix) {
			int				i, j;
			SlBool			singular = false;
			SlFloat			k;
			SlFloat			max_v, tmp;
			int				max_j;
			SlMatrix		r = NULL;
			SlMatrix		t = NULL;

				if (sl_assert (sl_object_is (matrix, SL_MATRIX_TYPE) && sl_matrix_is_square (matrix))) {
					t = sl_matrix_clone (matrix);
					r = sl_matrix_new_identity (t->rows);
						for (i = 1; !singular && i <= t->rows; i++) {
								/* Zamiana wierszy, jeśli potrzebna
							if (!sl_matrix_get (t, i, i)) {
									for (j = i + 1; j <= t->rows && !sl_matrix_get (t, j, i); j++);
									if (j <= t->rows) {
										sl_matrix_ero_switch (t, i, j);
										sl_matrix_ero_switch (r, i, j);
									} else
										singular = true;
							} */
							max_v = fabs (sl_matrix_get (t, i, i));
							max_j = i;
								for (j = i + 1; max_v != 1 && j <= t->rows; j++)
									if ((tmp = fabs (sl_matrix_get (t, j, i))) > max_v) {
										max_v = tmp;
										max_j = j;
									}
								if (max_j != i) {
									sl_matrix_ero_switch (t, i, max_j);
									sl_matrix_ero_switch (r, i, max_j);
								}
							singular |= (sl_matrix_get (t, i, i) == 0);
									/* Właściwa eliminacja */
								if (!singular) {
									k = 1 / sl_matrix_get (t, i, i);
									sl_matrix_ero_multiply (t, i, k);
									sl_matrix_ero_multiply (r, i, k);
										for (j = 1; j <= t->columns; j++)
											if (j != i) {
												k = - sl_matrix_get (t, j, i);
												sl_matrix_ero_add (t, i, j, k);
												sl_matrix_ero_add (r, i, j, k);
											}
								}
						}
						if (!singular)
							sl_matrix_swap (matrix, r);
					sl_object_dispose (&r);
					sl_object_dispose (&t);
				} else
					singular = true;
			return !singular;
		}

