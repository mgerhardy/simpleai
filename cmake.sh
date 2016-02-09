#!/bin/sh

set -e
#set -x
DIR=$(dirname $(readlink -f $0))

JOBS=4

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

cd $DIR/..
mkdir -p simpleai-build
cd simpleai-build
cmake -DSIMPLEAI_DEBUGGER=ON -DCMAKE_BUILD_TYPE=Debug $DIR $*
make -j ${JOBS} $*
