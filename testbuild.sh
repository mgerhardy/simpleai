#!/bin/sh

set -e
#set -x

JOBS=4

echo "" > testbuild.log

run_configure() {
	echo "===========================START========================"
	echo "configure $*"
	echo "clean"
	git clean -fdx 2>1 >> testbuild.log
	set +e
	echo "autogen"
	./autogen.sh 2>1 >> testbuild.log
	set -e
	echo "configure"
	./configure $* 2>&1 >> testbuild.log
	echo "make"
	make -j ${JOBS} 2>&1 >> testbuild.log
	echo "============================DONE========================"
}

run_configure --enable-exceptions --enable-lua --enable-xml --enable-tests --enable-run
run_configure --disable-exceptions --enable-lua --enable-xml --enable-tests --enable-run

run_cmake() {
	echo "===========================START========================"
	echo "cmake $*"
	git clean -fdx 2>&1 >> testbuild.log
	cmake CMakeLists.txt $* 2>&1 >> testbuild.log
	make -j ${JOBS} 2>&1 >> testbuild.log
	echo "============================DONE========================"
}

run_cmake -DSIMPLEAI_EXCEPTIONS=ON -DSIMPLEAI_LUA=ON -DSIMPLEAI_XML=ON -DSIMPLEAI_TESTS=ON -DSIMPLEAI_RUN=ON
run_cmake -DSIMPLEAI_EXCEPTIONS=OFF -DSIMPLEAI_LUA=ON -DSIMPLEAI_XML=ON -DSIMPLEAI_TESTS=ON -DSIMPLEAI_RUN=ON
