__kernel void vadd(__global const float *A, __global const float *B, __global float * c){
   int i  = get_global_id(0);
   c[i] = A[i] + B[i];

}
