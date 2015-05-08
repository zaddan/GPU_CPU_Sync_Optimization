import networkx as nx
import matplotlib.pyplot as plt
import math
from verify import *
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import copy
#generate a random graph 
#give it number of nodes and an output file
#this function return a zero if one of the vertices has a degree 0 (which can be problematic), thus in this case, we need to recall till we get a graph where all nodes has a degree of one or higher
def generate_random_graphs(numberOfNodes, outputFile):
    sparseResult = open(outputFile, "w")
    #first writing the number of nodes 
    G=nx.gnm_random_graph(numberOfNodes, numberOfNodes*int(math.sqrt(numberOfNodes)))
    sparseResult.write(str(numberOfNodes) +  " " + str(nx.number_of_edges(G)) + "\n");
    semiSparseRep = nx.to_dict_of_lists(G)
    #print semiSparseRep 
    for element in semiSparseRep:
        if len(semiSparseRep[element]) ==0:
            return 0
        for j in semiSparseRep[element]:
            sparseResult.write(str(j+1) + " ")
        sparseResult.write("\n")
    return 1


#gets a dictionary of lists and draw the graph associated with it. It also converts the status to colors
def draw_graph(graphDic, nodesStatus, imageName):
    node_colors = [] 
    #first writing the number of nodes 
    #nx.draw(G) 
    #select the color 
    newGraphDic = {} #without the status 
    for element in graphDic.keys():
        status = nodesStatus[element[0] - 1]
        if status == "INACTIVE":
            node_colors +=['white']
        if status == "ACTIVE":
            node_colors +=['red']
        if status == "SELECTED":
            node_colors +=['green']
    #generating the graph from the dictionary 
    G = nx.from_dict_of_lists(graphDic) 
    nx.draw_circular(G, node_color = node_colors, with_labels=True, node_size = 50)
    #G.text(3, 8, 'boxed italics text in data coords', style='italic', bbox={'facecolor':'red', 'alpha':0.5, 'pad':10})
#    plt.legend(handles=[ green_patch])
#    nx.draw_networkx(G, node_color=node_colors, with_labels=True)
    
    #nx.draw_networkx(G) 
    #save the result  semiSparseRep 
    plt.savefig(imageName);

#
##gets sparse representation and also the graph info which is random values and node status
#def drawGraphDebug(sparseRepFileName, graphInfoFileName):
#    sparseMatrix = getSparse(sparseRepFileName)
#    with open(graphInfoFileName) as f:
#        for line in f:
#            randValues = getValue(line)
#            key = zip(range(1, len(randValues)+1), randValues)
#            for i in range(0, len(sparseMatrix)):
#                for j in range(0, len(sparseMatrix[i])):
#                    sparseMatrix[i][j] = key[sparseMatrix[i][j] - 1]
#            print sparseMatrix
#            graph = dict(zip(key, sparseMatrix))
#            graph = dict(zip(key, sparseMatrix))
#            draw_graph(graph)
#            

#gets sparse representation and also the graph info which is random values and node status
def drawGraphDebug(logFileName):
    sparseMatrix = getSparseMessy(logFileName)
    randValues = [] 
    status = []
    randCollected = 0 #indicates whether randomValues where collected
    statusCollected = 0 #indicates whether statusValues where collected
    gotAllInfo = False #if true, we have enough info to draw a graph 
    stageNumber = 0  #stage number detemrins the number of iteration (for example, the nth time that cpu is randomizing the values, but doesnt have to be this)
    #---------guide:::  go through the whole find to collect information
    with open(logFileName) as f:
        for line in f:
            #getting the random values 
            if (len(line.strip().split()) > 0 and line.strip().split()[0]== "randValues:"):
                randCollected += 1   #indicates we have collected the random values
                randValues = getValue(line)
            
            #getting the status
            if (len(line.strip().split()) > 0 and line.strip().split()[0]== "status:"):
                status = getValue(line)
                statusCollected +=1 #indicates we have collected the status
            #---------guide:::  check and see if both criterian is satisfied
            if (randCollected == 1 and statusCollected ==1 ):
                gotAllInfo = True;
            elif (randCollected >= 2 or statusCollected >=2):
                print "something went wrong, we are not supposed to write more than one status or random values consecuitively \n"
            
            #---------guide:::  if all the criterian is satisfied, draw a graph
            if (gotAllInfo):
                modifiedSparseMatrix = copy.deepcopy(sparseMatrix)#clone
                key = zip(range(1, len(randValues)+1), randValues)
                #replacing each element of the sparseMatrix with a tuple containing the node number, rand 
                for i in range(0, len(modifiedSparseMatrix)):
                    for j in range(0, len(modifiedSparseMatrix[i])):
                        modifiedSparseMatrix[i][j] = key[modifiedSparseMatrix[i][j] - 1]
                    #            print sparseMatrix

                graph = dict(zip(key, modifiedSparseMatrix))
                imageName = logFileName[:-4] + "_" + str(stageNumber) + ".png"
                draw_graph(graph, status, imageName)
                print "the graph was made for " + logFileName + " with the name of " + imageName
                graph = [] 
                gotAllInfo = False  
                statusCollected = 0
                randCollected = 0
                stageNumber +=1
                del modifiedSparseMatrix 
                del key 
#    print status
#    print randValues
#    print sparseMatrix
#    key = zip(range(1, len(randValues)+1), randValues)
#    #replacing each element of the sparseMatrix with a tuple containing the node number, rand 
#    for i in range(0, len(sparseMatrix)):
#        for j in range(0, len(sparseMatrix[i])):
#            sparseMatrix[i][j] = key[sparseMatrix[i][j] - 1]
##            print sparseMatrix
#
#    graph = dict(zip(key, sparseMatrix))
#    draw_graph(graph, status, imageName)
##            



#drawGraphDebug("./log/log_1520.txt")
#def drawGraphDebug(sparseRepFileName, graphInfoFileName):
#drawGRaphDebug("sparse_rep.txt", "scratch2")
#draw_graph()
#drawRandomValues("sparse_rep.txt")   
#generate_random_graphs(20, "graph_generated_test.txt.")



