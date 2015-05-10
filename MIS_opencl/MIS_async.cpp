#include <CL/cl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "status.h"
#include "debug_helpers.h"






int convertToString(const char *filename, std::string& s)
{
	size_t size;
	char*  str;
	std::fstream f(filename, (std::fstream::in | std::fstream::binary));

	if(f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);
		str = new char[size+1];
		if(!str)
		{
			f.close();
			return 0;
		}

		f.read(str, fileSize);
		f.close();
		str[size] = '\0';
		s = str;
		delete[] str;
		return 0;
	}
	cout<<"Error: failed to open file\n:"<<filename<<endl;
	return 1;
}


int read_input_file(string filename, int **addr_node_array, int **addr_index_array, int *num_nodes, int *num_edges)
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
    
    *num_nodes = numofnodes;
    *num_edges = total_numofneighbors;

	return 0;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int main(int argc, char* argv[])
{

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

	int numofnodes, numofedges;
    	read_input_file(inFilename, &nodes, &index_array, &numofnodes, &numofedges);

	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms;	//the NO. of platforms
	cl_platform_id platform = NULL;	//the chosen platform
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		cout << "Error: Getting platforms!" << endl;
		return 1;
	}

	/*For clarity, choose the first available platform. */
	if(numPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id* )malloc(numPlatforms* sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		platform = platforms[0];
		free(platforms);
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint numDevices = 0;
	cl_device_id *devices;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);	
	if (numDevices == 0)	//no GPU available.
	{
		cout << "No GPU device available." << endl;
		cout << "Choose CPU as default device." << endl;
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);	
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL);
	}
	else
	{
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
//		printf("DEVICES = %d",sizeof(devices));
	}
	

	/*Step 3: Create context.*/
	cl_context context = clCreateContext(NULL,1, devices,NULL,NULL,NULL);
	cout<< "Context created!" << endl;	

	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
	cout << "Queue created!" << endl;

	cl_int error_program;

	/*Step 5: Create program object */
	const char *filename = "MIS_async.cl";
	string sourceStr;
	status = convertToString(filename, sourceStr);
	const char *source = sourceStr.c_str();
	size_t sourceSize[] = {strlen(source)};
	cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, &error_program);
		if(error_program != CL_SUCCESS) cout << "Create Program with Source Failed" << get_error_string(error_program) << endl;	

	const char *filename2 = "MIS_async_deactivate.cl";
	string sourceStr2;
	status = convertToString(filename2, sourceStr2);
	const char *source2 = sourceStr2.c_str();
	size_t sourceSize2[] = {strlen(source2)};
	cl_program program_deactivate = clCreateProgramWithSource(context, 1, &source2, sourceSize2, &error_program);
		if(error_program != CL_SUCCESS) cout << "Create Program with Source Failed" << get_error_string(error_program) << endl;	
	
	cout << "Programs are created using .cl kernel files!" << endl;

	/*Step 6: Build program. */
	status=clBuildProgram(program, 1,devices,NULL,NULL,NULL);
			if (status != CL_SUCCESS) cout << "Error Building Program = " << get_error_string(status) << endl;
	
	status=clBuildProgram(program_deactivate,1,devices,NULL,NULL,NULL);
			if (status != CL_SUCCESS) cout << "Error Building Program2 = " << get_error_string(status) << endl;

	char buf[0x10000];
	clGetProgramBuildInfo(program,devices[0],CL_PROGRAM_BUILD_LOG,0x10000,buf,NULL);
	printf("%s\n",buf);

	char buf2[0x30000];
	clGetProgramBuildInfo(program_deactivate,devices[0],CL_PROGRAM_BUILD_LOG,0x30000,buf2,NULL);
	printf("%s\n",buf2);
	
	cout << "Programs are build?!" << endl;

	/*Step 7: Setup the inputs*/
	float *nodes_randvalues = new float[numofnodes];
    	srand (static_cast <unsigned> (time(0)));
	std::fill_n(nodes_randvalues, numofnodes, 0);
 	cl_mem buffer_nodes_randvalues = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*numofnodes, (void*) nodes_randvalues,NULL);
//		if (buffer_nodes_randvalues != CL_SUCCESS) cout << "Error buffer_nodes_randvalues = " << buffer_nodes_randvalues << endl;

	int *nodes_status = new int[numofnodes];
    	std::fill_n(nodes_status, numofnodes, ACTIVE);
	cl_mem buffer_nodes_status = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes, (void*) nodes_status,NULL);
//		if (buffer_nodes_status != CL_SUCCESS) cout << "Error buffer_nodes_status = " << buffer_nodes_status << endl;
	
	int *nodes_status_parallel = new int[numofnodes];
 	std::fill_n(nodes_status_parallel, numofnodes, ACTIVE);
	cl_mem buffer_nodes_status_parallel = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes, (void*) nodes_status_parallel,NULL);
   	//if (status != CL_SUCCESS) cout << "Error = " << get_error_string(status) << endl;
	
	int *nodes_execute = new int[numofnodes];
	cl_mem buffer_nodes_execute = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes, (void*) nodes_execute,NULL);
	
	cl_int errorx_gpu_remain;
	int gpu_remainingnodes = numofnodes;
    	int step = 0;
	cl_mem buffer_gpu_remainingnodes = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), (void*) &gpu_remainingnodes,errorx_gpu_remain);
		if(errorx_gpu_remain != CL_SUCCESS) cout << "Error creating gpu remainig nodes buffer" << get_error_string(errorx_gpu_remain) << endl;
		
	int *counter_gpu = new int[numofnodes]; //this will be used to store counter information for every gpu thread
	std::fill_n(counter_gpu, numofnodes,0);	
	cl_mem buffer_counter_gpu = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes, (void*) counter_gpu,NULL);
	
	//Nodes and index
	cl_mem buffer_nodes = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*2*numofedges, (void*) nodes,NULL);
	cl_mem buffer_index_array = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float)*numofnodes, (void*)index_array,NULL);

	cout << "Inputs (buffers) are created!" << endl;

	cl_int error;
	cl_int error2;
	/* Step 8: Create Kernel Object*/
	cl_kernel kernel = clCreateKernel(program,"mis_parallel_async",&error);
	cl_kernel kernel_deactivate = clCreateKernel(program_deactivate,"deactivate_neighbors",&error2);
	
	if (error != CL_SUCCESS) cout << "Error kernel = " << get_error_string(error) << endl;
	if (error2 != CL_SUCCESS) cout << "Error kernel_deactivate = " << get_error_string(error2) << endl;
	
	/* Step 9: Sets Kernel Arguments*/
	clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&buffer_counter_gpu);
	clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&buffer_nodes);
	clSetKernelArg(kernel,2,sizeof(cl_mem),(void*)&buffer_nodes_randvalues);
	clSetKernelArg(kernel,3,sizeof(cl_mem),(void*)&buffer_nodes_status_parallel);
	clSetKernelArg(kernel,4,sizeof(cl_mem),(void*)&buffer_index_array);
	status = clSetKernelArg(kernel,5,sizeof(cl_mem),(void*)&buffer_nodes_execute);
	//mis_parallel_async(counter_gpu,nodes,nodes_randvalues,nodes_status_parallel, index_array,nodes_execute,lparm); 
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg= " << get_error_string(status) << endl;
		

	clSetKernelArg(kernel_deactivate,0,sizeof(cl_mem),(void*)&buffer_nodes);
	clSetKernelArg(kernel_deactivate,1,sizeof(cl_mem),(void*)&buffer_nodes_randvalues);
	clSetKernelArg(kernel_deactivate,2,sizeof(cl_mem),(void*)&buffer_nodes_status_parallel);
	clSetKernelArg(kernel_deactivate,3,sizeof(cl_mem),(void*)&buffer_gpu_remainingnodes); 
	clSetKernelArg(kernel_deactivate,4,sizeof(cl_mem),(void*)&buffer_index_array);
	clSetKernelArg(kernel_deactivate,5,sizeof(cl_mem),(void*)&buffer_nodes_execute);
	//deactivate_neighbors(nodes,nodes_randvalues,nodes_status_parallel,&gpu_remainingnodes, index_array,nodes_execute,lparm);
	
	cout << "Kernel objects out of programs are created! Kernel arguments are defined!" << endl;

	/*Step 10 = Rand and Call GPU*/

	cl_uint compute_units;
	size_t threadnumber;
	clGetDeviceInfo(devices[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, NULL);
	cout << "Compute Units =" << compute_units << endl;
	
	//cl_uint *in;
	threadnumber = 256 * compute_units;
	cl_int errormap;	
	while(gpu_remainingnodes > 0)
	{
		cout << "Randomizing values" << endl;
		for(int i = 0; i < numofnodes; i++)
		{
		nodes_randvalues[i]= static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/40));
       		}
        	printf("Randed. Now calling kernel!\n");

		//TODO CALL GPU HERE TO EXECUTE and DEACTIVATE
		status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &threadnumber, NULL, 0, NULL, NULL);
		cout << "Execution kernel sent!" <<endl;       
			if (status != CL_SUCCESS) cout << "Error kernel call! = " << get_error_string(status) << endl;
	
		status = clEnqueueNDRangeKernel(commandQueue, kernel_deactivate, 1, NULL, &threadnumber, NULL, 0, NULL, NULL);
		cout << "Deactivation kernel sent!" << endl;
			if (status != CL_SUCCESS) cout << "Error kernel_deactivate call! = " << get_error_string(status) << endl;
	
	//	barrier(CLK_GLOBAL_MEM_FENCE);
	//	in = (cl_uint *) clEnqueueMapBuffer(commandQueue,buffer_gpu_remainingnodes,CL_TRUE,CL_MAP_READ,0,sizeof(int),0,NULL,NULL,&errormap);
	//		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
	
	errormap = clEnqueueReadBuffer(commandQueue, buffer_gpu_remainingnodes, CL_TRUE, 0, sizeof(int), &gpu_remainingnodes, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;

	
	//gpu_remainingnodes = (int)(*in);
	printf("GPU Remaining Nodes = %d\n", gpu_remainingnodes);
	}

	
#if DEBUG
        showNodesInfo(nodes_status_parallel, nodes_randvalues, numofnodes, "all");
#endif

//	cout << "write to file before" << endl;
        //writing the random values in the log file 
//        writeToFileNodeInfo(nodes_status_parallel, nodes_randvalues, numofnodes,logFileName, "all");
        //showNodesInfo(nodes_status_parallel, nodes_randvalues, nodes_execute, numofnodes, "all");
//	cout << "after to file" <<endl;

/*
#if DEBUG2
	for(int k=0 ; k<numofnodes; k++)
		if(counter_gpu[k] != 0)
			printf("Counter is not zero at '%d' with value '%d'\n",k,counter_gpu[k]);
#endif	
*/	

	clFinish(commandQueue);

	cout << "End of the execution, clFinish has been satisfied!" << endl;
	//Print out the result
	for(int l=0; l<numofnodes; l++)
	{
		printf("Rand[%d]= %.5f -- ",l,nodes_randvalues[l]);
		printf("Status[%d] = %d",l,nodes_status_parallel[l]);
		printf("Execute[%d] = %d\n",l,nodes_execute[l]);
	}

	return 0;
}
