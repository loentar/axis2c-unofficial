TESTS = test_addr
check_PROGRAMS = test_addr
noinst_PROGRAMS = test_addr
SUBDIRS =
test_addr_SOURCES = test_addr.c

test_addr_LDADD   =  \
                    ../../../util/src/libaxutil.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
		            $(top_builddir)/neethi/src/libneethi.la \
					$(top_builddir)/src/core/engine/libaxis2_engine.la

INCLUDES = -I${CUTEST_HOME}/include \
            -I$(top_builddir)/src/xml/guththila \
            -I$(top_builddir)/include \
            -I ../../../util/include \
            -I ../../../axiom/include

