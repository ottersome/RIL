import sys
import numpy as np 
sys.path.append('./build')
import rilib

def test_numpy_to_rilib():
    m = np.asarray([[1,0,0],[0,1,0,],[0,0,1]],dtype=np.float64)
    rm = rilib.matrix_from_pybuffer(m)

    assert(m == rm)

def test_convolution():
    m = np.asarray([[1,0,0],[0,1,0,],[0,0,1]],dtype=np.float64)
    rm = rilib.matrix_from_pybuffer(m)

    assert(m == rm)



