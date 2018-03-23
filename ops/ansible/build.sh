#!/usr/bin/env bash

set -e
set -x

rm -rf  ../../CMakeFiles
rm -rf  ../../router/CMakeFiles

cmake --build ../../