#pragma once
#include "matrix.h"

class Kernel: public Matrix{

    Kernel(size_t nrow, size_t ncol)
        : Matrix(nrow,ncol) { }

    void decomposition();
    void padding();
    void convolution();
    void correlation();

};
