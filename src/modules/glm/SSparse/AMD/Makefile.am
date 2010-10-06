noinst_LTLIBRARIES = amd.la

amd_la_SOURCES = \
Source/amd_1.c \
Source/amdbar.f \
Source/amd_dump.c \
Source/amd_info.c \
Source/amd_post_tree.c \
Source/amd_2.c \
Source/amd_control.c \
Source/amd.f \
Source/amd_order.c \
Source/amd_preprocess.c \
Source/amd_aat.c \
Source/amd_defaults.c \
Source/amd_global.c \
Source/amd_postorder.c \
Source/amd_valid.c

amd_la_CPPFLAGS = -I$(top_srcdir)/src/modules/glm/SSparse/UFconfig \
-I$(top_srcdir)/src/modules/glm/SSparse/AMD/Include

noinst_HEADERS = \
Include/amd.h \
Include/amd_internal.h

amd_la_LDFLAGS = -no-undefined -module -avoid-version

DIST_EXTRA = README.txt