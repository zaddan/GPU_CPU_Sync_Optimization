#!/bin/bash

#   For this test case we need libbsd-dev for the random number generator
#   sudo apt-get install libbsd-dev
#
#  Set HSA Environment variables
[ -z $HSA_RUNTIME_PATH ] && HSA_RUNTIME_PATH=/opt/hsa
[ -z HSA_LIBHSAIL_PATH ] && HSA_LIBHSAIL_PATH=/opt/hsa/lib
[ -z HSA_LLVM_PATH ] && HSA_LLVM_PATH=/opt/amd/bin
export LD_LIBRARY_PATH=$HSA_RUNTIME_PATH/lib
echo $1
#setting up the compiler 
#cxx=colorgcc
cxx=g++

# Compile accelerated functions
echo 
if [ -f MIS_parallel.o ] ; then rm MIS_parallel.o ; fi
echo snack.sh -c -opt 3 -vv  MIS_parallel.cl 
snack.sh -q -c MIS_parallel.cl  

# Compile Main .c  and link to accelerated functions in matmulKernels.o
echo 
if [ -f MIS_parallel ] ; then rm MIS_parallel ; fi

#running the synchronux version
#echo $cxx -O3 -o MIS_parallel MIS_parallel.o MIS_parallel.cpp  debug_helpers.cpp -L$HSA_RUNTIME_PATH/lib -lhsa-runtime64 -lelf
$cxx -O3 -o MIS_parallel MIS_parallel.o MIS_parallel.cpp  debug_helpers.cpp -L$HSA_RUNTIME_PATH/lib -lhsa-runtime64 -lelf

#running the asynchrnous version
#echo $cxx -O3 -o MIS_parallel MIS_parallel.o MIS_parallel_async.cpp debug_helpers.cpp -L$HSA_RUNTIME_PATH/lib -lhsa-runtime64 -lelf
#$cxx -O3 -o MIS_parallel MIS_parallel.o MIS_parallel_async.cpp  debug_helpers.cpp -L$HSA_RUNTIME_PATH/lib -lhsa-runtime64 -lelf

#  Make sure parci
#./MIS_parallel
#./matmul 2000 2000 2000
