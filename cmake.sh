#!/bin/sh

set -e
#set -x
DIR=$(dirname $(readlink -f $0))

JOBS=4

cd $DIR/..
mkdir -p simpleai-build
cd simpleai-build
cmake -DSIMPLEAI_DEBUGGER=ON -DCMAKE_BUILD_TYPE=Release $DIR $*
make -j ${JOBS} $*
