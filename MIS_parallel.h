/* HEADER FILE GENERATED BY snack VERSION 0.8.0 */
/* THIS FILE:  /home/jenny/git_project/GPU_CPU_Sync_Optimization/MIS_parallel.h  */
/* INPUT FILE: /home/jenny/git_project/GPU_CPU_Sync_Optimization/MIS_parallel.cl  */
#ifdef __cplusplus
#define _CPPSTRING_ "C" 
#endif
#ifndef __cplusplus
#define _CPPSTRING_ 
#endif
#ifndef __SNK_DEFS
#define SNK_MAX_STREAMS 8 
extern _CPPSTRING_ void stream_sync(const int stream_num);

#define SNK_MAXEDGESIN 10
#define SNK_MAXEDGESOUT 10
#define SNK_ORDERED 1
#define SNK_UNORDERED 0
#define SNK_GPU 0
#define SNK_SIM 1
#define SNK_CPU 2

typedef struct snk_lparm_s snk_lparm_t;
struct snk_lparm_s { 
   int ndim;                         /* default = 1 */
   size_t gdims[3];                  /* NUMBER OF THREADS TO EXECUTE MUST BE SPECIFIED */ 
   size_t ldims[3];                  /* Default = {64} , e.g. 1 of 8 CU on Kaveri */
   int stream;                       /* default = -1 , synchrnous */
   int barrier;                      /* default = SNK_ORDERED */
   int acquire_fence_scope;          /* default = 2 */
   int release_fence_scope;          /* default = 2 */
   int num_edges_in;                 /*  not yet implemented */
   int num_edges_out;                /*  not yet implemented */
   int * edges_in;                   /*  not yet implemented */
   int * edges_out;                  /*  not yet implemented */
   int devtype;                      /*  not yet implemented-default=SNK_GPU */
   int rank;                         /*  not yet implemented-used for MPI work sharing */
} ;

/* This string macro is used to declare launch parameters set default values  */
#define SNK_INIT_LPARM(X,Y) snk_lparm_t * X ; snk_lparm_t  _ ## X ={.ndim=1,.gdims={Y},.ldims={64},.stream=-1,.barrier=SNK_ORDERED,.acquire_fence_scope=2,.release_fence_scope=2,.num_edges_in=0,.num_edges_out=0,.edges_in=NULL,.edges_out=NULL,.devtype=SNK_GPU,.rank=0} ; X = &_ ## X ;
 
/* Equivalent host data types for kernel data types */
typedef struct snk_image3d_s snk_image3d_t;
struct snk_image3d_s { 
   unsigned int channel_order; 
   unsigned int channel_data_type; 
   size_t width, height, depth;
   size_t row_pitch, slice_pitch;
   size_t element_size;
   void *data;
};

#define __SNK_DEFS
#endif
extern _CPPSTRING_ void mis_parallel(int* nodes,float* nodes_randvalues,int* nodes_status,int* indexarray,int* execute, const snk_lparm_t * lparm);
extern _CPPSTRING_ void mis_parallel_async(int* nodes,float* nodes_randvalues,int* nodes_status,int* indexarray,int* execute,int* ready, const snk_lparm_t * lparm);
extern _CPPSTRING_ void deactivate_neighbors(int* nodes,float* nodes_randvalues,int* nodes_status,int* remaining_nodes,int* indexarray,int* execute, const snk_lparm_t * lparm);
extern _CPPSTRING_ void removeneig(int* nodes,short int* nodes_status,int* numofnodes,int* indexarray, const snk_lparm_t * lparm);
