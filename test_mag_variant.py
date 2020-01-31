# test cases for std::variant strategies for "mag" example

# Copyright (c) 2020 Jeff Trull

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# grab stuff from the generated testcase module
import sys, os
sys.path.append(os.path.realpath(os.path.dirname(__file__)+"/build"))
import using_variant1, using_variant2

from pytest import approx, raises

def test_s1_dbl():
    assert using_variant1.mag(-11.5) == approx(11.5)
    assert using_variant1.mag(3.14) == approx(3.14)

def test_s1_cplx():
    assert using_variant1.mag(3+4j) == approx(5.0)
    assert using_variant1.mag(3-4j) == approx(5.0)

def test_s1_cvtfail():
    with raises(Exception):
        using_variant1.mag("this is not an OK type")
