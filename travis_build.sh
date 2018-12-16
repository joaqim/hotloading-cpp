#!/bin/sh


cd $TRAVIS_BUILD_DIR
echo "Current directory" $(pwd)
set -ev

cd build-debug
cmake -D CMAKE_BUILD_TYPE=Debug ../
make

cd ../build-opt
cmake -D CMAKE_BUILD_TYPE=Release ../
make

