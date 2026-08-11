#!/bin/bash
set -e

N=${1:-100000}

cmake --build build

echo
echo "Running CMS test with N=$N"
echo

./build/test/cms_test "$N"