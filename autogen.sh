#!/bin/sh

mkdir -p config

autoreconf --force --install -I config -I m4
