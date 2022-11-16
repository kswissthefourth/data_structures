#!/bin/bash

rm -rf bin
mkdir -p bin
rm -rf build
mkdir -p build
cd build
cmake ..
make
