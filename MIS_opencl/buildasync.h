#!/bin/bash

echo "Removing previous MIS_async.o"
if [ -f MIS_async.o ] ; then rm MIS_async.o ; fi
echo "g++ -o MIS_async.o -DAMD_OS_LINUX -c MIS_async.cpp -I$AMDAPPSDKROOT/include"
g++ -o MIS_async.o -DAMD_OS_LINUX -c MIS_async.cpp -I$AMDAPPSDKROOT/include -I/opt/AMDAPPSDK-2.9-1/include

echo "Removing previous executable MIS_async" 
if [ -f MIS_async ] ; then rm MIS_async ; fi
echo "g++ -o MIS_async MIS_async.o -lOpenCL -L$AMDAPPSDKROOT/lib/x86_64"
g++ -o MIS_async MIS_async.o -lOpenCL -L$AMDAPPSDKROOT/lib/x86_64 -I/opt/AMDAPPSDK-2.9-1/include
