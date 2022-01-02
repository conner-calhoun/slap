#!/usr/bin/env bash

TL=$(dirname $(readlink -f $BASH_SOURCE))/..
cd $TL

scripts/winbuild.sh

cd build/Debug && ./Slap.exe