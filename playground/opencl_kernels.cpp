// #defin CL_TARGET_OPENCL_VERSION 120
#include <CL/opencl.hpp>
#include <iostream>
#include <vector>
#define SIZE 100

std::string program_string{
R"(

)"
};

int main(){

    std::vector<double> h_a(SIZE), h_b(SIZE), h_c(SIZE);

    //Populate a and b 
    for(size_t i = 0;i<SIZE;i++){
        h_a[i] = i;
        h_b[i] = 200;
    }

    std::cout << "Now Setting up the OPENCL Compute Environment" <<std::endl; 

    //Creating a Context
    // cl::Context context(CL_DEVICE_TYPE_DEFAULT);
    const cl::Context context(CL_DEVICE_TYPE_DEFAULT);

    //Create Command QUeue
    cl::CommandQueue queue(context);

    cl::Program program(context,program_string,true);
    //Set up Memory  Objects
    
    using iterator_type = std::vector<double>::iterator;
    //I belive these ones fetch their own context
    cl::Buffer d_a(h_a.begin(), h_a.end(),true);
    cl::Buffer d_b(h_b.begin(), h_b.end(),true);
    cl::Buffer d_c(context, CL_MEM_READ_WRITE,sizeof(double)*SIZE);

    // cl::copy();

}
