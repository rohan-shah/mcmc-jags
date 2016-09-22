noinst_LTLIBRARIES = camd.la

camd_la_SOURCES = \
Source/camd_1.c \
Source/camd_2.c \
Source/camd_aat.c \
Source/camd_control.c \
Source/camd_defaults.c \
Source/camd_dump.c \
Source/camd_global.c \
Source/camd_info.c \
Source/camd_order.c \
Source/camd_postorder.c \
Source/camd_preprocess.c \
Source/camd_valid.c

camd_la_CPPFLAGS = -I$(top_srcdir)/src/modules/glm/SSparse/config \
-I$(top_srcdir)/src/modules/glm/SSparse/CAMD/Include

noinst_HEADERS = \
Include/camd.h \
Include/camd_internal.h

camd_la_LDFLAGS = -no-undefined -module -avoid-version

DIST_EXTRA = README.txt
