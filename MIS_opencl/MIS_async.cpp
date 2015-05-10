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
#include "CL/cl_ext.h"

//#include "CLUtil.hpp"

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


int read_input_file(string filename, int **addr_node_array, int **addr_index_array, int *num_nodes, int *num_edges, cl_context contextfrommain )
{
    	return 0;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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
	
	cl_int errorcode; //this will be used for error checking	

	/*Step 3: Create context.*/
	cl_context context = clCreateContext(NULL,1, devices,NULL,NULL,&errorcode);
		if(errorcode != CL_SUCCESS) cout << "Error Creating Context : " << get_error_string(errorcode) << endl;
		else cout << "Context created!" << endl;

	cout << "before queue creation" << endl;
	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue = clCreateCommandQueueWithProperties(context, devices[0], NULL, &errorcode);
	cout << "ohom" << endl;	
	if(errorcode != CL_SUCCESS) cout << "Error Creating Queue : " << get_error_string(errorcode) << endl;
		else cout << "Queue created!" << endl;

	
//////////////////////////////////////////////////////////////

	ifstream myfile;
    	myfile.open(inFilename.c_str());
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

    	cout << "read_input_file: " << numofnodes << " " << total_numofneighbors <<endl;

//#define numofnodes numofnodes
   	cout << "before" << endl; 
    	int* nodes = (int*)clSVMAlloc(context, CL_MEM_READ_ONLY, sizeof(int)*total_numofneighbors*2, 0);//new int[total_numofneighbors * 2]; //neighbours of each vertex, CSR representation
    //int* node_array = new int[total_numofneighbors]; //neighbours of each vertex, CSR representation
    	int* index_array = (int*)clSVMAlloc(context, CL_MEM_READ_ONLY, sizeof(int)*(numofnodes+1),0); //new int[numofnodes + 1]; //index values(address values) for each node, CSR representation
	cout << "aftr" << endl;
    	int node_index = 0;
    	int accum_numofneighbors = 0;


	cl_int readerror;
	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, nodes, sizeof(int)*total_numofneighbors*2, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for mapping nodes into host to create nodes array!" << get_error_string(readerror) << endl;
	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, index_array, sizeof(int)*(numofnodes+1), 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for mapping index_array into host to create index_array!" << get_error_string(readerror) << endl;
			
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
            		nodes[accum_numofneighbors] = temp - 1;    
            		accum_numofneighbors++;
        	}
        	node_index++;
    	}
    	index_array[numofnodes] = accum_numofneighbors;

	readerror = clEnqueueSVMUnmap(commandQueue,nodes, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping nodes!" << get_error_string(readerror) << endl;
	readerror = clEnqueueSVMUnmap(commandQueue,index_array, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping index_array!" << get_error_string(readerror) << endl;

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
	cout << "end of read file!" << endl;
    
//    	*addr_node_array = node_array;
//    	*addr_index_array = index_array;
    
//    *num_nodes = numofnodes;
//    *num_edges = total_numofneighbors;


/////////////////////////////////////////////////////////////////7	
//	int *nodes, *index_array;
	/* Read file  */
//	int numofnodes, numofedges;
//    	read_input_file(inFilename, &nodes, &index_array, &numofnodes, &numofedges, context);


	/*Step 5: Create program object */
	const char *filename = "MIS_async.cl";
	string sourceStr;
	status = convertToString(filename, sourceStr);
	const char *source = sourceStr.c_str();
	size_t sourceSize[] = {strlen(source)};
	cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, &errorcode);
		if(errorcode != CL_SUCCESS) cout << "Create Program with Source for MIS_async.cl Failed : " << get_error_string(errorcode) << endl;	
		else cout << "MIS_async.cl program created!" << endl;

	const char *filename2 = "MIS_async_deactivate.cl";
	string sourceStr2;
	status = convertToString(filename2, sourceStr2);
	const char *source2 = sourceStr2.c_str();
	size_t sourceSize2[] = {strlen(source2)};
	cl_program program_deactivate = clCreateProgramWithSource(context, 1, &source2, sourceSize2, &errorcode);
		if(errorcode != CL_SUCCESS) cout << "Create Program with Source for MIS_async_deactivate Failed!" << get_error_string(errorcode) << endl;	
		else cout << "MIS_async_deavtivate.cl program created!" << endl;

	
	/*Step 6: Build program. */
	status=clBuildProgram(program, 1,devices,NULL,NULL,NULL);
			if (status != CL_SUCCESS) cout << "Error Building Program = " << get_error_string(status) << endl;
			else cout << "Program of MIS_async.cl has been built!" << endl;
	
	status=clBuildProgram(program_deactivate,1,devices,NULL,NULL,NULL);
			if (status != CL_SUCCESS) cout << "Error Building Program2 = " << get_error_string(status) << endl;
			else cout << "Program of MIS_async_deactivate.cl has been built!" << endl;

	
	/* Those two bufs will show what is the error of building programs, if there is one*/
	char buf[0x10000];
	clGetProgramBuildInfo(program,devices[0],CL_PROGRAM_BUILD_LOG,0x10000,buf,NULL);
	printf("%s\n",buf);

	char buf2[0x30000];
	clGetProgramBuildInfo(program_deactivate,devices[0],CL_PROGRAM_BUILD_LOG,0x30000,buf2,NULL);
	printf("%s\n",buf2);

	srand (static_cast <unsigned> (time(0)));

	/*Step 7: Setup the inputs*/
	cout<< "SVMAllocing nodes_randvalues..." << endl;
	
	float* nodes_randvalues = (float*) clSVMAlloc(context,CL_MEM_READ_WRITE, sizeof(int)*numofnodes,0); //new float[numofnodes];
	
	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, nodes_randvalues, sizeof(int)*(numofnodes), 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error nodes_randvalues are not mapped!" << get_error_string(readerror) << endl;
	
	cout<< "nodes_randvalues are being initialized to zero..." << endl;
	std::fill_n(nodes_randvalues, numofnodes, 0);		
    	
	readerror = clEnqueueSVMUnmap(commandQueue,nodes_randvalues, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping nodes_randvalues!" << get_error_string(readerror) << endl;

	cout<< "SVMAllocing nodes_randvalues...DONE" << endl;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 
	cout<< "SVMallocing nodes_status..." << endl;
	int *nodes_status = (int*)clSVMAlloc(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes,0); //new int[numofnodes];
    	
	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, nodes_status, sizeof(int)*(numofnodes), 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error nodes_status are not mapped!" << get_error_string(readerror) << endl;
	
	cout<< "nodes_status are being initialized to ACTIVE..." << endl;
	std::fill_n(nodes_status, numofnodes, ACTIVE);		
    	
	cout<< "SVMAllocing nodes_status...DONE" << endl;
	
	//small stupid check if it works
	for(int r; r < numofnodes; r++) printf("nodes_status[%d] = %d\n",r,nodes_status[r]);

	readerror = clEnqueueSVMUnmap(commandQueue,nodes_status, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping nodes_status!" << get_error_string(readerror) << endl;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<< "SVMallocing nodes_status_parallel..." << endl;
	int *nodes_status_parallel = (int*)clSVMAlloc(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes,0); //new int[numofnodes];
    	
	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, nodes_status_parallel, sizeof(int)*(numofnodes), 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error nodes_status_parallel are not mapped!" << get_error_string(readerror) << endl;
	
	cout<< "nodes_status_parallel are being initialized to ACTIVE..." << endl;
	std::fill_n(nodes_status_parallel, numofnodes, ACTIVE);		
    	
	cout<< "SVMAllocing nodes_status_parallel...DONE" << endl;
	
	//small stupid check if it works
	for(int r; r < numofnodes; r++) printf("nodes_status_parallel[%d] = %d\n",r,nodes_status_parallel[r]);

	readerror = clEnqueueSVMUnmap(commandQueue,nodes_status_parallel, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping nodes_status_parallel!" << get_error_string(readerror) << endl;

////////////////////////////////////////////////////////
	int *nodes_execute = (int*)clSVMAlloc(context, CL_MEM_READ_WRITE, sizeof(int)*numofnodes,0); // new int[numofnodes];

/////////////////////////////////////////////////////////////////	

	int* gpu_remainingnodes = (int*)clSVMAlloc(context, CL_MEM_READ_WRITE, sizeof(int),0);

	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, gpu_remainingnodes, sizeof(int), 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error gpu_remainingnodes is not mapped!" << get_error_string(readerror) << endl;

	*gpu_remainingnodes = numofnodes;
	cout << "Initilizing gpu_remainingnodes!" << endl;
	
	readerror = clEnqueueSVMUnmap(commandQueue,gpu_remainingnodes, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping gpu_remainingnodes!" << get_error_string(readerror) << endl;

////////////////////////////////////////////////////////////////////
	int step = 0;
///////////////////////////////////////////////////////////////////////
	
	int *counter_gpu = (int*)clSVMAlloc(context,CL_MEM_READ_WRITE, sizeof(int)*numofnodes, 0); // new int[numofnodes]; 
	//this will be used to store counter information for every gpu thread
	readerror = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_WRITE, counter_gpu, sizeof(int)*numofnodes, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error counter_gpu is not mapped!" << get_error_string(readerror) << endl;

	std::fill_n(counter_gpu, numofnodes,0);	
	cout << "Initializing counter_gpu to Zero!" << endl;

	readerror = clEnqueueSVMUnmap(commandQueue,counter_gpu, 0, NULL, NULL);
		if(readerror != CL_SUCCESS) cout << "Error for unmapping counter_gpu!" << get_error_string(readerror) << endl;

	cout << "Inputs (buffers) are created!" << endl;

	cl_int error;
	cl_int error2;
	/* Step 8: Create Kernel Object*/
	cl_kernel kernel = clCreateKernel(program,"mis_parallel_async",&error);
	cl_kernel kernel_deactivate = clCreateKernel(program_deactivate,"deactivate_neighbors",&error2);
	
	if (error != CL_SUCCESS) cout << "Error kernel = " << get_error_string(error) << endl;
	if (error2 != CL_SUCCESS) cout << "Error kernel_deactivate = " << get_error_string(error2) << endl;
	
	/* Step 9: Sets Kernel Arguments*/
	status = clSetKernelArgSVMPointer(kernel,0,(void*)counter_gpu);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg 1 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel,1,(void*)nodes);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg 2 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel,2,(void*)nodes_randvalues);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg 3 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel,3,(void*)nodes_status_parallel);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg 4 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel,4,(void*)index_array);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg 5 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel,5,(void*)nodes_execute);
	//mis_parallel_async(counter_gpu,nodes,nodes_randvalues,nodes_status_parallel, index_array,nodes_execute,lparm); 
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg 6 = " << get_error_string(status) << endl;
		

	status = clSetKernelArgSVMPointer(kernel_deactivate,0,(void*)nodes);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg deactivate 1 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel_deactivate,1,(void*)nodes_randvalues);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg deactivate 2 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel_deactivate,2,(void*)nodes_status_parallel);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg deactivate 3 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel_deactivate,3,(void*)gpu_remainingnodes); 
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg deactivate 4 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel_deactivate,4,(void*)index_array);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg deactivate 5 = " << get_error_string(status) << endl;
	status = clSetKernelArgSVMPointer(kernel_deactivate,5,(void*)nodes_execute);
		if (status != CL_SUCCESS) cout << "Error kernel kernelarg deactivate 6 = " << get_error_string(status) << endl;
	//deactivate_neighbors(nodes,nodes_randvalues,nodes_status_parallel,&gpu_remainingnodes, index_array,nodes_execute,lparm);
	
	/*Step 10 = Rand and Call GPU*/

	cl_uint compute_units;
	size_t threadnumber;
	clGetDeviceInfo(devices[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, NULL);
	cout << "Compute Units =" << compute_units << endl;
	
	//cl_uint *in;
	threadnumber = 256 * compute_units;
	cl_int errormap;
	int blocker = 10;	
//	cl_event ndrEvt;
	int locked = 0;
	while(gpu_remainingnodes > 0 && blocker > 0)
	{
		if(locked == 1)
		{
		status = clEnqueueSVMUnmap(commandQueue, gpu_remainingnodes,0,NULL,NULL);
		locked = 0;
		}
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
		
		status = clFlush(commandQueue);
			if(status != CL_SUCCESS) cout << "Error Flushing First = " << get_error_string(status) << endl;
	
//		status = waitForEventAndRelease(&ndrEvt);
//			if(status != CL_SUCCESS) cout << "Error WaitForEvent 1 = " << get_error_string(status) << endl;
	
		status = clEnqueueNDRangeKernel(commandQueue, kernel_deactivate, 1, NULL, &threadnumber, NULL, 0,NULL,NULL);
		cout << "Deactivation kernel sent!" << endl;
			if (status != CL_SUCCESS) cout << "Error kernel_deactivate call! = " << get_error_string(status) << endl;
	
		status = clFlush(commandQueue);
			if (status != CL_SUCCESS) cout << "Error flushing second  = " << get_error_string(status) << endl;
		cout << "flush sent after deactivation" << endl;	

//		status = waitForEventAndRelease(&ndrEvt);
//			if(status != CL_SUCCESS) cout << "Error WaitForEvent 2 = " << get_error_string(status) << endl;
	
	//	barrier(CLK_GLOBAL_MEM_FENCE);
	//	in = (cl_uint *) clEnqueueMapBuffer(commandQueue,buffer_gpu_remainingnodes,CL_TRUE,CL_MAP_READ,0,sizeof(int),0,NULL,NULL,&errormap);
	//		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;

		clFinish(commandQueue);
	
		errormap = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_READ, gpu_remainingnodes, sizeof(int), 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
		else locked = 1;
	//gpu_remainingnodes = (int)(*in);
	cout<<"before print"<<endl;
	printf("GPU Remaining Nodes = %d\n", *gpu_remainingnodes);
	blocker--;
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

	errormap = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_READ, nodes_randvalues, sizeof(int)*numofnodes, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
	errormap = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_READ, nodes_status_parallel, sizeof(int)*numofnodes, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
	errormap = clEnqueueSVMMap(commandQueue, CL_TRUE, CL_MAP_READ, nodes_execute, sizeof(int)*numofnodes, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
			

	cout << "End of the execution, clFinish has been satisfied!" << endl;
	//Print out the result
	for(int l=0; l<numofnodes; l++)
	{
		printf("Rand[%d]= %.5f -- ",l,nodes_randvalues[l]);
		printf("Status[%d] = %d",l,nodes_status_parallel[l]);
		printf("Execute[%d] = %d\n",l,nodes_execute[l]);
	}

	errormap = clEnqueueSVMUnmap(commandQueue,nodes_randvalues, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
	errormap = clEnqueueSVMUnmap(commandQueue,nodes_status_parallel, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
	errormap = clEnqueueSVMUnmap(commandQueue, nodes_execute, 0, NULL, NULL);
		if(errormap != CL_SUCCESS) cout << "Error for reading back" << get_error_string(errormap) << endl;
	
	return 0;
}
