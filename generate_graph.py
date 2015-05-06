import networkx as nx
import matplotlib.pyplot as plt
import math
from verify import *
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

def draw_graph(graphDic):
    #first writing the number of nodes 
    G = nx.from_dict_of_lists(graphDic) 
#    nx.draw(G) 
    #nx.draw_networkx(G) 
    nx.draw_circular(G, with_labels=True)
    #save the result  semiSparseRep 
    plt.savefig("now");


#gets sparse representation and also the graph info which is random values and node status
def drawRandomValues(sparseRepFileName, graphInfoFileName):
    sparseMatrix = getSparse(sparseRepFileName)
    with open(graphInfoFileName) as f:
        for line in f:
            randValues = getValue(line)
            key = zip(range(1, len(randValues)+1), randValues)
            for i in range(0, len(sparseMatrix)):
                for j in range(0, len(sparseMatrix[i])):
                    sparseMatrix[i][j] = key[sparseMatrix[i][j] - 1]
            print sparseMatrix
            graph = dict(zip(key, sparseMatrix))
            graph = dict(zip(key, sparseMatrix))
            draw_graph(graph)
            


#drawRandomValues("sparse_rep.txt", "scratch2")
#draw_graph()
#drawRandomValues("sparse_rep.txt")   
#generate_random_graphs(20, "graph_generated_test.txt.")



