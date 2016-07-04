#!/bin/bash

set -e
set -x

clang --version
gcc --version
cmake --version
lsb_release -a
pkg-config --version

DEBUG="yes"
if [ "$BUILD_MODE" == "Release" ]; then
	DEBUG="no"
fi

CONFIGURE_PARAM="--enable-debug=$DEBUG --prefix=$PWD"

if [ "$BUILD_TYPE" == "automake" ]; then
	./autogen.sh
	./configure $CONFIGURE_PARAM --enable-tests=yes
	make
	make install
	./src/test/simpleai-tests
	make distclean
	./configure $CONFIGURE_PARAM --enable-tests=yes --enable-run=yes
	make
	make distclean
	./configure $CONFIGURE_PARAM --enable-tests=yes
	make
	make distclean
	./configure $CONFIGURE_PARAM --enable-run=yes
	make
	make distclean
	./configure $CONFIGURE_PARAM --enable-lua=no
	make
	make distclean
	./configure $CONFIGURE_PARAM --enable-lua=no --enable-tests=yes
	make
	make distclean
	./configure $CONFIGURE_PARAM --enable-gprof
	make
	make distclean
	cd src/debug
	qmake
	make
fi
