noinst_LTLIBRARIES = baserngs.la

baserngs_la_CPPFLAGS = -I$(top_srcdir)/src/include

baserngs_la_LDFLAGS = -no-undefined -module -avoid-version

baserngs_la_SOURCES = MarsagliaRNG.cc WichmannHillRNG.cc SuperDuperRNG.cc \
MersenneTwisterRNG.cc BaseRNGFactory.cc 

noinst_HEADERS = MarsagliaRNG.h WichmannHillRNG.h SuperDuperRNG.h \
MersenneTwisterRNG.h BaseRNGFactory.h
