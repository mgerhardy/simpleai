#!/bin/sh

set -e
#set -x
DIR=$(dirname $(readlink -f $0))

JOBS=4

mkdir -p ../simpleai-build
cmake -DCMAKE_BUILD_TYPE=Release $DIR/CMakeLists.txt $*
make -j ${JOBS} $*
