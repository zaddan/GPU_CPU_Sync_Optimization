import os
from book_keeping import *

#get the values in a line 
def getValue(line):
    values = [] 
    for words in (line.strip().split()):
        values +=[int(words)]
    return values


def getSparse(sparseRepFileName):
    sparseMatrixRow = []
    sparseMatrixResult = []
    lineNum = 0; 
    #get the sparse matrix 
    with open(sparseRepFileName) as f:
        for line in f:
            if (not(lineNum == 0)): #skip the first line
                sparseMatrixRow = []
                for  words in (line.strip().split()):
                    sparseMatrixRow += [int(words)]
                sparseMatrixResult.append(sparseMatrixRow)
            lineNum +=1 

    return sparseMatrixResult
#verifies that the set of nodes provided are actually independant by looking at the sparese representation  
def verifyNodesIndependant(possibleIndepNodeList, sparseRepFileName, printBool, logFileName):
    logFilePtr = open(logFileName, "a")
    allLines = [] 
    sparseMatrixResult = []
    lineNum = 0; 
    #get the sparse matrix 
#    with open(sparseRepFileName) as f:
#        for line in f:
#            if (not(lineNum == 0)): #skip the first line
#                sparseMatrixRow = []
#                for  words in (line.strip().split()):
#                    sparseMatrixRow += [int(words)]
#                sparseMatrixResult.append(sparseMatrixRow)
#            lineNum +=1 
#    

    sparseMatrixResult = getSparse(sparseRepFileName)
#    print possibleIndepNodeList
#    print sparseMatrixResult 
    for i in range(0, len(possibleIndepNodeList)):
        for j in range (0, len(possibleIndepNodeList)):
                if (not(i == j)):
                    if (possibleIndepNodeList[i]) in sparseMatrixResult[possibleIndepNodeList[j] - 1]:
                        if (printBool): 
                            logFilePtr.write(str(possibleIndepNodeList[i]) + " is connected to " +  str(possibleIndepNodeList[j]) + "\n")
                            logFilePtr.close(); 
                            logFilePtr = open(logFileName, "r")
                            print  str(possibleIndepNodeList[i]) + " is connected to " +  str(possibleIndepNodeList[j])
                        return 0
     
    logFilePtr.close(); 
    return 1; 
                

#def verifyMaximumSetSerials(sparseRepFileName, adjacentRepFilName, MISResultRefFileName, MISResultToVerifyFileName):
##
#   generateAdjacentFromSparce(sparseRepFileName, adjacentRepFilName)
#   
##  #finding the MIS using the MIS.cpp file
##  #generate the result using the internet found code(this is the refrence code
##  os.system("g++ MIS.cpp")
##  os.system("./a.out " + adjacentRepFilName +  " " + MISResultRefFileName)
##  
##  #genearte the result using out written serial code
##  os.system("g++ serial.cpp")
##  os.system("./a.out" + " " + sparseRepFileName + " " + MISResultToVerifyFileName); 
##  
#        #checking whether the max number of indep node equal   
#   if (numOfIndepNodeEqual(MISResultRefFileName, MISResultToVerifyFileName)):
#       logFilePtr.write("--------number of max independant node test:---->passed")
#       print "--------number of max independant node test:---->passed"
#    else:
#        logFilePtr.write("--------number of max independant node test:---->passed")
#        print "--------number of max independant node test:---->failed"
#        #checking wheter the nodes found are actually independant
#        possibleIndepNodeList = parseResultAndExtractIndepNodes(MISResultRefFileName)
#        if (verifyNodesIndependant(possibleIndepNodeList, sparseRepFileName, 1)):
#            print "--------all nodes independeent test:---->passed"
#        else:
#            print "--------all nodes independeent test:---->failed"
# 
 #
    #generateAdjacentFromSparce(sparseRepFileName, adjacentRepFilName)
    
    #finding the MIS using the MIS.cpp file
    #generate the result using the internet found code(this is the refrence code
#   os.system("g++ MIS.cpp")
#   os.system("./a.out " + adjacentRepFilName +  " " + MISResultRefFileName)
    
    #genearte the result using out written serial code
#   os.system("g++ serial.cpp")
#   os.system("./a.out" + " " + sparseRepFileName + " " + MISResultToVerifyFileName); 
#   
def verifyMaximalSet(sparseRepFileName,  MISResultToVerifyFileName, logFileName):
    #checking wheter the nodes found are actually independant
    possibleIndepNodeList = parseResultAndExtractIndepNodes(MISResultToVerifyFileName)
    failed = 0 #a flag indidcating whether some test was failed 
    if (verifyNodesIndependant(possibleIndepNodeList, sparseRepFileName, 1, logFileName)):
        logFilePtr = open(logFileName, "a")
        logFilePtr.write("--------number of max independant node test:---->passed\n")
        logFilePtr.close()
        print "--------all nodes independeent test:---->passed"
    else:
        logFilePtr = open(logFileName, "a")
        logFilePtr.write("--------number of max independant node test:---->failed \n")
        logFilePtr.close(); 
        #collecting all the failed files in a folder 
        logFilePtr = open(logFileName, "r")
        logFilePtr.close(); 
        print "--------all nodes independeent test:---->failed"
        failed = 1 
    #check whether the set is indeed maximal by adding other nodes and making sure that the set does not stay independant  
    numOfGraphNode= findNumOfLinesInAFile(sparseRepFileName) - 1
    for i in range(0, numOfGraphNode):
        if not((i+1) in possibleIndepNodeList):
            possibleIndepNodeListAddedOneElement = [] 
            #add an element and see if still independent 
            possibleIndepNodeListAddedOneElement = [i + 1] + possibleIndepNodeList;
            if (verifyNodesIndependant(possibleIndepNodeListAddedOneElement, sparseRepFileName, 0, logFileName)):
                #writing into the log folder 
                logFilePtr = open(logFileName, "a")
                logFilePtr.write("--------maximal test:---->failed\n")
                logFilePtr.write("we can add other nodes and still be independent\n")
                logFilePtr.write("for example " + str(possibleIndepNodeListAddedOneElement) + "\n")
                logFilePtr.close(); 
                #printing to the screen 
                print "--------maximal test:---->failed"
                print "we can add other nodes and still be independent"
                print "for example " + str(possibleIndepNodeListAddedOneElement);
                failed = 1 
                return failed 
    logFilePtr = open(logFileName, "a")
    logFilePtr.write("--------maximal test:---->passed\n")
    print "--------maximal test:---->passed"
    logFilePtr.close(); 
    return failed 

