#!/bin/bash
set -e

N=${1:-100000}
W=${2:-2500}
DISTRIBUTION=${3:-uniform}

cmake --build build

echo
echo "Running CMS test with N=$N WIDTH=$W DISTRIBUTION=$DISTRIBUTION"
echo

./build/test/cms_test "$N" "$W" "$DISTRIBUTION"