#include <pybind11/attr.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <iostream>
#include <tuple>

#include "python_packer.h"
#include "filters.hpp"
#include "image.h"
#include "kernel.h"


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
Image multiply_tile(const Image &m1, const Image &m2,size_t tile_size){
    return m1.multiply_tile(m2, tile_size);
}
Image multiply_mkl(Image &m1, const Image &m2){
    return m1.multiply_mkl(m2);
}

PYBIND11_MODULE(rilib, m) {

    py::class_<Image>(m, "Image",py::buffer_protocol())
        .def_buffer([](Image &m) -> py::buffer_info {
                return py::buffer_info(
                        m.data(),
                        sizeof(double),
                        py::format_descriptor<double>::format(),
                        3,
                        {m.nrow,m.ncol,m.nchan},
                        {sizeof(double)*m.ncol*m.nchan,sizeof(double)*m.nchan,sizeof(double)}
                        );
                })
        .def(py::init<size_t, size_t,size_t>())
        .def("print_vals",&Image::print_vals)
        .def("get_bw",&Image::get_bw)
        .def(py::self == py::self)
        .def_property_readonly("nrow",&Image::get_nrow)
        .def_property_readonly("ncol",&Image::get_ncol)
        .def("__getitem__", [](const Image &m, const py::slice & slice) {
            py::ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (!slice.compute(m.size(), &start, &stop, &step, &slicelength)) {
                throw py::error_already_set();
            }
            int istart = static_cast<int>(start);
            int istop = static_cast<int>(stop);
            int istep = static_cast<int>(step);

            //TODO Implement this for when we want to 
            //deal with slices
            return std::make_tuple(istart, istop, istep);
        })
        //TODO fix for multiple channels
        .def("__getitem__", [](const Image &m, std::tuple<size_t,size_t> indices) {
                size_t row, col;
                std::tie(row, col) = indices;
            return m(row,col,0);
        })
        //TODO fix for multiple channels
        .def("__setitem__", [](Image &m, std::tuple<size_t,size_t> indices, const double value) {
                size_t row, col;
                std::tie(row, col) = indices;
                m(row,col,0) = value;
        })
        .def("multiply_tile",&Image::multiply_tile);
    m.def("ident",&ident,"Function initializes identity Image");
    m.def("multiply_naive",&multiply_naive,"Naive multiplication");
    m.def("multiply_mkl",&multiply_mkl,"Multiply using MKL method");
    m.def("multiply_tile",&multiply_tile,"Multiply using Tile method");
    m.def("image_from_pybuffer",Image::image_from_pybuffer,"Get Image from Python Buffer Object");
    m.def("convolve",&Filters::convolve,"Perform on a convolution given a kernel");

    py::class_<Kernel>(m, "Kernel",py::buffer_protocol())
        .def_buffer([](Kernel &k) -> py::buffer_info {
                return py::buffer_info(
                        k.data(),
                        sizeof(double),
                        py::format_descriptor<double>::format(),
                        3,
                        {k.nrow,k.ncol,k.nchan},
                        {sizeof(double)*k.ncol,sizeof(double)*k.nchan,sizeof(double)}
                        );
                });
    // py::module filters = m.def_submodule("filters","Handles all filtering methods");
    // filters.def("Filters", &F)

}
