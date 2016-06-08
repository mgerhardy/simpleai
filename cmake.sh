#!/bin/sh

set -e
#set -x
DIR=$(dirname $(readlink -f $0))

JOBS=4

#export CC=/usr/bin/clang
#export CXX=/usr/bin/clang++

cd $DIR/..
mkdir -p simpleai-build-release
cd simpleai-build-release
cmake -DSIMPLEAI_DEBUGGER=ON -DCMAKE_BUILD_TYPE=Release -G"Eclipse CDT4 - Unix Makefiles" $DIR
make -j ${JOBS} $*

cd $DIR/..
mkdir -p simpleai-build-debug
cd simpleai-build-debug
cmake -DSIMPLEAI_DEBUGGER=ON -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles" $DIR
make -j ${JOBS} $*

cd $DIR/..
mkdir -p simpleai-build-clang-analyser
cd simpleai-build-clang-analyser
scan-build-3.9 cmake -DSIMPLEAI_DEBUGGER=ON -DSIMPLEAI_USE_CCACHE=OFF $DIR
scan-build-3.9 make -j ${JOBS} $*
