#!/bin/bash

export CC=clang
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=$CC ..
make
cd ..
