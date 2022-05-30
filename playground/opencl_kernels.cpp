#include "CL/cl.h"
#include <iterator>
#include <stdexcept>
#include <vector>
#include <iostream>
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>
#include <fstream>
#include <valarray>
#include <sstream>
#include <random>

#define SIZE 500
int main(int argc, char * args[], char ** envs){

    try{
        std::vector<float> h_a(SIZE), h_b(SIZE), h_c(SIZE);

        //Read Environment(for path)
        // for(int i = 0 ;envs[i] != NULL; i++){
        //     std::cout << envs[i]<<std::endl;
        // }

        //Populate a and b 
        for(size_t i = 0;i<SIZE;i++){ h_a[i] = i;
            h_a[i] = 0+i;
            h_b[i] = 200+i;
        }

        std::cout << "Now Setting up the OPENCL Compute Environment" <<std::endl; 

        //Creating a Context
        // cl::Context context(CL_DEVICE_TYPE_DEFAULT);
        const cl::Context contexto(CL_DEVICE_TYPE_DEFAULT);
        const cl::Device device = contexto.getInfo<CL_CONTEXT_DEVICES>().at(0);

        //Create Command QUeue
        cl::CommandQueue queue(contexto,device);
        cl::Platform platform{device.getInfo<CL_DEVICE_PLATFORM>()};

        std::cout << "Selected platform: "
            << platform.getInfo<CL_PLATFORM_VENDOR>() << "\n"
            << "Selected device: " << device.getInfo<CL_DEVICE_NAME>()
            << "\n"
            << std::endl;


        //---------------------------
        // Start Compiling the Kernel
        //---------------------------
        //std::string kernel_location = "../src/kernels/simple_vector_addition.cl";
        std::string kernel_location = "../src/kernels/saxpy.cl";
        std::ifstream cl_file{kernel_location};
        if(!cl_file.is_open()){
            throw std::runtime_error{ std::string{"Cannot open the kernel source :"} + kernel_location
            };
        }

        std::string cl_string(
                std::istreambuf_iterator<char>(cl_file), 
                (std::istreambuf_iterator<char>()));

        std::cout << "cl_string:\n" << cl_string<<std::endl;

        //---------------------------
        // Creating the program
        //---------------------------
        cl::Program program(contexto, std::string{
            std::istreambuf_iterator<char>{cl_file},
            std::istreambuf_iterator<char>()}
        );

        auto err = program.build();
            if(err != CL_BUILD_SUCCESS){
                std::cerr << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device) 
                << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
                exit(1);
            }
            
        //Set up Memory  Objects
        //Build info

        //------------------------------
        // Now Actually create the kernel
        //------------------------------

        auto kerny = cl::KernelFunctor<cl_float,cl::Buffer,cl::Buffer>(
            program, "saxpy"
        );

        //-----------------------------------
        // Init host-side storage
        //-----------------------------------

        std::valarray<cl_float> arr_x(SIZE), arr_y(SIZE);

        for(int i = 0 ; i<SIZE;i++){
            arr_x[i] = i;
            arr_y[i]= 100;
        }


        //-----------------------------------
        // Init device-side storage
        //-----------------------------------

        cl::Buffer buf_x{contexto, std::begin(arr_x),std::end(arr_x),true},
            buf_y{contexto,std::begin(arr_y),std::end(arr_y),false};

        kerny(cl::EnqueueArgs{queue, cl::NDRange(SIZE)},1.3,buf_x,buf_y);

        //Create Functo
        //THis method mighht be deprecated though, not so sure
        // Yeah, use a KernelFunctor Instead
        //std::cout <<"Making kernel functor"<<std::endl;
        //auto vadd = cl::Kernel(program,"vadd");
        //vadd.setArg(0,d_a);
        //vadd.setArg(1,d_b);
        //vadd.setArg(2,d_c);
        // auto vadd = cl::compatibility::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer,int>(program,"vector_add");
        //std::cout <<"Finished Making kernel functor"<<std::endl;
        // cl::compatibility::make_kernel<cl::Buffer,cl::Buffer,cl::Buffer>
        //     simple_kernel(program,"vector_add");

        //Specify Dimenions
        // cl::NDRange global(1024);
        // cl::NDRange local(64);/
        //
        // simple_kernel(cl::EnqueueArgs(queue,global),d_a,d_b,d_c);
        // vadd(cl::EnqueueArgs(queue, cl::NDRange(SIZE)),d_a,d_b,d_c);
        //queue.enqueueNDRangeKernel(vadd, cl::NullRange, cl::NDRange(10),cl::NullRange);
        //queue.finish();

        //Get Back Result
        //cl::copy(queue,d_c,h_c.begin(),h_c.end());
        //std::cout << "About to show the resulting values "<< std::endl;
        //for(int i = 0 ; i < 5;i++){
        //    std::cout << "Value("<<h_c[i]<<")" <<std::endl;
        //}
    }catch(cl::BuildError & e){
        std::cerr << "OpenCL runtime error: " << e.what() << std::endl;
        for (auto& build_log : e.getBuildLog())
        {
            std::cerr << "\tBuild log for device: "
                      << build_log.first.getInfo<CL_DEVICE_NAME>() << "\n"
                      << std::endl;
            std::cerr << build_log.second << "\n" << std::endl;
        }
        std::exit(e.err());
    } catch (cl::Error& e)
    {
        std::cerr << "OpenCL rutnime error: " << e.what() << " : " <<e.err()<<std::endl;
        std::exit(e.err());
    } catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

}
