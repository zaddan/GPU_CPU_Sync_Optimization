#!/usr/bin/python
import os
from book_keeping import *
from generate_graph import *
from verify import *
from time import gmtime, strftime


#-------------------------------------------------------------------------------------------------------------------------------------------------------------ONLY change this
numOfNodes = 20 
numOfTests = 1
testName = "runSomeSampleTests"
#-------------------------------------------------------------------------------------------------------------------------------------------------------------
#variables
sparseRepFileName = "sparse_rep.txt" #sparse representation of the graph
MISResultToVerifyFileNameSerial = "MIS_result_serial.txt" #generated by the serail implementation written by us
MISResultToVerifyFileNameParallel = "MIS_result_parallel.txt" #generated by the parralel implementation written by us

#running ./buildrun once to make sure that we have the .h files
#os.system("./buildrun.sh")

#MISResultRefFileName = "MISResultRef.txt" #generated by the reference serial implementation found in interner
#adjacentRepFilName = "adjacent_rep.txt" #adjacent matrix representation of the graph


#------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
if (testName == "runSomeSampleTests"):
    counter  = 0
    for i in range (0, numOfTests):
        #generate a graph	
        while not(generate_random_graphs(numOfNodes, sparseRepFileName)):
            counter +=1; #this is just so that while has a body, but the point is that we need to run the generate random_graph till the result is 1

        #---------guide::: run the serial code
        os.system("g++ MIS_serial.cpp -o MIS_serial")
        os.system("./MIS_serial" + " " + sparseRepFileName + " " + MISResultToVerifyFileNameSerial); 
        #
        #---------guide:::  generate a log file and append the spares rep nad resultto it
        if not(os.path.isdir("./log")):
            os.system("mkdir log");
        timeSuffix = strftime("%M", gmtime()) + strftime("%S", gmtime()) +".txt"
        logFileName = "./log/log_" + timeSuffix
        if not(os.path.isdir("./log/failed")):
            os.system("mkdir log/failed");
        logFileNameFailedCase= "./log/failed/log_" + timeSuffix 
        #os.remove(logFileName) 
        logFilePtr = open(logFileName, "a");
        sparseRepFilePtr= open(sparseRepFileName, "r");
        logFilePtr.write("*************************sparse representation of the graph************************************\n")
        logFilePtr.write(sparseRepFilePtr.read());
        logFilePtr.write("\n");
        logFilePtr.write("\n");
        logFilePtr.write("*************************Result of the serial implemetation************************************\n")
        MISResultToVerifyFileSerialPtr = open(MISResultToVerifyFileNameSerial, "r");
        logFilePtr.write(MISResultToVerifyFileSerialPtr.read());
        #---------guide::: verify
        print "verifying the serial exection...."
        logFilePtr.write("***********************************************************************************************\n")
        logFilePtr.write("*************************Serial Test Results: *************************************************\n")
        logFilePtr.close() 
        verifyMaximalSet(sparseRepFileName,  MISResultToVerifyFileNameSerial, logFileName)
    #	
         #------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ 
        #---------guide::: run the parallel code
        os.system("./buildrun.sh")
        os.system("./MIS_parallel" + " " + sparseRepFileName + " " + MISResultToVerifyFileNameParallel + " " + logFileName); 
        print "hoooking here" 
        logFilePtr = open(logFileName, "a");
        logFilePtr.write("\n");
        logFilePtr.write("\n");
        logFilePtr.write("*************************Result of the Parralel implemetations*********************************\n")
        MISResultToVerifyFileParallelPtr = open(MISResultToVerifyFileNameParallel, "r");
        logFilePtr.write(MISResultToVerifyFileParallelPtr.read());
    #	##---------guide::: verify
        print "verifying the parallel exection...."
        logFilePtr.write("***********************************************************************************************\n")
        logFilePtr.write("*************************Parralel Test Results: ***********************************************\n")
        #logFilePtr.write("*******************************************************************************************************************\n")
        logFilePtr.close() 
        failed = verifyMaximalSet(sparseRepFileName,  MISResultToVerifyFileNameParallel, logFileName)
        if (failed):
            logFileFailedCasePtr = open(logFileNameFailedCase, "a")
            logFilePtr = open(logFileName, "r")
            logFileFailedCasePtr.write(logFilePtr.read());
            logFilePtr.close()
            logFileFailedCasePtr.close(); 


if (testName == "runTillFailure"): 
    counter  = 0
    for i in range (0, numOfTests):
        #generate a graph	
        while not(generate_random_graphs(numOfNodes, sparseRepFileName)):
            counter +=1; #this is just so that while has a body, but the point is that we need to run the generate random_graph till the result is 1

        #---------guide::: run the serial code
        os.system("g++ MIS_serial.cpp -o MIS_serial")
        os.system("./MIS_serial" + " " + sparseRepFileName + " " + MISResultToVerifyFileNameSerial); 
        #
        #---------guide:::  generate a log file and append the spares rep nad resultto it
        if not(os.path.isdir("./log")):
            os.system("mkdir log");
        timeSuffix = strftime("%M", gmtime()) + strftime("%S", gmtime()) +".txt"
        logFileName = "./log/log_" + timeSuffix
        if not(os.path.isdir("./log/failed")):
            os.system("mkdir log/failed");
        logFileNameFailedCase= "./log/failed/log_" + timeSuffix 
        #os.remove(logFileName) 
        logFilePtr = open(logFileName, "a");
        sparseRepFilePtr= open(sparseRepFileName, "r");
        logFilePtr.write("*************************sparse representation of the graph************************************\n")
        logFilePtr.write(sparseRepFilePtr.read());
        logFilePtr.write("\n");
        logFilePtr.write("\n");
        logFilePtr.write("*************************Result of the serial implemetation************************************\n")
        MISResultToVerifyFileSerialPtr = open(MISResultToVerifyFileNameSerial, "r");
        logFilePtr.write(MISResultToVerifyFileSerialPtr.read());
        #---------guide::: verify
        print "verifying the serial exection...."
        logFilePtr.write("***********************************************************************************************\n")
        logFilePtr.write("*************************Serial Test Results: *************************************************\n")
        logFilePtr.close() 
        verifyMaximalSet(sparseRepFileName,  MISResultToVerifyFileNameSerial, logFileName)
    #	#---------guide::: run the parallel code
        os.system("./buildrun.sh")
        os.system("./MIS_parallel" + " " + sparseRepFileName + " " + MISResultToVerifyFileNameParallel + " " + logFileName); 
        logFilePtr = open(logFileName, "a");
        logFilePtr.write("\n");
        logFilePtr.write("\n");
        logFilePtr.write("*************************Result of the Parralel implemetations*********************************\n")
        MISResultToVerifyFileParallelPtr = open(MISResultToVerifyFileNameParallel, "r");
        logFilePtr.write(MISResultToVerifyFileParallelPtr.read());
    #	##---------guide::: verify
        print "verifying the parallel exection...."
        logFilePtr.write("***********************************************************************************************\n")
        logFilePtr.write("*************************Parralel Test Results: ***********************************************\n")
        #logFilePtr.write("*******************************************************************************************************************\n")
        logFilePtr.close() 
        failed = verifyMaximalSet(sparseRepFileName,  MISResultToVerifyFileNameParallel, logFileName)
        if (failed):
            logFileFailedCasePtr = open(logFileNameFailedCase, "a")
            logFilePtr = open(logFileName, "r")
            logFileFailedCasePtr.write(logFilePtr.read());
            logFilePtr.close()
            logFileFailedCasePtr.close(); 
        else:
            os.system("rm -r log");


#
