#pragma once
//In order for me to learn how the sdk works itself

#include <exception>
#include <memory>
#include <mutex>
#include <CL/cl.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <unordered_map>

namespace RIL{ namespace Util{

template<class T>
class CLWrapper{

    public:
        using cl_type = T;

        cl_type get_og(){
            return _clObj;
        }
        CLWrapper():_clObj(NULL){
        };

        //Constuctor time
        CLWrapper(const cl_type & obj, bool shouldRetain)
            :_clObj(obj),shouldRetain(shouldRetain){}
        CLWrapper(const cl_type & obj):_clObj(obj){}
        ~CLWrapper(){
            freeClObj();
        }
        CLWrapper & operator=(const CLWrapper<cl_type> & lhs){
            if(this != &lhs){//Not the same
                _clObj = lhs._clObj;
            }
            return *this;
        }
        CLWrapper & operator=(CLWrapper<cl_type> && rhs){
            std::swap(_clObj,rhs._clObj);
            return *this;
        }
        CLWrapper(CLWrapper<cl_type> & lhs){
            _clObj = lhs._clObj;
        }
        CLWrapper(CLWrapper<cl_type> && child_rhs){
            std::swap(_clObj,child_rhs._clObj);
        }

        const cl_type & operator()() const { return _clObj;}
        const cl_type & get() const { return _clObj;}
    protected:
        cl_type _clObj = NULL;
    private:

        //TODO: Figure out hoqw to use this properly
        bool shouldRetain;

        void freeClObj();
};

//Forward Declarations
class CLDevice;
class CLContext;
class CLPlatform;
class CLQueue;
class CLProgram;
class CLKernel;

class CLDevice : public Util::CLWrapper<cl_device_id>{

    static std::once_flag _default_initialized;
    static CLDevice _default;
    static cl_uint _cl_err;

    private:

    static void make_default();

    public:

    static CLDevice & get_default(){
        std::call_once(_default_initialized, make_default);
        //TODO: check/produce this error
        if(_cl_err){
        }
        return _default;
        //We should get the default platform and then just get \
        //the first device from it
    }

    CLDevice():CLWrapper<cl_device_id>(){}
    //Note: This will mainly be birthed by CLPlatform so dont worry much about it
    CLDevice(const cl_device_id & device, bool shouldRetain = false):
        CLWrapper<cl_device_id>(device,shouldRetain){
            //TODO: figure this retaintion argument out
        } 

};
class CLPlatform : 
    public Util::CLWrapper<cl_platform_id>{

    private:
        static std::once_flag  _is_initialized;
        static CLPlatform _default;
        static cl_uint _cl_err;

        
        static void make_default() {           //Initialize here
            try{
                cl_uint numPlatforms = 0;
                cl_int cl_err = 0;
                cl_err= clGetPlatformIDs(0 , NULL, &numPlatforms);
                if(cl_err != CL_SUCCESS){
                    std::cerr << "Error finding Platform IDs"<<std::endl;
                    return;
                }
                if(numPlatforms == 0){
                    std::cerr << "Not enough devices" <<std::endl;
                    return;
                }
                //ALlocate some space for new platforms
                std::vector<cl_platform_id> plat_ids(numPlatforms);
                //Actually get the Ids
                cl_err = clGetPlatformIDs(numPlatforms, plat_ids.data(), NULL);
                if(cl_err!=CL_SUCCESS){
                    std::cerr << "ERROR: Could not find ids of platforms"<<std::endl;
                    return;
                }
                _default = CLPlatform(plat_ids[0]);
            }catch(std::exception &e){
                //TODO Do something with this error
                // and make it more specific to opencl
            }
        }

    public:
        static CLPlatform & get_default(){
            std::call_once(_is_initialized,make_default);
            //TODO Do a better job at erro checking
            if(_cl_err){
            }
            return _default;
        }
        //We dont take an empty constructor
        CLPlatform() = delete;

        CLPlatform(const cl_platform_id & id):
            CLWrapper<cl_platform_id>(id)
        {
        }
        CLPlatform & operator=(CLPlatform &&rhs){
            CLWrapper<cl_type>::operator=(std::move(rhs));
            return *this;
        }
        CLPlatform & operator=(CLPlatform &rhs){
            CLWrapper<cl_type>::operator=(rhs);
            return *this;
        }

        //TODO maybe allow CPU to be our main choice
        //TODO 2: Return error int instead of vector and use a return parameter for vector
        std::vector<CLDevice> get_devices(){

            //Look for GPUs as preferences
            cl_int err         = 0;
            cl_uint numDevices = 0;
            //Query Number of devices
            err = clGetDeviceIDs(_clObj, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
            cl_device_id *devices;
            if(numDevices == 0){
                std::cerr << "Out of luck, no GPU avaiable for you"<<std::endl;
                std::cout << "Defaulting to a CPU device instead..."<<std::endl;

                err = clGetDeviceIDs(_clObj, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
                devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
                err = clGetDeviceIDs(_clObj,CL_DEVICE_TYPE_CPU,numDevices,devices,NULL);
            }else{
                std::cout << "Found a GPU, will be using it from now on"<<std::endl; 
                devices= (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
                err= clGetDeviceIDs(_clObj, CL_DEVICE_TYPE_GPU,numDevices,devices, NULL);
            }
            //Once we have our list of devices we can create our c++ objects
            std::vector<CLDevice> return_devices;
            for(size_t i = 0;i<numDevices;i++){
                return_devices.emplace_back(CLDevice(devices[i],false));
            }
            return return_devices;

        }
        CLDevice get_first_device(){return get_devices()[0];}
        //TODO: Implement some quick information retrieval from platform

};
//Define it here because we need platform
void CLDevice::make_default(){
    CLPlatform & def_platform = CLPlatform::get_default();
    _default = def_platform.get_first_device();
}
//TODO we define these later as wee need them
class CLContext : public Util::CLWrapper<cl_context>{
    //Context for a specific device
    private:
        static std::once_flag _default_initialized;
        static CLContext _default;
        static cl_int cl_err;


    static void make_default(){
        //We can get default devices here, these we should be able to get from platform
        const CLDevice defdev = CLDevice::get_default();
        //Crete A defult context through move constructor
        _default = CLContext(defdev);
    }
    public:

    static CLContext & get_default(){
        std::call_once(_default_initialized, make_default,NULL);
        if(cl_err){
            std::cerr <<" Error while making default context"<<std::endl;
        }
        return _default;
    }

    CLContext &  operator= (const CLContext & ctx){
        CLWrapper<cl_type>::operator=(ctx);
        return *this;
    }
    CLContext &  operator= (CLContext && ctx){
        CLWrapper<cl_type>::operator=(std::move(ctx));
        return *this;
    }

    CLContext(const std::vector<CLDevice>& devices){
        cl_int cl_error=0;
        
        //Get back an array c can deal with
        auto it = devices.begin();
        cl_device_id * device_ids = (cl_device_id*) malloc(devices.size()*sizeof(cl_device_id));

        size_t idx = 0;
        while(it != devices.end()){
            device_ids[idx++] = it->get();
            it++;
        }


        //Get Context
        _clObj = clCreateContext(
                NULL,//TODO: Perhaps change this
                (cl_uint) devices.size(),
                device_ids,
                NULL,//Implement this pfn_notify
                NULL,
                &cl_error
                );

        if(cl_error != NULL){
            std::cerr<<"There was a rpoblem creating your conrtext"<<std::endl;
        }
        delete[]  device_ids;
    }
    CLContext(const CLDevice & device){
        cl_int cl_error=0;
        
        //Get back an array c can deal with(only the single one
        cl_device_id  device_id = device.get();

        //Get Context
        _clObj = clCreateContext(
                NULL,//TODO: Perhaps change this
                (cl_uint) 1,
                &device_id,
                NULL,//Implement this pfn_notify
                NULL,
                &cl_error
                );

        if(cl_error != NULL){
            std::cerr<<"There was a rpoblem creating your conrtext"<<std::endl;
        }

        //TODO: maybe some copy assignment ?
    }
    //TODO: ?
    CLContext(CLContext && rhs):CLWrapper<cl_context>(std::move(rhs)){}
};
class CLQueue : public Util::CLWrapper<cl_command_queue>{
    private:
    static std::once_flag _default_initialized;
    static CLQueue _default;
    static cl_int _default_error;

    static void make_default(){
        try{
            cl_int cl_err;

            //Get the Defautl Context
            //TODO: do some error checking here
            CLContext & defctx = CLContext::get_default();
            CLDevice & defdev = CLDevice::get_default();
            _default = CLQueue(defctx,defdev,0,&_default_error);

        }catch(std::exception &e){
            std::cerr<<"We got an error here"<<std::endl;

        }

    }
    public:

    static CLQueue & get_default(){
        std::call_once(_default_initialized,make_default);
        if(_default_error){//TODO
        }
        return _default;
    }

    CLQueue(CLContext & context,
            CLDevice & device,
            cl_command_queue_properties q_properties = 0,
            cl_int* cl_err =NULL){

            //We are assuming opengl 3.0 for now
            cl_queue_properties properties[] = {
                CL_QUEUE_PROPERTIES,q_properties,0};
            _clObj = clCreateCommandQueueWithProperties(context.get(),device.get(), properties, cl_err);
            ///Check for error 
            if(cl_err!=NULL){
            }
        }
};

class CLKernel: public CLWrapper<cl_kernel>{
    //I guess this is for the most part just holding info and facilitating  info

    public:
    CLKernel(cl_kernel kernel, bool retain):CLWrapper<cl_kernel>(kernel,retain){ }

    template<typename T, class D>
    cl_int setArg(cl_uint index, const std::unique_ptr<T,D> &args){
        clSetKernelArgSVMPointer(_clObj, index, args.get());
        //TODO: handle errors
    }

};
class CLProgram: public CLWrapper<cl_program>{

    public:

        CLProgram(const std::string & source, bool build, cl_int * err){

            const char * c_source = source.c_str();
            const size_t source_length = source.size();

            //Since no default context given we crete our own
            CLContext & ctx = CLContext::get_default();

            //We try to create the program
            _clObj = clCreateProgramWithSource(ctx.get(), (cl_uint)1, &c_source, &source_length, err);


            //For now this will do
            if(err == CL_SUCCESS && build){

                //Get default device
                CLDevice & device = CLDevice::get_default();
                cl_device_id device_id = device.get();

                cl_device_id * ptr = &device_id;

                *err = clBuildProgram(
                        _clObj,
                        (cl_uint)1,
                        ptr,
                        NULL,
                        NULL,
                        NULL); 
                //Maybe have the build log here
            }
        }
        //Crete Kernels
        std::unordered_map<std::string,CLKernel> getKernels(){
            //Get number of kernels
            std::unordered_map<std::string, CLKernel> return_map;
            //Get number of kernels
            cl_uint num_kernels;
            cl_int err = clCreateKernelsInProgram(_clObj, 0, NULL, &num_kernels);
            if(err != CL_SUCCESS){
                std::cout << "Could not get number of Kernels from program"<<std::endl;
            }
            //Actually Create all these kernels
            std::vector<cl_kernel> kernels(num_kernels);
            err = clCreateKernelsInProgram(_clObj, num_kernels, kernels.data(), NULL);
            if(err != CL_SUCCESS){
                std::cerr << "Could not get kernels from program"<<std::endl;
            }

            for(size_t i = 0;i < num_kernels;i++){
                cl_kernel kernel = kernels[i];
                //Get thename //TODO oh lord do something about that 100
                size_t stringSize = 100;
                char name[stringSize];
                size_t actual_size;
                err = clGetKernelInfo(kernel,CL_KERNEL_FUNCTION_NAME,stringSize*sizeof(char), name,&actual_size);
                if(err != CL_SUCCESS){
                    std::cerr<< "ERROR: Failed retrieving name of kernel"<<std::endl;
                }
                return_map[std::string(name)] = CLKernel(kernel,false);
            }
            return return_map;
        }
};

}}
