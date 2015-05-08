#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include "MIS_parallel.h"
#include <fstream>

#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "status.h"
#define RUNBOTH 0
#define DEBUG 0
void writeResults(string outFileName, int *nodes_status, int numofnodes){
  ofstream outfile (outFileName.c_str());
  int numOfIndependentNodes= 0; 
  for(int p=0; p < numofnodes; p++)
  {
      if (nodes_status[p] == SELECTED) {
          numOfIndependentNodes++;
      }
  }
  outfile << "(" <<numOfIndependentNodes<<"):" << " ";
  
  for(int p=0; p < numofnodes; p++)
  {
      if (nodes_status[p] == SELECTED) {
          outfile << p + 1 << " "; 
          numOfIndependentNodes++;
      }
  }
  outfile << endl; 
//  outfile << "number of independent nodes are: " << numOfIndependentNodes<<endl;
}
int read_input_file(string filename, int **addr_node_array, int **addr_index_array)
{
    ifstream myfile;
	myfile.open(filename.c_str());
    if(!myfile.is_open()){
        cout << "Error opening file!" << endl; 
        return 1; 
    }

	string line;
    getline(myfile, line);
    istringstream iss(line);
    
	int numofnodes = 0;
    int total_numofneighbors = 0;
    iss >> numofnodes;
    iss >> total_numofneighbors;  // number is INCORRECT right now
#if DEBUG
    cout << "read_input_file: " << numofnodes << " " << total_numofneighbors <<endl;
#endif

	
	int* node_array = new int[numofnodes * 500]; //neighbours of each vertex, CSR representation
	//int* node_array = new int[total_numofneighbors]; //neighbours of each vertex, CSR representation
	int* index_array = new int[numofnodes];	//index values(address values) for each node, CSR representation

    int node_index = 0;
    int accum_numofneighbors = 0;
	while(!myfile.eof())
	{	
		getline(myfile,line);
		istringstream iss(line);
	 if (node_index < numofnodes) {	
         index_array[node_index] = accum_numofneighbors;
      }
		int temp;
		while (iss >> temp)
		{
			node_array[accum_numofneighbors] = temp - 1;	
            //cout << temp << " ";
            accum_numofneighbors++;
		}
        //cout << endl;
        //cout << accum_numofneighbors << endl;
        //cout << node_index << endl;
        node_index++;
	}
#if DEBUG
    cout << "read_input_file:" << endl;
    for(int i = 0; i < numofnodes - 1; i++)
    {
        cout << "node "<< i << ": ";
        for(int j = 0; j < index_array[i+1] - index_array[i]; j++)
            cout << node_array[index_array[i] + j] << " ";

        cout << endl;
    }
#endif
    
    *addr_node_array = node_array;
    *addr_index_array = index_array;

    return numofnodes;
}

int main(int argc, char *argv[]) {
    //string filename = "af_shell9.graph.copy";
    string outFileName; 
    string filename; 
    if (argc <= 2) {
        cout << "not enough arguments" << endl;
        cout <<" you need to provide the sparse graph file name and then desired output file name"<<endl; 
        exit (EXIT_FAILURE); 
    }else{
        filename = argv[1];
        outFileName = argv[2]; 
    } 
    int *nodes, *index_array;

    int numofnodes =  read_input_file(filename, &nodes, &index_array);
#if DEBUG
    cout << "MAIN: " <<endl;
    cout << "numofnodes: " << numofnodes << endl;
    for(int i = 0; i < numofnodes - 1; i++)
    {
        cout << "node "<< i << ": ";
        for(int j = 0; j < index_array[i+1] - index_array[i]; j++)
            cout << nodes[index_array[i] + j] << " ";
        cout << endl;
    }
#endif

    // setup random array
    float *nodes_randvalues = new float[numofnodes];
    srand (static_cast <unsigned> (time(0)));
    for(int i = 0; i < numofnodes; i++)
        nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/20));


    // setup status array
    int *nodes_status = new int[numofnodes];
    std::fill_n(nodes_status, numofnodes, ACTIVE);
#if DEBUG
    cout << "MAIN: " <<endl;
    for(int p = 0; p < numofnodes; p++)
    {
        printf("RandValues[%d] = %f\t",p,nodes_randvalues[p]);
        printf("Status[%d] = %d ",p,nodes_status[p]); 
        printf("\n");
    }
#endif


    /* SERIAL EXECUTION*/

    int remainingnodes = numofnodes;
    int step=1;
    while(remainingnodes > 0)
    {
        printf("%dst Stage\n",step);
        for (int v = 0; v < numofnodes; v++)
        {
            int execute = 1;
            if(nodes_status[v] == ACTIVE)
            {	
                // compare values with all ACTIVE neighbors
                int numofneighbour = index_array[v+1] - index_array[v];
                for(int i = 0; i < numofneighbour; i++)
                {	
                    if(nodes_status[nodes[index_array[v] + i]] == ACTIVE)
                    {
                        //printf("Comparing %d to %d\n",v,nodes[index_array[v] + i]);
                        if(nodes_randvalues[v] > nodes_randvalues[nodes[index_array[v] + i]])
                        {
                            execute=0;
                            break;
                        }
                    }
                }		

                // if I have the minimum value comparing to my neighbors
                if(execute == 1)
                {	
                    //printf("Selected %d at %dst Step\n", v, step);
                    nodes_status[v] = SELECTED;
                    remainingnodes--;

                    // deactivate all of my neighbors
                    for(int w = 0; w < numofneighbour; w++)
                    {
                        if(nodes_status[nodes[index_array[v]+w]] != INACTIVE)
                        {
                            nodes_status[nodes[index_array[v]+w]] = INACTIVE;
                            remainingnodes--;
                        }
                    }

                }

            }
        }
        //printf("Remaining nodes = %d after step\n",remainingnodes);
        step++;
    }

#if DEBUG
    printf("~~Serial Result~~\n");
    for(int y = 0; y < numofnodes; y++){
        if(nodes_status[y] == SELECTED)
            printf("%d\n",y);
    }
    printf("~~Serial Result Ones~~ should be nothing\n");
    for(int y=0; y< numofnodes; y++){
        if(nodes_status[y] == ACTIVE)
            printf("%d\n",y);
    }
    printf("After Serial Exeuction\nNumOfNOdes = %d\n",numofnodes);
    for(int p=0; p < numofnodes; p++)
    {
        printf("RandValues[%d]=%f ",p,nodes_randvalues[p]);
        printf("Status[%d]=%d\n",p,nodes_status[p]); 
    }
    printf("\n");

#endif

//  ofstream outfile (outFileName.c_str()); 
  writeResults(outFileName, nodes_status, numofnodes); 
//  int numOfIndependentNodes= 0; 
//  outfile<<"here is the set of independent nodes:" << endl;
//  for(int p=0; p < numofnodes; p++)
//  {
//      if (nodes_status[p] == SELECTED) {
//          outfile << p + 1 << " "; 
//          numOfIndependentNodes++;
//      }
//  }
//  outfile << endl; 
//  outfile << "number of independent nodes are: " << numOfIndependentNodes<<endl;
  return 0;
}
