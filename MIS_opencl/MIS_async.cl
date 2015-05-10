__kernel void mis_parallel_async(__global int* counter,__global int *nodes, __global float *nodes_randvalues, __global int *nodes_status, __global int* indexarray, __global int* execute) {
    
	int i = get_global_id(0);
	execute[i] = 1;

	int numofneighbour = indexarray[i+1] - indexarray[i];
	int countervalue = 0;
	
	while( nodes_randvalues[i] == 0 )
		countervalue++;

	if(nodes_status[i] == 1 )
	{	 
       		for(int k = 0; k < numofneighbour; k++)
		{
       
		while(nodes_randvalues[nodes[indexarray[i]+k]] == 0)
			countervalue++;

			if(nodes_status[nodes[indexarray[i] + k]] == 1 && nodes_randvalues[i] > nodes_randvalues[nodes[indexarray[i] + k]]) 
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
