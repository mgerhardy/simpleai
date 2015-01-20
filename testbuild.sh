#!/bin/sh

set -e
#set -x

JOBS=4

echo "" > testbuild.log

run_configure() {
	echo "===========================START========================"
	echo "configure $*"
	echo "clean"
	git clean -fdx >> testbuild.log 2>&1
	set +e
	echo "autogen"
	./autogen.sh >> testbuild.log 2>&1
	set -e
	echo "configure"
	./configure $* >> testbuild.log 2>&1
	echo "make"
	make -j ${JOBS} >> testbuild.log 2>&1
	echo "============================DONE========================"
}

run_configure --enable-exceptions --enable-lua --enable-xml --enable-tests --enable-run
run_configure --disable-exceptions --enable-lua --enable-xml --enable-tests --enable-run

run_cmake() {
	echo "===========================START========================"
	echo "cmake $*"
	git clean -fdx >> testbuild.log 2>&1
	cmake CMakeLists.txt $* >> testbuild.log 2>&1
	make -j ${JOBS} >> testbuild.log 2>&1
	echo "============================DONE========================"
}

run_cmake -DSIMPLEAI_EXCEPTIONS=ON -DSIMPLEAI_LUA=ON -DSIMPLEAI_XML=ON -DSIMPLEAI_TESTS=ON -DSIMPLEAI_RUN=ON
run_cmake -DSIMPLEAI_EXCEPTIONS=OFF -DSIMPLEAI_LUA=ON -DSIMPLEAI_XML=ON -DSIMPLEAI_TESTS=ON -DSIMPLEAI_RUN=ON
