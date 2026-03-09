#!/bin/bash

GREEN="\033[32m"
RED="\033[31m"
CYAN="\033[36m"
YELLOW="\033[33m"
BOLD="\033[1m"
RESET="\033[0m"

mkdir -p temp_file

printf "${CYAN}${BOLD}%-8s %-10s %-18s %-18s %-18s${RESET}\n" "STATUS" "SIZE" "EXPECTED" "EIGEN" "OUR" 

for size in 1 2 3 4 5 10 20 30 40 50 60 70 80 90 100; do
    expected_determinant=$(awk -v seed=$RANDOM 'BEGIN{srand(seed); printf "%.4f\n", (rand()*2000 - 1000)}')
    python3 tests/generate_matrix.py "$size" "$expected_determinant" > temp_file/test.txt
    our=$(build/Matrix < temp_file/test.txt)
    ideal=$(build/tests/IdealDeterminant < temp_file/test.txt)

    diff1=$(echo "$our - $expected_determinant" | bc -l)
    diff2=$(echo "$ideal - $expected_determinant" | bc -l)
    abs_diff1=$(echo "$diff1" | awk '{if($1<0) $1=-$1; print $1}')
    abs_diff2=$(echo "$diff2" | awk '{if($1<0) $1=-$1; print $1}')
    if (( $(echo "$abs_diff1 > 0.01" | bc -l) && $(echo "$abs_diff2 > 0.01" | bc -l) )); then
        status="${RED}[FAILED]${RESET}"
    else
        status="${GREEN}[PASSED]${RESET}"
    fi

    printf "%-10b %-10d %-18s %-18s %-18s\n" \
        "$status" "$size" "$expected_determinant" "$ideal" "$our"
done

rm -rf temp_file
