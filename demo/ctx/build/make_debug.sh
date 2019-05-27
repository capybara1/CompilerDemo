#!/usr/bin/env bash

set -eu

cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
cd src/
echo "print (3+3)*2\n" >> testfile
