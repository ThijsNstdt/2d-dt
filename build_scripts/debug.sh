#! /usr/bin/bash
cd ..
cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B cmake-build-debug
cmake --build ./cmake-build-debug --target 2d_dt -j 6