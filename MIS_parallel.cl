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

__kernel void mis_parallel_async(__global int *nodes, __global float *nodes_randvalues, __global int *nodes_status, __global int* indexarray, __global int* execute, __global char *ready, __global int* node_counters, __global int* node_neighbor_counters) {
    int i = get_global_id(0);
    
    execute[i] = 1;

    int numofneighbour = indexarray[i+1] - indexarray[i];

    if(nodes_status[i] == ACTIVE )
    {   
        int counter = 0;
        while(ready[i] == 0){
            /* this fails at compile stage, uncomment and run ./buildrun.sh to see the error messages
            int count = DELAY;
            while(count > 0)
                count--;
            */

            /* this fails the same thing
            for(int i = 0; i < DELAY; i++);
            */
            ++counter;
        }

        //node_counters[i] = counter;

        for(int k = 0; k < numofneighbour; k++){
            //counter = 0;
            while(ready[nodes[indexarray[i] + k]] == 0){
                //int count = DELAY;
                //while(count > 0)
                //    count--; 
                //for(int i = 0; i < DELAY; i++);
                ++counter;
            }
            //node_neighbor_counters[indexarray[i] + k] = counter;
            node_counters[i] = counter;

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
