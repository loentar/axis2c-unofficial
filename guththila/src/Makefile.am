lib_LTLIBRARIES = libguththila.la

libguththila_la_LDFLAGS = $(VERSION_INFO)

libguththila_la_SOURCES = guththila_buffer.c \
			  guththila_namespace.c \			  
	 		  guththila_token.c \
			  guththila_reader.c \
			  guththila_attribute.c \
			  guththila_xml_parser.c \
			  guththila_stack.c \
			  guththila_xml_writer.c \
			  guththila_utf8.c

libguththila_la_LIBADD = ../../util/src/libaxutil.la

INCLUDES = -I$(top_builddir)/include \
			-I ../../util/include

