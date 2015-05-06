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
void showNodesStatus(int *nodesStatus, int nodeArraySize){
    //this veriable determines whether we want tow show the complete name of the status or just numbers 
    bool showFullStatusName = true; 
    //change the following if status.h changed 
    string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
    for (int i = 0; i< nodeArraySize; i++){
        if(showFullStatusName) {
            cout<<"node "<< i << "th status is" << nodeStatusString[nodesStatus[i]];
        }else{
            cout<<"node "<< i << "th status is" << nodesStatus[i];
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------
////-----------------------------------------------------------------------------------------------------------------------------
////---------module_name::: writeToFileNodesStatus
////---------functionlity::: writes the status of the nodes to the file
////-----------------------------------------------------------------------------------------------------------------------------
////-----------------------------------------------------------------------------------------------------------------------------
void writeToFileNodesStatus(int *nodesStatus, int nodeArraySize, string fileName){
    //open the file
    ofstream myfile;
    myfile.open (fileName.c_str());
    //this veriable determines whether we want to show the complete name of the status or just numbers 
    bool showFullStatusName = true; 
    //change the following if status.h changed 
    string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
    for (int i = 0; i< nodeArraySize; i++){
        if(showFullStatusName) {
            myfile <<"node "<< i << "th status is" << nodeStatusString[nodesStatus[i]];
        }else{
            myfile <<"node "<< i << "th status is" << nodesStatus[i];
        }
     } 
    //closing the file 
    myfile.close();
}
