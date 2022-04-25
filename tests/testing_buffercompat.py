import sys
import numpy as np 
import PIL.Image
sys.path.append('./build')
sys.path.append('../build')
import rilib
import matplotlib.image as mpimg
import matplotlib.pyplot as plt


def test_numpy_to_rilib():
    m = np.asarray([[1,0,0],[0,1,0,],[0,0,1]],dtype=np.float64)
    rm = rilib.matrix_from_pybuffer(m)

    assert(m == rm)

def test_convolution():
    pic = PIL.Image.open("./TestImages/valve.PNG")
    w,h = pic.size
    channels = len(pic.getbands())
    pix = np.asarray(pic.getdata(),dtype=np.float64).reshape((h,w,channels))
    # pix /= 255;
    rm = rilib.image_from_pybuffer(pix)
    bw = rm.get_bw()
    bw = np.asarray(bw)
    rm = np.asarray(rm)
    print("rm shape is : {}".format(rm.shape))
    plt.imshow(bw)
    plt.show()
    # rk = rilib.kernel()
    # k_sobel = np.asarray([[1,0,-1],[2,0,-2],[1,0,-1]])
    # rilib.convolution()


