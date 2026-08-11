#!/bin/bash
set -e

cmake --build build

N=${1:-10000000}

for WIDTH in 2000 5000 10000 20000 50000 100000
do
    echo
    echo "========================================"
    echo "N=$N WIDTH=$WIDTH"
    echo "========================================"

    ./build/test/cms_test "$N" "$WIDTH"
done