#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include "data/image.h"

using namespace RIL;

/*********************
* Constructors
*********************/
void Image::print_vals(){

    if(_nrow > 100 or _ncol){
        for(size_t r=0;r< _nrow;r++){
            for(size_t c=0;c< _ncol;c++){
                //TODO maybe fix this if necessary
                py::print((*this)(r,c,0));
            }
            py::print("\n");
        }
    }else{
        py::print("Matrix too big to print");
    }
}

//bool Image::operator==(Image const & other) const{
//    //Compare through memory first maybe ? 
//    //We have to time to check if its more effective this way
//    if(_ncol != other._ncol || _nrow != other._nrow)
//        return false;
//
//    //Too strict of an approach
//    // int n = memcmp(this->data(), other.data(),sizeof(double)*ncol*nrow);
//    // return (n==0)? true: false;
//    for(size_t row= 0;row<_nrow;row++){
//        for(size_t col= 0;col<_ncol;col++){
//            // if(std::abs((*this)(row,col) - other(row,col)) >= EQ_DIFF){
//            for(size_t chan = 0;chan < _nchan;chan++){
//                if((*this)(row,col,chan) != other(row,col,chan)){
//                    return false;
//                }
//            }
//        }
//    }
//
//    return true;
//}


/*********************
    Private Members
*********************/


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

    //Super TODO: implement this
    //return Image(info.shape[0],info.shape[1],info.shape[2],static_cast<const double*>(info.ptr));
}


Image Image::get_bw(){
    Image retimg(this->_nrow,this->_ncol,3);

    double val = 0;
    for(size_t i =0 ;i<_nrow;i++){
        for(size_t j =0 ;j<_ncol;j++){
            val = 0;
            for(size_t k = 0;k<_nchan;k++){
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
