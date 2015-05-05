from networkx import nx
import math
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
