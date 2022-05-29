#include <vector>
#include <iostream>
#include <CL/opencl.hpp>
#include <fstream>
#include <sstream>

#define SIZE 500
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

    std::stringstream program_string;
    std::ifstream t("../src/kernels/matmul.c");
    program_string << t.rdbuf();

    cl::Program program(context,program_string.str(),true);
    program.build();
    //Set up Memory  Objects
    
    using iterator_type = std::vector<double>::iterator;
    //I belive these ones fetch their own context
    cl::Buffer d_a(context,h_a.begin(), h_a.end(),true);
    cl::Buffer d_b(context,h_b.begin(), h_b.end(),true);
    cl::Buffer d_c(context, CL_MEM_READ_WRITE,sizeof(double)*SIZE);

    //Create Functo
    // THis method mighht be deprecarted though, not so sure
    //  Yeah, use a KernelFunctor Instead
    cl::compatibility::make_kernel<cl::Buffer,cl::Buffer,cl::Buffer> 
        simple_kernel(program,"simple");

    //SPecify Dimenions
    cl::NDRange global(1024); 
    cl::NDRange local(64); 

    simple_kernel(cl::EnqueueArgs(queue,global),d_a,d_b,d_c);


    //Get Back Result
    cl::copy(queue,d_c,h_c.begin(),h_c.end());

}
