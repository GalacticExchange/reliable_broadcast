#!/usr/bin/env bash

set -e
set -x

cmake --build ./cmake/

./cmake/router