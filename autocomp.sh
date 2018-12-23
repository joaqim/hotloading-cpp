#!/bin/bash

pkill -f "entr compiler" && exit
echo "entr compiler"
ls src/* plugin/* CMakeLists.txt cmake/* tests/* | entr -np ./build.sh
