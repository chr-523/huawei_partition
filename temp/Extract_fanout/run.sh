#!/bin/bash
cd ./test/benchmarks
file_list=$(find $(pwd) -type f -name "*.v" -printf "%p\n")

cd ..
cd ..
current_dir=$(pwd)
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd test
for file in $file_list
do
    time ./test_bison $file 
done
#current_dir=$(pwd)


