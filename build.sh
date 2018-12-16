#!/bin/bash

if [ "$1" == "run" ]; then
	cd build-debug/apps/
	./musl_test_main
	exit $?
fi


#rm -r build-debug/*
pushd build-debug
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
ninja

popd
./build.sh run
