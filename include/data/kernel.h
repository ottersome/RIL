#pragma once
#include "image.h"

using namespace RIL;
class Kernel: public Image{

    Kernel(size_t nrow, size_t ncol)
        : Image(nrow,ncol,1) { }

    void decomposition();
    void padding();
    void convolution();
    void correlation();

};
