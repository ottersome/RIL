#include <iostream>
#include <iomanip>
#include <pybind11/numpy.h>
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <algorithm>
#include <math.h>
#include "data/image.h"
// #ifdef __unix__
// #include "mkl.h"
// #else
// #include "f77blas.h"
// #endif

namespace py = pybind11;
using namespace RIL;

#include "data/image.h"



/*********************
* Constructors
*********************/

Image::Image(size_t o_nrow, size_t o_ncol,size_t o_nchan, DataType dtype)
    : m_nrow(o_nrow),m_ncol(o_ncol),m_nchan(o_nchan),
    nrow(m_nrow),ncol(m_ncol),nchan(m_nchan)
{
    _data_type = dtype;
    reset_buffer(nrow, ncol,m_nchan);
}
//Matrix::Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
    //: m_nrow(nrow),m_ncol(ncol),nrow(m_nrow),ncol(m_ncol)

//{
    //reset_buffer(nrow, ncol);
    //(*this) = vec;
//}

//TODO : This might end up being a numpy ptr instead of a double ptr
// Image::Image(size_t o_nrow, size_t o_ncol, size_t o_nchan, const double * ptr,DataType dtype)
//     : m_nrow(o_nrow),m_ncol(o_ncol),m_nchan(o_nchan),
//     nrow(m_nrow),ncol(m_ncol),nchan(m_nchan)
// {
//     reset_buffer(o_nrow, o_ncol,o_nchan);
//     //TODO this might not work at all. Might need a for loop
//     std::memcpy(m_buffer,ptr,nrow*ncol*nchan*sizeof(double));
//     for(size_t i =0 ;i<o_nrow;i++){
//         for(size_t j =0 ;j<o_ncol;j++){
//             for(size_t k = 0;k<o_nchan;k++){
//                 //(*this)(i,j,k) = ptr[i*ncol*nchan+j*nchan+k]/255;
//                 (*this)(i,j,k) /= 255;
//             }
//         }
//     }
//
// }


Image::Image(Image const & other, DataType dtype)
    : m_nrow(other.nrow),m_ncol(other.ncol),m_nchan(other.nchan),
    nrow(m_nrow),ncol(m_ncol),nchan(m_nchan)
{

    //get cpp_dtype
    getCppType(dtype)

    this->_data_type = other._data_type;
    reset_buffer(other.nrow, other.ncol,other.nchan);
    for (size_t i=0; i<nrow; ++i)
    {
        for (size_t j=0; j<ncol; ++j)
        {
            for (size_t k=0; j<nchan; ++k)
            {
                (*this)<double>(i,j,k) = other<double>(i,j,k);
            }
        }
    }
}

Image & Image::operator=(Image const & other)
{    
    std::cerr<<"FAILURE: operator not yet part of implementation!!"<<std::endl;
    if (this == &other) { return *this; }
    if (nrow != other.nrow || ncol != other.ncol)
    {
        reset_buffer(other.nrow, other.ncol,other.nchan);
    }
    for (size_t i=0; i<nrow; ++i)
    {
        for (size_t j=0; j<ncol; ++j)
        {
            for (size_t k=0; k<nchan; ++k)
            {
                (*this)(i,j,k) = other(i,j,k);
            }
        }
    }
    return *this;
}

Image::Image(Image && other)
    : nrow(m_nrow),ncol(m_ncol),nchan(m_nchan)

{
    reset_buffer(0, 0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_nchan, other.m_nchan);
    std::swap(m_buffer, other.m_buffer);
}

Image & Image::operator=(Image && other)
{
    if (this == &other) { return *this; }
    reset_buffer(0, 0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_nrow, other.m_ncol);
    std::swap(m_nchan,other.m_nchan);
    std::swap(m_buffer, other.m_buffer);
    return *this;
}

Image Image::multiply_naive(const Image & mat2) const
{
    //Will iimplement only if I find a use to inner product of tensors
    //if (this->m_ncol != mat2.nrow)
    //{
        //throw std::out_of_range(
            //"the number of first matrix column "
            //"differs from that of second matrix row");
    //}

    //Image ret(this->m_nrow, mat2.ncol);

    //for (size_t i=0; i<ret.nrow; ++i)
    //{
        //for (size_t k=0; k<ret.ncol; ++k)
        //{
            //double v = 0;
            //for (size_t j=0; j<this->m_ncol; ++j)
            //{
                //v += (*this)(i,j) * mat2(j,k);
            //}
            //ret(i,k) = v;
        //}
    //}

    //return ret;
}



Image::~Image()
{
    reset_buffer(0, 0, 0);
}


size_t Image::size() const { return m_ncol*m_nrow; }

//Also never call this
// double Image::buffer(size_t i) const { return m_buffer[i]; }

std::vector<double> Image::buffer_vector() const
{
    //I personally never calll this one
    return std::vector<double>(m_buffer, m_buffer+size());
}


void * Image::data(){
    return m_buffer.data();
}
void * Image::data() const {
    return m_buffer;
}
void Image::zero_out(){
    memset(m_buffer,0,m_nrow*m_ncol*sizeof(double));
}


void Image::print_vals(){

    if(m_nrow > 100 or m_ncol){
        for(size_t r=0;r< m_nrow;r++){
            for(size_t c=0;c< m_ncol;c++){
                //TODO maybe fix this if necessary
                py::print((*this)(r,c,0));
            }
            py::print("\n");
        }
    }else{
        py::print("Matrix too big to print");
    }
}

bool Image::operator==(Image const & other) const{
    //Compare through memory first maybe ? 
    //We have to time to check if its more effective this way
    if(ncol != other.ncol || nrow != other.nrow)
        return false;

    //Too strict of an approach
    // int n = memcmp(this->data(), other.data(),sizeof(double)*ncol*nrow);
    // return (n==0)? true: false;
    for(size_t row= 0;row<nrow;row++){
        for(size_t col= 0;col<ncol;col++){
            // if(std::abs((*this)(row,col) - other(row,col)) >= EQ_DIFF){
            //TODO , agian maybe fix htis if necessary
            if((*this)(row,col,0) != other(row,col,0)){
                return false;
            }
        }
    }

    return true;
}


/*********************
    Private Members
*********************/

size_t Image::index(size_t row, size_t col) const
{
    //Ew no, row-major better
    //return row + col * m_nrow;
    return row*this->m_ncol + col;
}

void Image::reset_buffer(size_t o_nrow, size_t o_ncol, size_t o_nchannels)
{
    //Handle Datatype
    using cpp_dtype = getCppType(_data_type);
    
    if (m_buffer!==nullptr) {
        m_buffer.swap(std::make_shared<cpp_dtype>());
    }
    const size_t nelement = o_nrow * o_ncol* o_nchannels;
    if (nelement) { 
        m_buffer = new double[nelement]; 

    }
    else          { m_buffer = nullptr; }
    m_nrow = o_nrow;
    m_ncol = o_ncol;
    m_nchan = o_nchannels;
}

Image Image::image_from_pybuffer(py::buffer &b){
    //Get info 
    py::buffer_info info = b.request();
    std::cout << "Creating from buffer object"<<std::endl;

    //Sanity Checks
    if(info.format != py::format_descriptor<double>::format()){
        std::string error_msg = "Expected Matrix of type double, got : "+info.format;
        throw std::runtime_error(error_msg);
    }
    if(info.ndim != 3){
        std::string err_msg = "Incompatible Dimensions. You must use 3, "+std::to_string(info.ndim)+" were given";
        throw std::runtime_error(err_msg);
    } 
    std::cout << "Building image with dimensions :"
        <<info.shape[0]<<","
        <<info.shape[1]<<","
        <<info.shape[2]<<","<<std::endl;

    return Image(info.shape[0],info.shape[1],info.shape[2],static_cast<const double*>(info.ptr));
}

Image Image::multiply_mkl(const Image & mat2) const{ 
    //TODO remove this
    
   
    // if (this->ncol != mat2.nrow)
    // {
    //     throw std::out_of_range(
    //         "the number of first matrix column "
    //         "differs from that of second matrix row");
    // }
    //Image ret(this->nrow, mat2.ncol);
    //
    // //TODO, not so sure about teh values of these alphas
    // cblas_dgemm(
    //         CblasRowMajor,
    //         CblasNoTrans,
    //         CblasNoTrans,
    //         this->nrow,
    //         mat2.ncol,
    //         this->ncol,
    //         1.0,
    //         this->m_buffer,
    //         this->m_ncol,
    //         mat2.data(),
    //         mat2.ncol,
    //         0.0,
    //         ret.data(),
    //         ret.ncol
    //         );
    //
    //return ret;
    //
}

Image Image::get_bw(){
    Image retimg(this->nrow,this->ncol,3);

    double val = 0;
    for(size_t i =0 ;i<nrow;i++){
        for(size_t j =0 ;j<ncol;j++){
            val = 0;
            for(size_t k = 0;k<nchan;k++){
                val += (*this)(i,j,k);
            }
            retimg(i,j,0) = (val/3);
            retimg(i,j,1) = (val/3);
            retimg(i,j,2) = (val/3);
        }
    }
    return retimg;
}
//TODO: make sure we tell numpy to do reference count on m_buffer
template<class T> 
py::array_t<T> Image::get_ndarray(){
    return py::array_t<T>(
            {m_nrow,m_ncol},// Shape
            {m_ncol*sizeof(T),sizeof(T)},// Strides
            m_buffer,// Data Pointer
            py::cast(m_buffer)
            // py::none() //Lets assume that we wont use the np version when this c++ instance doesnt exist. If not use above
            );
}
