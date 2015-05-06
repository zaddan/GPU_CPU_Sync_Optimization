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
#include <iostream>

#include "debug_helpers.h" 

//-----------------------------------------------------------------------------------------------------------------------------
////-----------------------------------------------------------------------------------------------------------------------------
////---------module_name:::showNodesStatus
////---------functionlity:::shows the status of the nodes
////-----------------------------------------------------------------------------------------------------------------------------
////-----------------------------------------------------------------------------------------------------------------------------
void showNodesInfo(int *nodesStatus, float *nodeRandValues, int nodeArraySize, string request){
    //this veriable determines whether we want tow show the complete name of the status or just numbers 
    bool showFullStatusName = true; 
    //change the following if status.h changed 
    //show status 
    if (request == "status") {
        cout<<"***********************************************";
        cout<<"showing all the nodes status was requests"<<endl; 
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        for (int i = 0; i< nodeArraySize; i++){
            if(showFullStatusName) {
                cout<<"node "<< i << "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            }else{
                cout<<"node "<< i << "th status is: " << nodesStatus[i]<<endl;
            }
        }
        cout<<"***********************************************"; 
    }
    //show rand values 
    else if (request == "randValues") {
        cout<<"***********************************************"; 
        cout<<"showing all the nodes status was requests"<<endl; 
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        for (int i = 0; i< nodeArraySize; i++){
            if(showFullStatusName) {
                cout<<"node "<< i << "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            }else{
                cout<<"node "<< i << "th status is: " << nodesStatus[i]<<endl;
            }
        }
        cout<<"***********************************************"; 
    } 
    //show all the info 
    else if (request == "all") {
        showNodesInfo(nodesStatus, nodeRandValues, nodeArraySize, "status");
        showNodesInfo(nodesStatus, nodeRandValues, nodeArraySize, "randValues");
    } 
    else {
        cout<< "this information is not applicable"<<endl;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------
////-----------------------------------------------------------------------------------------------------------------------------
////---------module_name::: writeToFileInfo
////---------functionlity::: writes the status of the nodes to the file
////-----------------------------------------------------------------------------------------------------------------------------
////-----------------------------------------------------------------------------------------------------------------------------
void writeToFileNodeInfo(int *nodesStatus, float *nodeRandValues, int nodeArraySize, string fileName, string request){
    //open the file
    ofstream myfile;
    myfile.open (fileName.c_str(), std::ios_base::app);
    //this veriable determines whether we want to show the complete name of the status or just numbers 
    bool showFullStatusName = true; 

    //change the following if status.h changed 
    //show status 
    if (request == "status") {
        myfile<<"***********************************************";
        myfile<<"showing all the nodes status was requests"<<endl; 
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        for (int i = 0; i< nodeArraySize; i++){
            if(showFullStatusName) {
                myfile<<"node "<< i << "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            }else{
                myfile<<"node "<< i << "th status is: " << nodesStatus[i]<<endl;
            }
        }
        myfile<<"***********************************************"; 
    }
    //show rand values 
    else if (request == "randValues") {
        myfile<<"***********************************************"; 
        myfile<<"showing all the nodes status was requests"<<endl; 
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        for (int i = 0; i< nodeArraySize; i++){
            if(showFullStatusName) {
                myfile<<"node "<< i << "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            }else{
                myfile<<"node "<< i << "th status is: " << nodesStatus[i]<<endl;
            }
        }
        myfile<<"***********************************************"; 
    } 

    //show all the info 
    //
    else if (request == "all") {
        writeToFileNodeInfo(nodesStatus, nodeRandValues, nodeArraySize, fileName, "status");
        writeToFileNodeInfo(nodesStatus, nodeRandValues, nodeArraySize, fileName, "randValues");
    }else {
        myfile<< "this information is not applicable"<<endl;

    }
    //closing the file 
    myfile.close();
}




