#!/usr/bin/env bash

# set -euo pipefail

mkdir -p run_tree/bin
mkdir -p travis_build && cd travis_build

cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug -- -j $(nproc)
ctest -j $(nproc) --output-on-failure
