prglibdir=$(prefix)/services/version
prglib_LTLIBRARIES = libversion.la
prglib_DATA=services.xml 
noinst_HEADERS = version.h
SUBDIRS =
libversion_la_SOURCES = version.c version_skel.c
libversion_la_LIBADD  = -L$(AXIS2C_HOME)/lib -laxis2_axiom -laxis2_engine -laxutil
INCLUDES = @AXIS2INC@
EXTRA_DIST = services.xml version.h

