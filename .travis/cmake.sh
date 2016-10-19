#!/bin/bash

set -e
set -x

clang --version
gcc --version
cmake --version
lsb_release -a
pkg-config --version
doxygen --version

if [ -z "$BUILD_MODE" ]; then
	BUILD_MODE=Debug
fi

if [ "$BUILD_TYPE" == "cmake" ]; then
	mkdir -p simpleai-build-with-debugger
	cd simpleai-build-with-debugger
	cmake -DSIMPLEAI_DEBUGGER=ON -DCMAKE_BUILD_TYPE=$BUILD_MODE -G"Unix Makefiles" ..
	make
	cd ..

	mkdir -p simpleai-build-with-xml
	cd simpleai-build-with-xml
	cmake -DSIMPLEAI_XML=ON -DCMAKE_BUILD_TYPE=$BUILD_MODE -G"Unix Makefiles" ..
	make
	cd ..

	mkdir -p simpleai-build-without-lua
	cd simpleai-build-without-lua
	cmake -DSIMPLEAI_LUA=OFF -DCMAKE_BUILD_TYPE=$BUILD_MODE -G"Unix Makefiles" ..
	make
	cd ..

	mkdir -p simpleai-build-everything-disabled
	cd simpleai-build-everything-disabled
	cmake -DSIMPLEAI_LUA=OFF -DSIMPLEAI_TEST=OFF -DSIMPLEAI_RUN=OFF -DCMAKE_BUILD_TYPE=$BUILD_MODE -G"Unix Makefiles" ..
	make
	cd ..

	mkdir -p simpleai-build-exceptions
	cd simpleai-build-exceptions
	cmake -DSIMPLEAI_EXCEPTIONS=ON -DCMAKE_BUILD_TYPE=$BUILD_MODE -G"Unix Makefiles" ..
	make
	cd ..
fi
