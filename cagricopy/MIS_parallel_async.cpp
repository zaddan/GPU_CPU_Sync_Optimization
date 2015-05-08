using namespace std;
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "MIS_parallel.h"
#include "status.h"
#include "debug_helpers.h"

#define DEBUG2 1

/*
    reads input file and allocate node_array (list of neighbors) and 
    index_array (number of neighbors) to represent the graph
    ** pass reference to int* in order to get the address of the allocated arrays
    returns the number of nodes in the graph
*/
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

//#define numofnodes numofnodes
    
    int* node_array = new int[total_numofneighbors * 2]; //neighbours of each vertex, CSR representation
    //int* node_array = new int[total_numofneighbors]; //neighbours of each vertex, CSR representation
    int* index_array = new int[numofnodes + 1]; //index values(address values) for each node, CSR representation

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
            accum_numofneighbors++;
        }
        node_index++;
    }
    index_array[numofnodes] = accum_numofneighbors;

#if DEBUG
    cout << "read_input_file:" << endl;
    for(int i = 0; i < numofnodes; i++)
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

/*
    this function is to check if all the nodes with status SELECTED are actually independent
*/

bool check_independence(int *node_array, int *index_array, int *status_array, int numofnodes){
    bool independent = true;

    // true until found a node with its neighbor also SELECTED
    for(int i = 0; i < numofnodes; i++){
        if(status_array[i] == SELECTED){
            int numofneighbors = index_array[i+1] - index_array[i];
            for(int j = 0; j < numofneighbors; j++){
#if DEBUG
                cout << "Node " << i << " neighbor node " << node_array[index_array[i] + j] << " status " << status_array[node_array[index_array[i] + j]] << endl;
#endif
                if( status_array[node_array[index_array[i] + j]] == SELECTED )
                    independent = false;
            }
        }
    }
    return independent;
}

/*
    this function checks if all the nodes that are NOT SELECTED 
    have at least one neighbor that is SELECTED
*/

bool check_maximal(int *node_array, int *index_array, int *status_array, int numofnodes){
    bool maximal = true;

    for(int i = 0; i < numofnodes; i++){
        if(status_array[i] == INACTIVE){
            int numofneighbors = index_array[i+1] - index_array[i];
            bool neighbor_selected = false;
            for(int j = 0; j < numofneighbors; j++){
               if ( status_array[node_array[index_array[i] + j]] == SELECTED )
                    neighbor_selected = true; 
            }
            
            if(!neighbor_selected)
                maximal = false;
        }
    }
    return maximal;
}

void write_output(string filename, int *status_array, int numofnodes){

    ofstream ofs(filename.c_str(), ofstream::out);

    int count = 0;
    for(int i = 0; i < numofnodes; i++){
        if(status_array[i] == SELECTED)
            count++;
    }
    
    ofs << "(" << count << "): ";

    for(int i = 0; i < numofnodes; i++){
        if(status_array[i] == SELECTED)
            ofs << i + 1 << " ";
    }

    ofs << endl;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]) {
    
    	string outFilename; 
   	string inFilename; 
    	string logFileName;

    	if (argc <= 3) {
        	cout << "not enough arguments" << endl;
        	cout <<" you need to provide the sparse graph file name and then desired output file name and a logfile name"<<endl; 
        	exit (EXIT_FAILURE); 
    	}else{
        	inFilename = argv[1];
        	outFilename = argv[2]; 
        	logFileName = argv[3]; 
    	} 

    	int *nodes, *index_array;

    	int numofnodes =  read_input_file(inFilename, &nodes, &index_array);

#if DEBUG
    cout << "MAIN: " <<endl;
    cout << "numofnodes: " << numofnodes << endl;
    for(int i = 0; i < numofnodes; i++)
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
	std::fill_n(nodes_randvalues, numofnodes, 0); // initilize the rand array to zero	

//    for(int i = 0; i < numofnodes; i++)
//        nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/20));


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

    	/*PARALLEL EXECUTION*/

    	// reset status
    	int *nodes_status_parallel = new int[numofnodes];
    	int *nodes_execute = new int[numofnodes];
    	std::fill_n(nodes_status_parallel, numofnodes, ACTIVE);
	//    int *nodes_ready = new int[numofnodes];

#if DEBUG
    printf("Before Parallel Exeuction\nNumOfNOdes = %d\n",numofnodes);
    for(int p=0; p<numofnodes; p++)
    {
        printf("RandValues[%d]=%f ",p,nodes_randvalues[p]);
        printf("Status[%d]=%d\n",p,nodes_status_parallel[p]); 
    }
#endif

    	int gpu_remainingnodes = numofnodes;
    	int step = 0;
 	int stream = 0;
    
    	SNK_INIT_LPARM(lparm,numofnodes);
	int *counter_gpu = new int[numofnodes]; //this will be used to store counter information for every gpu thread
	std::fill_n(counter_gpu, numofnodes,0);	

	//Defining the prime
	int prime;
//	printf("numofnodes here is %d",numofnodes);
//	if (numofnodes > 100 )
//		prime = numofnodes / 1000;
//	else prime = numofnodes;
//	printf("Prime is = %d\n", prime);
	//Normally input sizes are pretty big. Remember the dimacs sparse input graphs. smallest was 500,000

#if DEBUG2
printf("gpu_remaningnodes is %d \n", gpu_remainingnodes);
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

while(gpu_remainingnodes > 0)
{
      	lparm->stream = stream;
       
	if(step == 0)
	{	
#if DEBUG2
printf("In First Prime Step\n");
#endif	

	if(numofnodes >= 10)
	{	
	//Initilize the first primeth nodes of the input then launch kernel
//	for(int i = 0; i < prime; i++)
//		{
  //          	nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40));
    //    	}
        printf("Randed prime part now calling kernel\n");
	mis_parallel_async(counter_gpu,nodes,nodes_randvalues,nodes_status_parallel, index_array,nodes_execute,lparm);
       
	//rand the rest of the nodes while gpu is executing
	for(int i = 0; i < numofnodes; i++)
		{
            	nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40));
            	//nodes_ready[i] = 1;
        	}
	cout << "before deactivatin" <<endl;       
	deactivate_neighbors(nodes,nodes_randvalues,nodes_status_parallel,&gpu_remainingnodes, index_array,nodes_execute,lparm);
	}

	else if (numofnodes < 10)
	{
	//rand the rest of the nodes while gpu is executing
		mis_parallel_async(counter_gpu,nodes,nodes_randvalues,nodes_status_parallel, index_array,nodes_execute,lparm); 
	
for(int i = 0; i < numofnodes; i++)
		{
            	nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40));
            	//nodes_ready[i] = 1;
        	}
 deactivate_neighbors(nodes,nodes_randvalues,nodes_status_parallel,&gpu_remainingnodes, index_array,nodes_execute,lparm);
	

	}
#if DEBUG
        showNodesInfo(nodes_status_parallel, nodes_randvalues, numofnodes, "all");
#endif
	cout << "write to file before" << endl;
        //writing the random values in the log file 
        writeToFileNodeInfo(nodes_status_parallel, nodes_randvalues, numofnodes,logFileName, "all");
        //showNodesInfo(nodes_status_parallel, nodes_randvalues, nodes_execute, numofnodes, "all");
	cout << "after to file" <<endl;
#if DEBUG2
	for(int k=0 ; k<numofnodes; k++)
		if(counter_gpu[k] != 0)
			printf("Counter is not zero at '%d' with value '%d'\n",k,counter_gpu[k]);
#endif	

	/*end of first step where we have launched prime and launched following kernel and then kept  
	randomizing the rest of the nodes. Remember from the preliminary results GPU takes longer than CPU
	since the GPU cores are pretty slow. Although Pannotia paper MIS implementation states CPU takes longer
	than GPU's execution in that case they have used discrete GPU where it is way way faster than HSA
	Computing Units (CUs). 8 CU we have and their frequency is not much high. Something around 700Mhz maybe
	if I am remembering correctly. So having GPU time > CPU time in our case is expected. -Cagri
	*/
	cout<< "before stream sync" << endl;
	stream_sync(stream); //Make GPU finish its current execution before goes to the next one(the one will be submitted at the else part)
        cout<< "done with firststatge" <<endl;	
	
	for(int l=0; l<numofnodes; l++)
	{
	printf("Rand[%d]= %.5f -- ",l,nodes_randvalues[l]);
	printf("Status[%d] = %d",l,nodes_status_parallel[l]);
	printf("Execute[%d] = %d\n",l,nodes_execute[l]);
	}



	}

	
	else   //as long as we have more nodes to work on, we will get into this 'else' 
	{	
#if DEBUG2
printf("Else part is executed, remaining gpu nodes =%d \n",gpu_remainingnodes);
#endif		
		stream_sync(stream);
		//randomize every node for the next step meanwhile gpu executes
//		for(int i = 0; i < numofnodes; i++)
//		{
  //          	nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40));
    //        	//nodes_ready[i] = 1;
      // 	 	}
        	mis_parallel_async(counter_gpu,nodes,nodes_randvalues,nodes_status_parallel, index_array,nodes_execute,lparm);
       
        
	/* One concern is : I will be using rand_values to check whether the node is ready to compare or not
	since checking the readyness array then comparing rand values requires two memory accesses and it is
	inefficienct. We have to optimize the code.
	Concern is Am I sure that second step CPU randomization will not interfere with the current goi`ng on GPU execution.
	*/
	
		deactivate_neighbors(nodes,nodes_randvalues,nodes_status_parallel,&gpu_remainingnodes, index_array,nodes_execute,lparm);
   		stream_sync(stream);


#if DEBUG
        showNodesInfo(nodes_status_parallel, nodes_randvalues,  numofnodes, "all");
#endif
        
	//writing the random values in the log file 
        writeToFileNodeInfo(nodes_status_parallel, nodes_randvalues, numofnodes,logFileName, "all");
        //showNodesInfo(nodes_status_parallel, nodes_randvalues, nodes_execute, numofnodes, "all");
	
	for(int l=0; l<numofnodes; l++)
	{
	printf("Rand[%d]= %.5f -- ",l,nodes_randvalues[l]);
	printf("Status[%d] = %d",l,nodes_status_parallel[l]);
	printf("Execute[%d] = %d\n",l,nodes_execute[l]);
	}
	}	
step++;

}



#if DEBUG
        for(int p=0;p<numofnodes;p++)
        {
            printf("RandValues[%d]=%f ",p,nodes_randvalues[p]);
            printf("Status[%d]=%d\n",p,nodes_status_parallel[p]); 
        }
        int count = 0;
        for(int i = 0; i < numofnodes; i++)
            if( nodes_status_parallel[i] == 1)
                count++;
        printf("remaining: %d\n", gpu_remainingnodes);
        printf("count: %d\n", count);
        cout << "gpu remaining nodes: " << gpu_remainingnodes << endl;
#endif


    cout << "Total number of steps parallel: " << step << endl;


#if DEBUG
    printf("~~Parallel Result~~\n");
    for(int y = 0; y < numofnodes; y++){
        if(nodes_status_parallel[y] == SELECTED)
        printf("%d\n",y);
    }

    printf("After Parallel Exeuction\nNumOfNOdes = %d\n",numofnodes);
    for(int p=0; p < numofnodes; p++)
    {
        printf("RandValues[%d]=%f ",p,nodes_randvalues[p]);
        printf("Status[%d]=%d\n",p,nodes_status_parallel[p]); 
    }
    printf("\n");
#endif

//    /* VERIFY RESULT */
//
//    bool match = true;
//    int serial_count = 0;
//    int parallel_count = 0;
//    for(int i = 0; i < numofnodes; i++){
//        if(nodes_status[i] == SELECTED)
//            serial_count++;
//        if(nodes_status_parallel[i] == SELECTED)
//            parallel_count++;
//        else if (nodes_status_parallel[i] == ACTIVE){
//            cout << "SOMETHING IS WRONG!!!!" <<endl;
//            //return 1;
//        }
//
//        if(nodes_status[i] != nodes_status_parallel[i]){
//            match = false;
//            //cout << "index " << i << " different." <<endl;
//        }
//    }
//
//    if(match)
//        cout << "Parallel MATCHESS Serial" << endl;
//    else
//        cout << "Parallel DOES NOT match Serial" << endl;
//
//    cout << "Serial Count " << serial_count << endl;
//    cout << "Parallel Count " << parallel_count << endl;
//
//    if(check_independence(nodes, index_array, nodes_status, numofnodes))
//        cout << "Serial Independent!" << endl;
//    else
//        cout << "Serial Not independent!" << endl;
//
//    if(check_independence(nodes, index_array, nodes_status_parallel, numofnodes))
//        cout << "Parallel Independent!" << endl;
//    else
//        cout << "Parallel Not independent!" << endl;
//
//    if(check_maximal(nodes, index_array, nodes_status, numofnodes))
//        cout << "Serial Maximal" << endl;
//    else
//        cout << "Serial NOT Maximal" << endl;
//
//    if(check_maximal(nodes, index_array, nodes_status_parallel, numofnodes))
//        cout << "Parallel Maximal" << endl;
//    else
//        cout << "Parallel NOT Maximal" << endl;
//
//    write_output(outFilename + "_serial", nodes_status, numofnodes);
    write_output(outFilename , nodes_status_parallel, numofnodes);
     
    ///things zad changed  
    //writeToFileNodesStatus(nodes_status_parallel, numofnodes,logFileName);
    //showNodesStatus(nodes_status_parallel, numofnodes);
   /// end of things zad changed 
    
    delete[] nodes;
    delete[] nodes_randvalues;
    delete[] nodes_status;
    delete[] nodes_status_parallel;
    delete[] index_array;

    return 0;
}
