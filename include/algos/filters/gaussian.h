#pragma once

#include "data/kernel.h"
#include "devices/opencl.hpp"

namespace RIL{ namespace Filters{

/********************
 * Gaussian Filter
********************/
    Image & gaussian_filter(Image & img, const Kernel *& krn);

}}
