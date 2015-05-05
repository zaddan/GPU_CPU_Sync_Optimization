import os
from networkx import nx

#this file contains all the book keeping functions required by the python verify.py file such as parsgin files and such
def findNumOfLinesInAFile(inputFile):
    counter = 0
    with open(inputFile) as f:
        for line in f:
            if line.strip(): 
                counter +=1; 
    return counter




def generateAdjacentFromSparce(sparseRepFileName, adjacentRepFilName):
    #parsing the sparse file 
    #finding the number of nodes in the graph 
    numOfGraphNode= findNumOfLinesInAFile(sparseRepFileName) - 1
    #generating the adjacent graph 
    adjacentMatrix = [[0 for x in range(numOfGraphNode)] for x in range(numOfGraphNode)] 
    lineNum = 0; 
    with open(sparseRepFileName) as f:
        for line in f:
            if (not(lineNum == 0)): #skip the first line
                if line.strip():
                    for words in line.strip().split(' '): 
                        adjacentMatrix[lineNum - 1][int(words) - 1] = 1
            lineNum +=1
    #writing the adjacent representation
    adjacentRepFilePtr = open(adjacentRepFilName, "w")
    #first writing the number of nodes 
    adjacentRepFilePtr.write(str(numOfGraphNode) + "\n");
    #writing the matrix 
    for i in range(0, numOfGraphNode):
        for j in range(0, numOfGraphNode):
            adjacentRepFilePtr.write(str(adjacentMatrix[i][j]) + " ");
        adjacentRepFilePtr.write("\n");




#parses the Result file and finds the Independant list of nodes
#returns the list of these nodes
def parseResultAndExtractIndepNodes(resultFileName):
    independentNodeList = [] 
    with open(resultFileName) as f:
        for line in f:
            for words in line.split():
                if (not("(" in words)):
                    independentNodeList += [int(words)]
    return independentNodeList
#parses the Result file and finds the number of Independant nodes
#returns a number
def parseResultAndExtractNumOfIndepNodes(resultFileName):
    with open(resultFileName) as f:
        for line in f:
            for words in line.split():
                if "(" in words:
                    return int(words.replace("(", '').replace(")", '').replace(":", ''))


#checks whether the number of independent nodes found in the result ref file (acquired from the internet serial version) to our version is the same
def numOfIndepNodeEqual(resultRefFileName, resultFileToVerify):
    numOfIndependentNodesInRefResult= parseResultAndExtractNumOfIndepNodes(resultRefFileName)
    numOfIndependentNodesInFileToVerify= parseResultAndExtractNumOfIndepNodes(resultFileToVerify)
    return (numOfIndependentNodesInRefResult == numOfIndependentNodesInFileToVerify)
