#!/bin/sh

set -e
#set -x

JOBS=4

TESTLOG="../testbuild.log"
echo "Write logs to ${TESTLOG}"
echo "" > ${TESTLOG}

run_configure() {
	echo "===========================START========================"
	echo "configure $*"
	echo "clean"
	git clean -fdx >> ${TESTLOG} 2>&1
	set +e
	echo -n "autogen: "
	./autogen.sh >> ${TESTLOG} 2>&1
	[ $? -ne 0 ] && echo "Failed" || echo "Success"
	set -e
	echo -n "configure: "
	./configure $* >> ${TESTLOG} 2>&1
	[ $? -ne 0 ] && echo "Failed" || echo "Success"
	echo -n "make: "
	make -j ${JOBS} >> ${TESTLOG} 2>&1
	[ $? -ne 0 ] && echo "Failed" || echo "Success"
	echo "============================DONE========================"
}

run_configure --enable-exceptions --enable-lua --enable-xml --enable-tests --enable-run
run_configure --disable-exceptions --enable-lua --enable-xml --enable-tests --enable-run
run_configure --disable-lua --enable-xml --enable-tests --enable-run
run_configure --enable-lua --disable-xml --enable-tests --enable-run
run_configure --disable-lua --disable-xml --enable-tests --enable-run
run_configure --disable-tests
run_configure --disable-run

run_cmake() {
	echo "===========================START========================"
	echo -n "cmake $*: "
	git clean -fdx >> ${TESTLOG} 2>&1
	cmake CMakeLists.txt $* >> ${TESTLOG} 2>&1
	[ $? -ne 0 ] && echo "Failed" || echo "Success"
	echo -n "make: "
	make -j ${JOBS} >> ${TESTLOG} 2>&1
	[ $? -ne 0 ] && echo "Failed" || echo "Success"
	echo "============================DONE========================"
}

run_cmake -DSIMPLEAI_EXCEPTIONS=ON -DSIMPLEAI_LUA=ON -DSIMPLEAI_XML=ON -DSIMPLEAI_TEST=ON -DSIMPLEAI_RUN=ON
run_cmake -DSIMPLEAI_EXCEPTIONS=OFF -DSIMPLEAI_LUA=ON -DSIMPLEAI_XML=ON -DSIMPLEAI_TEST=ON -DSIMPLEAI_RUN=ON
run_cmake -DSIMPLEAI_EXCEPTIONS=OFF -DSIMPLEAI_LUA=OFF -DSIMPLEAI_XML=ON -DSIMPLEAI_TEST=ON -DSIMPLEAI_RUN=ON
run_cmake -DSIMPLEAI_EXCEPTIONS=OFF -DSIMPLEAI_LUA=OFF -DSIMPLEAI_XML=OFF -DSIMPLEAI_TEST=OFF -DSIMPLEAI_RUN=OFF
