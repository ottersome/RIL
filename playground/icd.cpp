#include <CL/cl.h>

#define CL_TARGET_OPENCL_VERSION 300
#include <string.h>
#include <iostream> #include <fstream>
#include <sstream>
#include <vector>
#define SIZE 10


using namespace std;


int main(int argc, char* argv[]){
    cl_uint numPlatforms;

    cl_platform_id platform = NULL;
    cl_int status = clGetPlatformIDs(0 , NULL, &numPlatforms);

    if(status != CL_SUCCESS){
        std::cerr<<"Oh no there is trouble looking for platforms"<<std::endl;
    }

    if(numPlatforms >0){
        std::cout <<  "Now selecting a platform"<<std::endl;
        cl_platform_id* platforms = 
            (cl_platform_id*) malloc(numPlatforms*sizeof(cl_platform_id));

        status = clGetPlatformIDs(numPlatforms , platforms ,NULL);

        //Well since we can assume that there is more than one platform
        platform = platforms[0];
    }

    cl_uint numDevices = 0;
    cl_device_id *devices;
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU , 0, NULL, &numDevices);
    if(numDevices == 0){
        std::cerr << "Out of luck, no GPU avaiable for you"<<std::endl;
        std::cout << "Defaulting to a CPU device instead..."<<std::endl;
        
        status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
        devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
        status = clGetDeviceIDs(platform,CL_DEVICE_TYPE_CPU,numDevices,devices,NULL);
    }else{
        std::cout << "Found a GPU, will be using it from now on"<<std::endl; 
        devices= (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
        status= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU,numDevices,devices, NULL);
    }

    //Now we create a context

    cl_context context = clCreateContext(NULL, 1,devices,NULL,NULL,NULL);
    // //TODO maybe have a look up on an erro code
    cl_command_queue  commandQueue = clCreateCommandQueueWithProperties(context, devices[0], 0, NULL);

    //Load the file
    std::ifstream streamy("../src/kernels/simple_vector_addition.cl");
    std::stringstream buffer;
    buffer << streamy.rdbuf();

    std::string file_contents{buffer.str()};
    size_t source_size[] = {file_contents.length()} ;
    const char * source = file_contents.c_str();

    cl_program program = clCreateProgramWithSource(context, 1,&source, source_size, NULL);

    //Build the input
    std::vector<float> A(SIZE),
        B(SIZE),C(SIZE);
    for(int i = 0;i<SIZE;i++){
        A[i] = i;
        B[i] = 100;
        C[i] = 0;
    }

    //Build the program
    status = clBuildProgram(program,1,devices,NULL,NULL,NULL);

    cl_int err_code;
    cl_mem buff_A = clCreateBuffer(
            context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
            SIZE*sizeof(float), A.data(), &err_code);
    cl_mem buff_B = clCreateBuffer(
            context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
            SIZE*sizeof(float), B.data(), &err_code);
    cl_mem buff_C = clCreateBuffer(
            context, CL_MEM_WRITE_ONLY,
            SIZE*sizeof(float), NULL, &err_code);

    cl_kernel kernel = clCreateKernel(program,"vadd",NULL);

    //Set up kernel arguments
    status =clSetKernelArg(kernel, 0 ,sizeof(cl_mem),&buff_A);
    status =clSetKernelArg(kernel, 1 ,sizeof(cl_mem),&buff_B);
    status =clSetKernelArg(kernel, 2 ,sizeof(cl_mem),&buff_C);

    size_t global_work_size[] = {SIZE};
    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1,NULL,
            global_work_size,NULL,0,NULL,NULL);

    status = clEnqueueReadBuffer(commandQueue,buff_C, CL_TRUE, 0, 
            SIZE*sizeof(float), C.data(), 0, NULL, NULL);


    std::cout<< "We have finished all of our work, output incoming:"<<std::endl;
    auto it = C.begin();
    int i = 0;
    while(it!=C.end()){
        std::cout << "C["<<i++<<"]: "<<*it<<std::endl;
        it++;
    }

    status = clReleaseKernel(kernel);
    status = clReleaseProgram(program);
    status = clReleaseMemObject(buff_A);
    status = clReleaseMemObject(buff_B);
    status = clReleaseMemObject(buff_C);
    status = clReleaseCommandQueue(commandQueue);
    status = clReleaseContext(context);
    if(devices !=NULL){
        free(devices);
        devices = NULL;
    }
    std::cout <<"Done with the program"<<std::endl;
    return 0;
}
