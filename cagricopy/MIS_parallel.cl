#include "status.h"

#define DELAY 50

__kernel void mis_parallel(__global int *nodes, __global float *nodes_randvalues, __global int *nodes_status, __global int* indexarray, __global int* execute) {
    int i = get_global_id(0);
    
    execute[i] = 1;
    
    int numofneighbour = indexarray[i+1] - indexarray[i];

    if(nodes_status[i] == ACTIVE )
    {   
        for(int k = 0; k < numofneighbour; k++){
            if(nodes_status[nodes[indexarray[i] + k]] == ACTIVE && nodes_randvalues[i] > nodes_randvalues[nodes[indexarray[i] + k]]) 
            {
                execute[i] = 0;
                break;
            }
        }   
    }
    else
        execute[i] = 0;
}

__kernel void mis_parallel_async(__global int* counter,__global int *nodes, __global float *nodes_randvalues, __global int *nodes_status, __global int* indexarray, __global int* execute) {
    
	int i = get_global_id(0);
	execute[i] = 1;

	int numofneighbour = indexarray[i+1] - indexarray[i];
	int countervalue = 0;
	
	while( nodes_randvalues[i] == 0 )
		countervalue++;

	if(nodes_status[i] == ACTIVE )
	{	 
       		for(int k = 0; k < numofneighbour; k++)
		{
       
		while(nodes_randvalues[nodes[indexarray[i]+k]] == 0)
			countervalue++;

			if(nodes_status[nodes[indexarray[i] + k]] == ACTIVE && nodes_randvalues[i] > nodes_randvalues[nodes[indexarray[i] + k]]) 
            		{
                	execute[i] = 0;
                	break;
            		}
        	}   
	}	
    	else
        	execute[i] = 0;

	counter[i]=countervalue;
}

__kernel void deactivate_neighbors(__global int *nodes, __global float *nodes_randvalues, __global int *nodes_status, __global int *remaining_nodes, __global int* indexarray, __global int* execute) {
    int i = get_global_id(0);
    
    int numofneighbour = indexarray[i+1] - indexarray[i];

    if (execute[i] == 1)
    {
        nodes_status[i] = SELECTED;  // select myself
        atomic_dec(remaining_nodes);  // remove myself from active node count
        for(int k = 0 ; k < numofneighbour; k++)
        {
            if( atom_xchg(&nodes_status[nodes[indexarray[i] + k]], INACTIVE) )
            {
                //nodes_status[nodes[indexarray[i] + k ]] = 0;
                atomic_dec(remaining_nodes);
            }
        }
    }
}
    
__kernel void removeneig (__global int* nodes, __global short int* nodes_status, __global int* numofnodes, __global int* indexarray){

    int v = get_global_id(0);


    if(nodes_status[v] == 2)
        {
            int numofneighbour = indexarray[v+1]-indexarray[v];
            for(int w=0 ; w<numofneighbour; w++)
                {
                if(nodes_status[nodes[indexarray[v]+w]] != 0 )
                    {
                    nodes_status[nodes[indexarray[v]+w]] = 0;
                    atomic_dec(numofnodes);
                    }
                }
        }

}
