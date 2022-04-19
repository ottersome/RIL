#pragma once
#include "matrix.h"
#include "kernel.h"

namespace filters{
    //We can predefine some filters here 
    //and some utility functions
    Matrix filter2d(const Matrix & m, const Kernel& k);
}
