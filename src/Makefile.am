AUTOMAKE_OPTIONS = gnu
pkglib_LTLIBRARIES = libslice.la
pkglibdir = /usr/lib
AM_CFLAGS = --std=gnu99 @CFLAGS@
libslice_la_SOURCES = sl_types.c sl_table.c sl_table_2d.c sl_map.c sl_map_si.c sl_matrix.c sl_lelement.c sl_nlelement.c sl_circuit.c \
	sl_assert_internal.h sl_types_internal.h sl_table_internal.h sl_table_2d_internal.h sl_map_internal.h sl_map_si_internal.h sl_matrix_internal.h \
	sl_lelement_internal.h sl_nlelement_internal.h sl_circuit_internal.h \
	slice.h slice_internal.h
libslice_la_LDFLAGS = -module -avoid-version
libslice_ladir = $(includedir)/slice
libslice_la_HEADERS = slice.h sl_assert.h sl_types.h sl_table.h sl_table_2d.h sl_map.h sl_map_si.h sl_matrix.h sl_lelement.h sl_nlelement.h sl_circuit.h
