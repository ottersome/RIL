#pragma once
#include "data/image.h"
#include "data/kernel.h"
#include <stdexcept>
#include "algos/filters/gaussian.h"

namespace Filters{
    //We can predefine some filters here 
    //and some utility functions

    inline double img_val(const Image & img,size_t x,size_t y,size_t c){
        if(!(x>=0 && x<img.get_ncol())
            || !(y>=0 && y<img.get_nrow()))
            return 0;
        else return img(x,y,c);
    }
    //Textbook Version
    inline double convolve(const Image & mat, const Kernel & k,  size_t x, size_t y){
        //We will only work with odd kernels for now 
        if(k.get_ncol() %2 == 1 || k.get_nrow() %2 == 1)
            throw std::out_of_range("Even Kernel Provided");
        //Continue with convollution

        return 0.0;
    }
    //Textbook Version
    inline double correlate(const Image & mat, const Kernel & k,  size_t x, size_t y){
        //We will only work with odd kernels for now 
        double c = 0.0;
        if(k.get_ncol() != k.get_nrow() || k.get_ncol() %2 == 1 || k.get_nrow() %2 == 1)
            throw std::out_of_range("Dimensions dont match or even Kernel Provided");
        //Continue with correlation
        int center = 0+(k.get_ncol()/2);
        for(size_t i=0;i<k.get_nrow();i++){
            for(size_t j=0;j<k.get_ncol();j++){
                c+= k(i,j,0)*img_val(mat,x+(i-center),y+(j-center),0);
            }
        }
        return c;
    }
    Image filter2d_naive(const Image & img, const Kernel& k){
        //Lets try somethign really quick
        Image resulting_img(img.get_nrow(),k.get_nrow(),1);
        for(size_t x=0;x< img.get_nrow();x++){
            for(size_t y =0;y< img.get_ncol();y++){
                convolve(img,k,x,y);
            }
        }
        return resulting_img;
    }


}
