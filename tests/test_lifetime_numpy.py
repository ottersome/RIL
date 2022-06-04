# TODO: Implement a test
#
import sys
import numpy as np
sys.path.append('./build')
import rilib

def test_reference():
    rb_img1 = rilib.Image(5,5,5)
    np_img1 = rb_img1.get_ndarray()
    # TODO implement more here

