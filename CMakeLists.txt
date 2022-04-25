cmake_minimum_required (VERSION 3.9)
project (rilib VERSION 0.1 DESCRIPTION "A general purpose image processing library" LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(SOURCE
    src/matrix.cpp
    src/python_packer.cpp
    )

find_package(Python COMPONENTS Interpreter Development REQUIRED)
#find_package(PythonLibs 3.10 REQUIRED)
# find_package(LAPACK REQUIRED)

add_subdirectory(pybind11)

message("1 CMake module path is now ${CMAKE_MODULE_PATH}")
pybind11_add_module(rilib src/python_packer.cpp src/matrix.cpp)
# add_library(rilib SHARED
#     Python.h
#     "${SOURCE}"
#     )

# if(MKL_FOUND)
#     target_include_directories(rilib PUBLIC ${MKL_INCLUDE_DIRS})
#     target_link_libraries(rilib ${MKL_CORE_LIBRARY})
# else()
#     message(WARNING "MKL not found")
# endif()

target_include_directories(rilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(rilib PUBLIC "${PYTHON_INCLUDE_DIRS}")


