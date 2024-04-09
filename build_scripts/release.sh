#! /usr/bin/bash
cd ..
cmake -DCMAKE_BUILD_TYPE=Release -S . -B cmake-build-release
cmake --build ./cmake-build-release --target 2d_dt -j 6