#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <CL/cl.h>
#include "opencl_wrapper.hpp"

namespace RIL {
    class OCL_Mgr{

    //Since we only need one context througyout out executions then 
    //we will make uise of statsticity and of Singleton
    //TODO: check the validity of this singleton
    public :
        typedef std::shared_ptr<OCL_Mgr> ptr;
        static ptr sgetInstance(){
            static ptr instance = std::make_shared<OCL_Mgr>();
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
        OCL_Mgr(){
        }
        //Should be called when OCL_Mgr has no references
        ~OCL_Mgr(){
            std::cout << "Destroying our device"<<std::endl;
        }

        /********************
         * Actual Content Here
        ********************/

        //We want to keep track of:
        Util::CLContext context;
        Util::CLQueue queue;
        Util::CLDevice devices;
        Util::CLPlatform platform;

};}
