#!/bin/bash
set -e

cmake --build build

N=${1:-10000000} # Default N=10,000,000
D=${2:-uniform}

printf "\nN: %s\n" "$N"
printf "Distribution: %s\n" "$D"
printf "\n%-10s %-12s %-12s\n" "Width" "Exact" "Avg Error"
printf "%s\n" "-------------------------------------"

for WIDTH in 500 2000 5000 10000 20000 50000 100000
do
    OUTPUT=$(./build/test/cms_test "$N" "$WIDTH" "$D")

    EXACT=$(echo "$OUTPUT" | awk '/^Exact[[:space:]]*:/ {
        gsub(/[()%]/, "", $4)
        print $4
    }')

    AVG_ERROR=$(echo "$OUTPUT" | awk '/^Average error[[:space:]]*:/ {
        print $4
    }')

    printf "%-10s %-12s %-12s\n" \
        "$WIDTH" \
        "${EXACT}%" \
        "$AVG_ERROR"
done