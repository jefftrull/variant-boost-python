# test Python variant wrappers

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
from tc import *

from pytest import approx,raises

# variant parameter
def test_passbool(capfd):
    doSomething(True)
    captured = capfd.readouterr()
    assert captured.out == "got a b\n"

def test_passint(capfd):
    doSomething(42)
    captured = capfd.readouterr()
    assert captured.out == "got a i\n"

def test_passdouble(capfd):
    doSomething(12.1)
    captured = capfd.readouterr()
    assert captured.out == "got a d\n"

# variant return type
def test_retdouble():
    assert produceSomething(True) == approx(12.1)

def test_retstring():
    assert produceSomething(False) ==  'surprise!'

# wrapped custom class inside variant
def test_cc_double():
    assert type(foo(3.14)) is Foo

def test_cc_wrapped():
    assert type(foo(Foo(42, "hello"))) is str

# reference wrappers
def test_rw_cc(capfd):
    bar(Foo(42, "hello"))
    captured = capfd.readouterr()
    assert captured.out.endswith("got a St17reference_wrapperIK3FooE\n")  # std::reference_wrapper<Foo const>

def test_rw_int(capfd):
    bar(7)
    captured = capfd.readouterr()
    assert captured.out == "got a i\n"

# treatment of std::monostate and None
def test_ms_int():
    assert type(baz(12)) is int

def test_ms_none():
    assert baz(None) is None

def test_ms_not_allowed():
    with raises(TypeError):
        bar(None)    # baz allows monostate but bar does NOT

