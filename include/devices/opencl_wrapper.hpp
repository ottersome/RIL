#pragma once
//In order for me to learn how the sdk works itself

#include <CL/cl.h>

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

        CLWrapper(const cl_type & obj):_clObj(obj){}
        ~CLWrapper(){
            freeClObj();
        }
    protected:
        cl_type _clObj;
    private:

        //TODO: Implement Copy and Move Constructor
        

        void freeClObj();
};

//TODO we define these later as wee need them
class CLContext : 
    //By Inheriting from this we 
    //ensure clTypes are handled properly
    public Util::CLWrapper<cl_context>{

        //I think we only want to have one ?


        CLContext(){
            //Will take some stuff
            //and set up a context
        }
};
class CLQueue : 
    public Util::CLWrapper<cl_command_queue>{

        CLQueue();
};
class CLDevice : 
    public Util::CLWrapper<cl_device_id>{

        CLDevice();
};
class CLPlatform : 
    public Util::CLWrapper<cl_device_id>{
        CLPlatform();
};


}}
