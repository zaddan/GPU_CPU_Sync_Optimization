#!/bin/bash

export AMDAPPSDKROOT="/opt/AMDAPPSDK-3.0-0-Beta"
export LD_LIBRARY_PATH="/opt/AMDAPPSDK-3.0-0-Beta/lib/x86_64/"

echo "Removing previous MIS_async.o"
if [ -f MIS_async.o ] ; then rm MIS_async.o ; fi
echo "g++ -o MIS_async.o -c MIS_async.cpp -I$AMDAPPSDKROOT/include"
g++ -o MIS_async.o -c MIS_async.cpp -I$AMDAPPSDKROOT/include 

echo "Removing previous executable MIS_async" 
if [ -f MIS_async ] ; then rm MIS_async ; fi
echo "g++ -o MIS_async MIS_async.o -lOpenCL -L$AMDAPPSDKROOT/lib/x86_64"
g++ -o MIS_async MIS_async.o -lOpenCL -L$AMDAPPSDKROOT/lib/x86_64
