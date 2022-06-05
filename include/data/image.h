#pragma once
#include <memory>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 
#include <chrono>
#include "data/abs_image.h"

#define EQ_DIFF 1.0e-5

namespace RIL{

    class Image : public AbstractImage<std::uint32_t>{
        public:
            using dtype = std::uint32_t;

            enum DataType{
                RGBA_8BIT,
                ABGR_8BIT//Some Nonsense here
            };

            static Image image_from_pybuffer(py::buffer &b);
            Image(size_t nrow, size_t ncol,size_t channels = 3, DataType dtype = RGBA_8BIT);
            //Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);
            // Image(size_t nrow, size_t ncol, size_t channels,const double * vec);

            // Image & operator=(std::vector<double> const & vec);

            Image get_bw();


            bool operator==(const Image & other) const;

            //double get(size_t row, size_t col) const;
            double & get(size_t row, size_t col) ;

            //For convenience
            //const size_t  & nrow;
            //const size_t  & ncol;
            //const size_t  & nchan;

            //For python members
            size_t get_nrow() const{ return _nrow;}
            size_t get_ncol() const { return _ncol;}

            size_t size() const;
            double buffer(size_t i) const; std::vector<double> buffer_vector() const;

            static Image to_YUV(Image input_image);
            
            template<class T>
            py::array_t<T> get_ndarray();

            //Arithmetic Operations
            Image multiply_naive(const Image & mat2) const;
            Image multiply_mkl(const Image & mat2) const;

            //Return pointer to Data

            //Helpers
            void zero_out();

            //Info
            void print_vals();

        protected:

            size_t index(size_t row, size_t col) const;

    };

}
