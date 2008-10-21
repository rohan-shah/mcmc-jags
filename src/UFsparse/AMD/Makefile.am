noinst_LTLIBRARIES = libamd.la

libamd_la_CPPFLAGS = -I$(top_srcdir)/src/include/UFsparse -DDINT

libamd_la_LDFLAGS = -no-undefined

libamd_la_SOURCES = amd_aat.c amd_1.c amd_2.c amd_dump.c		\
		amd_postorder.c amd_post_tree.c amd_defaults.c		\
		amd_order.c amd_control.c amd_info.c amd_valid.c	\
		amd_preprocess.c amd_global.c

noinst_HEADERS = amd_internal.h
