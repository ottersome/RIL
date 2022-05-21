cmake_minimum_required (VERSION 3.9)
project (rilib VERSION 0.1 DESCRIPTION "A general purpose image processing library" LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(SOURCE
    src/image.cpp
    src/python_packer.cpp
    )

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(OPENCL REQUIRED) #TODO find an alternative in case lacking
#find_package(PythonLibs 3.10 REQUIRED)
# find_package(LAPACK REQUIRED)

add_subdirectory(pybind11)

message("1 CMake module path is now ${CMAKE_MODULE_PATH}")
pybind11_add_module(rilib src/python_packer.cpp src/image.cpp)

# Add Choices for Debugging and Release Compilations

target_include_directories(rilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(rilib PUBLIC "${PYTHON_INCLUDE_DIRS}")
target_include_directories(rilib PUBLIC  ${OpenCL_INCLUDE_DIRS} )

target_link_libraries(rilib PUBLIC ${OpenCL_LIBRARIES})

#file(GLOB_RECURSE ${MISC_SOURCE_FILES} "./playground/*.cpp")
file(GLOB MISC_SOURCE_FILES playground/*.cpp )

# For test files
message("Adding Extra Executables")
foreach(SOURCE_PATH ${MISC_SOURCE_FILES})
    message("Adding ${SOURCE_PATH}")
    get_filename_component(EXECUTABLE_NAME ${SOURCE_PATH} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_PATH})

endforeach(SOURCE_PATH ${MISC_SOURCE_FILES})

