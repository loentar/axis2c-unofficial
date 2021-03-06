#!/bin/bash -e
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
echo 'Running autogen.sh in Util'
cd util
./autogen.sh

echo 'Running autogen.sh in Axiom'
cd ../axiom
./autogen.sh

echo 'Running autogen.sh in Guththila'
cd ../guththila
./autogen.sh

echo 'Running autogen.sh in Neethi'
cd ../neethi
./autogen.sh

echo 'Running autogen.sh in Axis2/C'
cd ..

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


if [ `sed --version 2>/dev/null | grep -c GNU` -eq 0 ]
then
  if [ -n "`which gsed`" ]
  then
    function sedi() { gsed -i "$@"; }
  else
    function sedi() { sed -i "" "$@"; }
  fi
else
  function sedi() { sed -i "$@"; }
fi

# patch config scripts to make MinGW libraries installed into original (not the ../bin) place
# set dll names as native for windows - without version and "lib" prefix.
replacer="\
/DLL is installed to \$(libdir)\/\.\.\/bin by postinstall_cmds/,/shlibpath_overrides_runpath=yes/d;\
/MinGW DLLs use traditional 'lib' prefix/,/;;/\
  {s:^\(.*soname_spec=\).*:\1'\`echo \$\{libname\} | \$SED -e 's/^lib//'\`\${shared_ext}'; library_names_spec=\$soname_spec:;}"

find . -type l -maxdepth 1 -exec sh -c 'f=$(readlink {}); rm -f {}; cp -f $f {}' \; 2>/dev/null

for f in configure aclocal.m4; do
  sedi "$replacer" $f
done

# patch to link shared library against static lib
sedi '/# Not a shared library/{N;s/test.*;/false;/;};/tdlname=\.\.\/bin\/\$dlname/s:../bin/::' ltmain.sh

# touching files to prevent re-configuring
for f in aclocal.m4 config.h.in configure config.guess config.sub depcomp install-sh missing Makefile.in; do
  touch $f 2>/dev/null
done

echo 'done'
