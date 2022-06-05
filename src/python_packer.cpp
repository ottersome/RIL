//#include <pybind11/attr.h>
#include <cstdint>
#include <cstdlib>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <iostream>
#include <tuple>
#include <type_traits>

#include "python_packer.h"
#include "algos/filters.hpp"
#include "data/image.h"
#include "data/kernel.h"
#include "algos/colorspaces.hpp"
#include "algos/compression.h"
#include <stdio.h>
#include <assert.h>//TODO: remove when not needed
#include <bits/stdc++.h>
#include <type_traits>


namespace py = pybind11;

//Mostly for tests
Image ident(size_t row,size_t col){

    if (row != col)
    {
        throw std::out_of_range(
            "the number of first Image column "
            "differs from that of second Image row");
    }

    Image mat(row,col,1);
    mat.zero_out();

    for(size_t i=0;i<row;i++){
        mat(i,i,0) = 1;
    }

    return mat;
}

Image multiply_naive(const Image &m1, const Image &m2){
    return m1.multiply_naive(m2);
}
// Image multiply_tile(const Image &m1, const Image &m2,size_t tile_size){
    // return m1.multiply_tile(m2, tile_size);
// }
Image multiply_mkl(Image &m1, const Image &m2){
    return m1.multiply_mkl(m2);
}

PYBIND11_MODULE(rilib, m) {

    auto dtype = Image::DataType::RGBA_8BIT;

    py::class_<Image, std::shared_ptr<Image>>(m, "Image",py::buffer_protocol())
        .def_buffer([](Image &m) -> py::buffer_info {
                //Image internal type
                using img_internal_type = std::remove_pointer<decltype(m.data())>::type;
                auto dtype_size = sizeof(img_internal_type);
                if(!std::is_same<img_internal_type,std::uint16_t>()){//Intentionally wrong, TODO fix this once you confirms it breaks
                    std::cout << "ERROR: Wrong tyeps of containers"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                return py::buffer_info(
                        m.data(),
                        dtype_size,
                        py::format_descriptor<img_internal_type>::format(),
                        3,
                        {m.get_nrow(),m.get_ncol(),m.get_nchan()},
                        {sizeof(dtype_size)*m.get_ncol()*m.get_nchan(),sizeof(dtype_size)*m.get_nchan(),sizeof(double)}
                        );
                })
        .def(py::init<size_t, size_t,size_t>())
        .def("print_vals",&Image::print_vals)
        .def("get_bw",&Image::get_bw)
        .def(py::self == py::self)
        .def_property_readonly("nrow",&Image::get_nrow)
        .def_property_readonly("ncol",&Image::get_ncol)
        // .def("__getitem__", [](const Image &m, const py::slice & slice) {
        //     py::ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
        //     if (!slice.compute(m.size(), &start, &stop, &step, &slicelength)) {
        //         throw py::error_already_set();
        //     }
        //     int istart = static_cast<int>(start);
        //     int istop = static_cast<int>(stop);
        //     int istep = static_cast<int>(step);
        //
        //     //TODO Implement this for when we want to
        //     //deal with slices
        //     return std::make_tuple(istart, istop, istep);
        // })
        //TODO fix for multiple channels
        .def("__getitem__", [](const Image &m, std::tuple<size_t,size_t,size_t> indices) {
                size_t row, col,chan;
                std::tie(row, col,chan) = indices;
            return m(row,col,chan);
        })
        //TODO fix for multiple channels
        .def("__setitem__", [](Image &m, std::tuple<size_t,size_t,size_t> indices, const double value) {
                size_t row, col,chan;
                std::tie(row, col,chan) = indices;
                m(row,col,chan) = value;
        });
        // .def("multiply_tile",Image::multiply_tile);
    m.def("ident",&ident,"Function initializes identity Image");
    m.def("multiply_naive",&multiply_naive,"Naive multiplication");
    m.def("multiply_mkl",&multiply_mkl,"Multiply using MKL method");
    // m.def("multiply_tile",&multiply_tile,"Multiply using Tile method");
    m.def("image_from_pybuffer",Image::image_from_pybuffer,"Get Image from Python Buffer Object");
    m.def("convolve",&Filters::convolve,"Perform on a convolution given a kernel");

    py::class_<Kernel>(m, "Kernel",py::buffer_protocol())
        .def_buffer([](Kernel &k) -> py::buffer_info {
                //Image internal type
                using img_internal_type = std::remove_pointer<decltype(k.data())>::type;
                auto dtype_size = sizeof(img_internal_type);
                if(!std::is_same<img_internal_type,std::uint32_t>()){
                    std::cout << "ERROR: Wrong tyeps of containers"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                return py::buffer_info(
                        k.data(),
                        dtype_size,
                        py::format_descriptor<img_internal_type>::format(),
                        3,
                        {k.get_nrow(),k.get_ncol(),k.get_nchan()},
                        {sizeof(dtype_size)*k.get_ncol()*k.get_nchan(),sizeof(dtype_size)*k.get_nchan(),sizeof(double)}
                        );
                });
    // py::module filters = m.def_submodule("filters","Handles all filtering methods");
    // filters.def("Filters", &F)

}
