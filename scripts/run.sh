#!/usr/bin/env bash

TL=$(dirname $(readlink -f $BASH_SOURCE))/..
cd $TL

cd build/Debug && ./Slap.exe