#!/usr/bin/env bash

set -e
set -x

rm -rf  ../../CMakeFiles
rm -rf  ../../router/CMakeFiles

cd ../../ && cmake . && cd -
cmake --build ../../

set +x