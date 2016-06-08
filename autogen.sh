#!/bin/sh

mkdir -p config

#libtoolize --force
#aclocal
#autoheader
#automake --force-missing --add-missing
#autoconf
autoreconf --force --install -I config -I m4
