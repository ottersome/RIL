#include "algos/filters/gaussian.h"
#include "configs/configs.h"//For configuring kernel paths
#include <string.h>

 using namespace RIL::Filters;


Image & gaussian_filter(Image & img, const Kernel *& krn){
    //Get the location
    auto config = Configs::Get();
    std::string kernel_path = config->getKernelPath() + "./gaussian.cl";
    //Load Programl
    getOpenCLProgram(kernel_path);
    
    //Compile
    
    //Create Mememory Buffers

    //Enqueue it

    //Retrieve it
}
