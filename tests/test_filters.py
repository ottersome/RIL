import rilib as ril
from rilib import filters
# import opencv2 to compare perhaphs
import  opencv2 as cv2


def test_indexing():
    m1 = mtx.ident(3,3)
    assert m1[0,0] == 1
    assert m1[1,1] == 1
    assert m1[0,1] == 0
    assert m1[1,0] == 0
