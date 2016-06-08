#!/bin/bash

set -e
set -x

clang --version
gcc --version
cmake --version
lsb_release -a

DEBUG="yes"
if [ "$BUILD_MODE" == "Release" ]; then
	DEBUG="no"
fi

CONFIGURE_PARAM="--enable-debug=$DEBUG --prefix=."

if [ "$BUILD_TYPE" == "automake" ]; then
	./autogen.sh
	./configure $CONFIGURE_PARAM --enable-tests=yes
	make
	make install
	./src/test/simpleai-tests
	make dist-clean
	./configure $CONFIGURE_PARAM --enable-tests=yes --enable-run=yes
	make
	make dist-clean
	./configure $CONFIGURE_PARAM --enable-tests=yes
	make
	make dist-clean
	./configure $CONFIGURE_PARAM --enable-run=yes
	make
	make dist-clean
	./configure $CONFIGURE_PARAM --enable-lua=no
	make
	make dist-clean
	./configure $CONFIGURE_PARAM --enable-lua=no --enable-tests=yes
	make
	make dist-clean
	./configure $CONFIGURE_PARAM --enable-gprof
	make
	make dist-clean
	cd src/debug
	qmake
	make
fi
