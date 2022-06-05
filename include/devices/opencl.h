#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <CL/cl.h>
#include "opencl_wrapper.hpp"

namespace RIL {

    //Note:
    //   This manager might be temporary. 
    //   We only need to use it for a little bit after all
    //   Maybe you can think about it as being the default manager
    //   If we need more contexts/platforms we can use specific ones
    class OCL_Mgr{

    //Since we only need one context througyout out executions then 
    //we will make uise of statsticity and of Singleton
    //TODO: check the validity of this singleton
    public :
        typedef OCL_Mgr* ptr;
        static ptr getInstance(){
            static ptr instance = new OCL_Mgr;
            // CLU_ptr ptr = std::make_unique();
            // return ptr;
        }

        //Remove copy/move constructors
        OCL_Mgr operator=(OCL_Mgr &&) = delete;
        OCL_Mgr operator=(const OCL_Mgr & ) = delete;

        OCL_Mgr(OCL_Mgr &&) = delete;
        OCL_Mgr(const OCL_Mgr & ) = delete;

        /********************
         * Actual Content Here
        ********************/
        int createProgramFromSourceFile(const std::string & path);

        cl_int getCommandQueue();


    private:

        //Make Normal Constructor Private
        OCL_Mgr():
            platform(Util::CLPlatform::get_default()),
            device(Util::CLDevice::get_default()),
            context(Util::CLContext::get_default()),
            queue(Util::CLQueue::get_default())
        {
            //We now have the default values;
            if(_ptr == nullptr){
            }
        }
        //Should be called when OCL_Mgr has no references
        ~OCL_Mgr(){
            std::cout << "Destroying our device"<<std::endl;
        }

        //Here we shoudl have aquick program to load

        static OCL_Mgr* _ptr;

        /********************
         * Actual Content Here
        ********************/

        //We want to keep track of:
        Util::CLContext & context;
        Util::CLQueue & queue;
        Util::CLDevice & device;
        Util::CLPlatform & platform;

        //Helpers to build/search programs


};}
