__kernel void deactivate_neighbors(__global int *nodes, __global float *nodes_randvalues, __global int *nodes_status, __global int *remaining_nodes, __global int* indexarray, __global int* execute) {
    int i = get_global_id(0);

//if(i<6)
//{    
    int numofneighbour = indexarray[i+1] - indexarray[i];

    if (execute[i] == 1)
    {
        nodes_status[i] = 2;  // select myself
        atomic_dec(remaining_nodes);  // remove myself from active node count
        for(int k = 0 ; k < numofneighbour; k++)
        {
            if( atomic_xchg(&nodes_status[nodes[indexarray[i] + k]], 0) )
            {
                //nodes_status[nodes[indexarray[i] + k ]] = 0;
                atomic_dec(remaining_nodes);
            }
        }
    }
}
//}
