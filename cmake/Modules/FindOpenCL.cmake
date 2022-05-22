# For now we focus on mac/unix-like
# We also search for OpenCL in the NVIDIA SDK default location
set(CUSTOM_DIR  $ENV{HOME}/Projects/Libraries/OpenCL/include/)
set(CUSTOM_DIR_LB $ENV{HOME}/Projects/Libraries/OpenCL/lib/)
message("My HOME VARIABLE IS : ${CUSTOM_DIR}")
FIND_PATH(OPENCL_INCLUDE_DIR CL/opencl.hpp
    HINTS ${CUSTOM_DIR}
    )
message("OPENCL_INCLUDE_DIR: ${OPENCL_INCLUDE_DIR}")
FIND_LIBRARY(OPENCL_LIBRARIES OpenCL 
    ENV LD_LIBRARY_PATH
)

SET( OPENCL_FOUND "NO" )
IF(OPENCL_LIBRARIES )
    SET( OPENCL_FOUND "YES" )
ENDIF(OPENCL_LIBRARIES)
MARK_AS_ADVANCED(
  OPENCL_INCLUDE_DIR
)
