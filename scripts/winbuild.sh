#!/usr/bin/env bash

TL=$(dirname $(readlink -f $BASH_SOURCE))/..
cd $TL

if [[ ! -d build ]]; then
    mkdir build
fi

cd build
cmake -G "Visual Studio 17 2022" ..

MSBuild.exe *.sln
