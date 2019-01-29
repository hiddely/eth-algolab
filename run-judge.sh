#!/bin/bash

set -e

# For colors, add
export COLOR_PASS=$'\x1b[32m'
export COLOR_FAIL=$'\x1b[31m'
export COLOR_END=$'\x1b[0m'

if [ $# = 0 ]; then
        echo "Please specify file to test."
        return
fi

cd $1

# Compile the cpp file to an executable
filename="main.cpp"
executable=${filename%.*}

compile="g++ $filename -o $executable -std=c++11 -lCGAL -lmpfr -lgmp -lCGAL_Core -lboost_thread -lboost_system -frounding-math -I /usr/include/boost/ -Wall -g -O3"
printf "compiling using $compile "
compile=$compile" > comp_out.txt"
eval ${compile}

#sleep 5

# Check if compilation was successfull
if [ $? -ne 0 ]; then
        printf "[ %sERROR%s ]\n" "$COLOR_FAIL" "$COLOR_END"
        cat comp_out.txt
        rm comp_out.txt
        exit 1
else
        printf "\t[ %sCOMPILATION OK%s ]\n" "$COLOR_PASS" "$COLOR_END"
        rm comp_out.txt
fi

ls -al

# Test the file against test cases
# Check for test cases
printf "Retrieving test cases...\n"
if [ ! -d testsets/ ]; then
        printf "[ %sERROR%s ] No testsets folder found.\nEnd of the testing process.\n" "$fail_colour" "$colour_end"
        exit 1
fi

# test in this case will be smth like testsets/test[1...n]
for test in testsets/*; do
        extension=${test##*.}

        if [ $extension = "in" ]; then
                test_in=${test##*/} # will remove testsets/
                test_no_ext=${test%.*}
                test_out=$test_no_ext".out"

                printf "Testing %s..." "$test_in"

                # Execute the code
                result="./$executable < $test > $test_in.out"
                eval ${result}

                # Compute the difference between the given output and the computed one
                difference="diff $test_in.out $test_out > $test_in.diff"
                eval ${difference}

                # If the file exists and is non-empty
                if [ -s $test_in".diff" ]; then
                        printf "\t[ %sERROR%s ]\n" "$COLOR_FAIL" "$COLOR_END"
                        printf "< : Output\n> : Expected\n"
                        print_diff="cat $test_in.diff"
                        eval ${print_diff}
                else
                        printf "\t[ %sOK%s ]\n" "$COLOR_PASS" "$COLOR_END"
                fi

                rm $test_in".diff"
                rm $test_in".out"
        fi
done

exit 0