#pragma once
#include "matrix.h"
#include "kernel.h"
#include <stdexcept>

namespace filters{
    //We can predefine some filters here 
    //and some utility functions

    inline double img_val(const Matrix & img,size_t x,size_t y){
        if(!(x>=0 && x<img.ncol)
            || !(y>=0 && y<img.nrow))
            return 0;
        else return img(x,y);
    }
    //Textbook Version
    inline double convolve(const Matrix & mat, const Kernel & k,  size_t x, size_t y){
        //We will only work with odd kernels for now 
        if(k.ncol %2 == 1 || k.nrow %2 == 1)
            throw std::out_of_range("Even Kernel Provided");
        //Continue with convollution

        return 0.0;
    }
    //Textbook Version
    inline double correlate(const Matrix & mat, const Kernel & k,  size_t x, size_t y){
        //We will only work with odd kernels for now 
        double c = 0.0;
        if(k.ncol != k.nrow || k.ncol %2 == 1 || k.nrow %2 == 1)
            throw std::out_of_range("Dimensions dont match or even Kernel Provided");
        //Continue with correlation
        int center = 0+(k.ncol/2);
        for(size_t i=0;i<k.nrow;i++){
            for(size_t j=0;j<k.ncol;j++){
                c+= k(i,j)*img_val(mat,x+(i-center),y+(j-center));
            }
        }
        return c;
    }
    Matrix filter2d_naive(const Matrix & img, const Kernel& k){
        //Lets try somethign really quick
        Matrix resulting_img(img.nrow,k.nrow);
        for(size_t x=0;x< img.nrow;x++){
            for(size_t y =0;y< img.ncol;y++){
                convolve(img,k,x,y);
            }
        }
        return resulting_img;
    }
}
