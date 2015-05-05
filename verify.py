import os
from book_keeping import *
#verifies that the set of nodes provided are actually independant by looking at the sparese representation  
def verifyNodesIndependant(possibleIndepNodeList, sparseRepFileName, printBool, logFileName):
    logFilePtr = open(logFileName, "a")
    allLines = [] 
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
    
#    print possibleIndepNodeList
#    print sparseMatrixResult 
    for i in range(0, len(possibleIndepNodeList)):
        for j in range (0, len(possibleIndepNodeList)):
                if (not(i == j)):
#                    print possibleIndepNodeList[i]
#                    print  sparseMatrixResult[possibleIndepNodeList[j] - 1]

                    if (possibleIndepNodeList[i]) in sparseMatrixResult[possibleIndepNodeList[j] - 1]:
                        if (printBool): 
                            logFilePtr.write(str(possibleIndepNodeList[i]) + " is connected to " +  str(possibleIndepNodeList[j]) + "\n")
                            print  str(possibleIndepNodeList[i]) + " is connected to " +  str(possibleIndepNodeList[j])
                            logFilePtr.close(); 
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
    if (verifyNodesIndependant(possibleIndepNodeList, sparseRepFileName, 1, logFileName)):
        logFilePtr = open(logFileName, "a")
        logFilePtr.write("--------number of max independant node test:---->passed\n")
        print "--------all nodes independeent test:---->passed"
    else:
        logFilePtr = open(logFileName, "a")
        logFilePtr.write("--------number of max independant node test:---->failed \n")
        print "--------all nodes independeent test:---->failed"
    #check whether the set is indeed maximal by adding other nodes and making sure that the set does not stay independant  
    logFilePtr.close()
    numOfGraphNode= findNumOfLinesInAFile(sparseRepFileName) - 1
    for i in range(0, numOfGraphNode):
        if not((i+1) in possibleIndepNodeList):
            possibleIndepNodeListAddedOneElement = [] 
            #add an element and see if still independent 
            possibleIndepNodeListAddedOneElement = [i + 1] + possibleIndepNodeList;
            if (verifyNodesIndependant(possibleIndepNodeListAddedOneElement, sparseRepFileName, 0, logFileName)):
                logFilePtr = open(logFileName, "a")
                logFilePtr.write("--------maximal test:---->failed\n")
                logFilePtr.write("we can add other nodes and still be independent\n")
                logFilePtr.write("for example " + str(possibleIndepNodeListAddedOneElement) + "\n")
                print "--------maximal test:---->failed"
                print "we can add other nodes and still be independent"
                print "for example " + str(possibleIndepNodeListAddedOneElement);
                logFilePtr.close(); 
                return 0
    
    logFilePtr = open(logFileName, "a")
    logFilePtr.write("--------maximal test:---->passed\n")
    print "--------maximal test:---->passed"
    logFilePtr.close(); 
    return 1

