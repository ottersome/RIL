#pragma once
#include <memory>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream> 
#include <chrono>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

using namespace std::chrono;
namespace py = pybind11;

#define EQ_DIFF 1.0e-5


//Lets do a macro to do handle the above

//We want the data of this to be flexible

#define getCppTypeCase(DType)\
    case RGBA_8BIT: using cpp_dtype =  std::uint32_t; break;\
    case ABGR_8BIT: using cpp_dtype = ::uint32_t; break;

#define getCppType(DType) \
        getCTypeCase(DType)\
    }

namespace RIL{

    class Image {

        public:

            enum DataType{
                RGBA_8BIT,
                ABGR_8BIT//Some Nonsense here
            };

            static Image image_from_pybuffer(py::buffer &b);
            Image(size_t nrow, size_t ncol,size_t channels = 3, DataType dtype = RGBA_8BIT);
            //Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec);
            // Image(size_t nrow, size_t ncol, size_t channels,const double * vec);

            // Image & operator=(std::vector<double> const & vec);
            Image(Image const & other);
            Image & operator=(Image const & other);
            Image(Image && other);
            Image & operator=(Image && other);
            ~Image();

            Image get_bw();


            template<typename T>
            T const & operator() (size_t row, size_t col,size_t chan) const{
                auto buff = static_cast<T>(m_buffer);
                return buff[row*m_ncol*nchan + col*m_nchan + chan];
            };
            template<typename T>
            double & operator() (size_t row, size_t col,size_t chan){
                const auto buff = static_cast<T>(m_buffer);
                return buff[row*m_ncol*nchan + col*m_nchan + chan];
            }

            bool operator==(const Image & other) const;

            //double get(size_t row, size_t col) const;
            double & get(size_t row, size_t col) ;

            //For convenience
            const size_t  & nrow;
            const size_t  & ncol;
            const size_t  & nchan;

            //For python members
            size_t get_nrow() const{ return m_nrow;}
            size_t get_ncol() const { return m_ncol;}

            size_t size() const;
            double buffer(size_t i) const; std::vector<double> buffer_vector() const;

            static Image to_YUV(Image input_image);
            
            template<class T>
            py::array_t<T> get_ndarray();

            //Arithmetic Operations
            Image multiply_naive(const Image & mat2) const;
            Image multiply_mkl(const Image & mat2) const;

            //template<size_t ts_bytes>
            Image multiply_tile(const Image & mat2, size_t ts) const{

                return Image(1,1,1);//Just for now

                //// auto start = high_resolution_clock::now();
                //if (this->ncol != mat2.nrow)
                //{
                //throw std::out_of_range(
                //"the number of first matrix column "
                //"differs from that of second matrix row");
                //}

                ////Create matrix to return and zero out its elements
                //Image retMat(this->nrow, mat2.ncol,3);
                //retMat.zero_out();

                //size_t tile_vbound =0, tile_hbound=0,op_bound=0;
                //// auto b4_loop = high_resolution_clock::now();

                //// auto aft_loop = high_resolution_clock::now();

                ////Go around Cells in increments of their respective size
                //for(size_t rtile = 0; rtile < this->nrow;rtile+=ts)
                //{
                //tile_vbound =  std::min(ts+rtile, retMat.nrow);
                //for(size_t t_col = 0;t_col< mat2.ncol;t_col+=ts)
                //{
                //tile_hbound = std::min(ts+t_col, mat2.ncol);
                ////For every vertical tile in matrix 2(of the ctil tile column)
                //for(size_t vtile=0;vtile<mat2.nrow;vtile+=ts)
                //{
                ////Actually do cell by cell dot product
                //op_bound = std::min(ts+vtile, (this)->ncol);
                //for(size_t elem = vtile; elem< op_bound;elem++)
                //{
                //for(size_t row = rtile;row<tile_vbound;row++)
                //{
                //for(size_t col = t_col;col<tile_hbound;col++)
                //{
                //// rtr = rtile+row;
                //retMat(row,col)  += (*this)(row,elem)
                //* mat2(elem,col);
                //}
                //}
                //}
                //}

                //}

                //}

                //// auto prep_duration = duration_cast<milliseconds>(b4_loop-start);
                //// auto loop_duration = duration_cast<milliseconds>(aft_loop-b4_loop);
                //// auto tot_duration = duration_cast<milliseconds>(aft_loop-start);
                //// std::cout << "Execution Times:\n"
                ////     <<"\tTotalDuration"<<tot_duration.count()<<"\n"
                ////     <<"\tprep_duration"<<prep_duration.count()<<"\n"
                ////     <<"\tloop_duration"<<loop_duration.count()<<"\n"
                ////     <<std::endl;

                //return retMat;

            }


            //Return pointer to Data
            template<class T>
            T * data();
            template<class T>
            T * data() const;

            //Helpers
            void zero_out();

            //Info
            void print_vals();

        protected:

            size_t index(size_t row, size_t col) const;

            void reset_buffer(size_t nrow, size_t ncol, size_t nchannels);

            size_t m_nrow = 0;
            size_t m_ncol = 0;
            size_t m_nchan = 0;
            // double * m_buffer = nullptr;
            std::shared_ptr<void> m_buffer = nullptr;
            //This helps us know how to deal with the data when processing it
            DataType _data_type;
    };

}
