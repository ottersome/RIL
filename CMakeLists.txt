cmake_minimum_required (VERSION 3.9)
project (rilib VERSION 0.1 DESCRIPTION "A general purpose image processing library" LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(KERNELS_PATH "${CMAKE_SOURCE_DIR}/src/kernels")
set(KERNELS 
    "${KERNELS_PATH}/gaussian.cl"
    )
# Adding my COmpile Commands 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")
set(SOURCE
    src/image.cpp
    src/python_packer.cpp
    )

find_package(Python COMPONENTS Interpreter Development REQUIRED)
# set(CMAKE_PREFIX_PATH $ENV{HOME}/Projects/Libraries/OpenCL/share/cmake/OpenCL)
set(CMAKE_PREFIX_PATH "/usr/include;/usr/lib")
find_package(OpenCL REQUIRED)
#find_package(PythonLibs 3.10 REQUIRED)
# find_package(LAPACK REQUIRED)

add_subdirectory(pybind11)

pybind11_add_module(rilib 
    src/python_packer.cpp src/image.cpp src/algos/filters/guassian.cpp)

# Add Choices for Debugging and Release Compilations

target_include_directories(rilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(rilib PUBLIC "${PYTHON_INCLUDE_DIRS}")
target_include_directories(rilib PUBLIC  ${OPENCL_INCLUDE_DIR} )


message("OpenCL includes : ${OPENCL_INCLUDE_DIR}")
message("OpenCL Library : ${OPENCL_LIBRARY}")
target_link_libraries(rilib PUBLIC ${OPENCL_LIBRARY})

#file(GLOB_RECURSE ${MISC_SOURCE_FILES} "./playground/*.cpp")
file(GLOB MISC_SOURCE_FILES playground/*.cpp )

# Include CMake Dependencies here
# TODO, but I'll  organize that later
include(cmake/Dependencies.cmake)

# Move Kernels to runnable directory
configure_fiel()

# For test files
message("Adding Extra Executables")
foreach(SOURCE_PATH ${MISC_SOURCE_FILES})
    message("Adding ${SOURCE_PATH}")
    get_filename_component(EXECUTABLE_NAME ${SOURCE_PATH} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_PATH})

    target_include_directories(${EXECUTABLE_NAME} PUBLIC  ${OPENCL_INCLUDE_DIR} )
    target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${OPENCL_LIBRARY})
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC CL_HPP_TARGET_OPENCL_VERSION=300)

endforeach(SOURCE_PATH ${MISC_SOURCE_FILES})

