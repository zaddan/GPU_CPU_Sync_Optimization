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
void showNodesInfo(int *nodesStatus, float *nodeRandValues, int * nodesExec, int nodeArraySize, string request){
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
                cout<<"node "<< i  + 1<< "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            }else{
                cout<<"node "<< i + 1 << "th status is: " << nodesStatus[i]<<endl;
            }
        }
        cout<<"***********************************************"; 
    }
    //show rand values 
    else if (request == "randValues") {
        cout<<"***********************************************"; 
        cout<<"showing all the nodes rand values was requests"<<endl; 
        for (int i = 0; i< nodeArraySize; i++){
            cout<<"node "<< i  + 1<< "th status is: " << nodeRandValues[i]<<endl;
        }
        cout<<"***********************************************"; 
    } 
    //show all the info 
    else if (request == "all") {
        //showNodesInfo(nodesStatus, nodeRandValues, nodeArraySize, "status");
        //showNodesInfo(nodesStatus, nodeRandValues, nodeArraySize, "randValues");
        cout<<"***********************************************";
        cout<<"showing all the nodes status was requests"<<endl; 
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        for (int i = 0; i< nodeArraySize; i++){
            cout <<"node "<< i + 1 << ":\t" << nodeStatusString[nodesStatus[i]] << "\t" << nodeRandValues[i] << "\t" << nodesExec[i] <<  endl;
        }
        cout<<"***********************************************"; 
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
void writeToFileNodeInfo(int *nodesStatus, float *nodeRandValues, int *nodesExec, int nodeArraySize, string fileName, string request){
    //open the file
    ofstream myfile;
    myfile.open (fileName.c_str(), std::ios_base::app);
    //this veriable determines whether we want to show the complete name of the status or just numbers 
    bool showFullStatusName = true; 

    //change the following if status.h changed 
    //show status 
    if (request == "status") {
        myfile<<"***********************************************"<<endl;
        myfile<<"showing all the nodes status was requests"<<endl; 
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        for (int i = 0; i< nodeArraySize; i++){
            if(showFullStatusName) {
                myfile<<"node "<< i + 1 << "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            }else{
                myfile<<"node "<< i  + 1<< "th status is: " << nodesStatus[i]<<endl;
            }
        }
        myfile<<"***********************************************"<<endl;; 
        myfile<< "status:" << " "; 
        for (int i = 0; i< nodeArraySize; i++){
            if(showFullStatusName) {
                //myfile<<"node "<< i + 1<< "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
                myfile<< nodeStatusString[nodesStatus[i]]<< " ";
            }
        }
        myfile << endl; 
    }
    //show rand values 
    else if (request == "randValues") {
        myfile<<"***********************************************"<<endl;; 
        myfile<<"showing all the nodes rand values was requests"<<endl; 
        for (int i = 0; i< nodeArraySize; i++){
            //myfile<<"node "<< i + 1<< "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            myfile<<"node "<< i + 1<< "rand value is: " << nodeRandValues[i] << endl;
        }
        myfile<<"***********************************************"<<endl; 
        myfile<< "randValues:" << " "; 
        for (int i = 0; i< nodeArraySize; i++){
            //myfile<<"node "<< i + 1<< "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            myfile<< nodeRandValues[i] << " ";
        }
        myfile<< endl; 
        myfile<<"***********************************************"<<endl;; 
    } 

    //show all the info 
    //
    else if (request == "all") {
        //writeToFileNodeInfo(nodesStatus, nodeRandValues, nodeArraySize, fileName, "status");
        //writeToFileNodeInfo(nodesStatus, nodeRandValues, nodeArraySize, fileName, "randValues");
        string nodeStatusString[3] = {"INACTIVE", "ACTIVE", "SELECTED"};
        myfile<<"***********************************************"<<endl;; 
        myfile<<"showing all the nodes rand values was requests"<<endl; 
        for (int i = 0; i< nodeArraySize; i++){
            //myfile<<"node "<< i + 1<< "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            myfile<<"node "<< i + 1 << ":\t" << nodeStatusString[nodesStatus[i]] << "\t" << nodeRandValues[i] << "\t" << nodesExec[i] <<  endl;
        }
        myfile<<"***********************************************"<<endl; 
        myfile<< "randValues:" << " "; 
        for (int i = 0; i< nodeArraySize; i++){
            //myfile<<"node "<< i + 1<< "th status is: " << nodeStatusString[nodesStatus[i]]<<endl;
            myfile<< nodeRandValues[i] << " ";
        }
        myfile<< endl; 
        myfile<<"***********************************************"<<endl;; 
    }else {
        myfile<< "this information is not applicable"<<endl;

    }
    //closing the file 
    myfile.close();
}




