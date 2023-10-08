#!/bin/sh
cmake -S . -B ./build
cmake --build ./build
./build/bin/UNITTEST
./build/bin/PERFROMTEST
./build/bin/CONCURRENTTEST