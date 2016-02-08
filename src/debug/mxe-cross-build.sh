#!/bin/bash
MXE_PATH=${1:-~/dev/mxe}
TARGET=${2:-i686-w64-mingw32.static}
JOBS=${3:-4}

PATH="${MXE_PATH}"/usr/bin/:$PATH QMAKE_CXX="ccache g++" ${MXE_PATH}/usr/${TARGET}/qt5/bin/qmake
PATH="${MXE_PATH}"/usr/bin/:$PATH make clean && make -j ${JOBS}
