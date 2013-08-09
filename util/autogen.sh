#!/bin/bash

echo -n 'Running libtoolize...'
if [ `uname -s` = Darwin ]
then
    LIBTOOLIZE=glibtoolize
else
    LIBTOOLIZE=libtoolize
fi

if $LIBTOOLIZE --force > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running aclocal...'
if aclocal > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running autoheader...'
if autoheader > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running autoconf...'
if autoconf > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

echo -n 'Running automake...'
if automake --add-missing > /dev/null 2>&1; then
	echo 'done.'
else
	echo 'failed.'
	exit 1
fi

# patch config scripts to make MinGW libraries installed into original (not the ../bin) place
# set dll names as native for windows - without version and "lib" prefix.
replacer="\
/DLL is installed to \$(libdir)\/\.\.\/bin by postinstall_cmds/,/shlibpath_overrides_runpath=yes/d;\
/MinGW DLLs use traditional 'lib' prefix/,/;;/\
  {s:^\(.*soname_spec=\).*:\1'\`echo \${libname} | \$SED -e 's/^lib//'\`\${shared_ext}'; library_names_spec=\$soname_spec:;}"

find . -type l -maxdepth 1 -exec sh -c 'f=$(readlink {}); rm -f {}; cp -f $f {}' \; 2>/dev/null

for f in aclocal.m4 configure; do
  sed -i~ "$replacer" $f
done

# patch to link shared library against static lib
sed -i~ '/# Not a shared library/{N;s/test.*;/false;/;};/tdlname=\.\.\/bin\/\$dlname/s:../bin/::' ltmain.sh

# touching files to prevent re-configuring
for f in aclocal.m4 config.h.in configure config.guess config.sub depcomp install-sh missing Makefile.in; do
  touch $f
done

echo 'done'
