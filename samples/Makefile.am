SUBDIRS = server client user_guide mtom_caching_callback mtom_sending_callback

dist-hook:
	rm -rf `find $(distdir)/ -type d -name .svn`
	rm -rf `find $(distdir)/ -type d -name .deps`
	rm -rf `find $(distdir)/ -type d -name .libs`

EXTRA_DIST=codegen autogen.sh build.sh

install-data-hook:
	for d in $(prefix)/services/* $(prefix)/samples/lib/*; do rm -f $$d/*.a $$d/*.la; done
