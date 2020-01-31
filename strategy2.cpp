// second strategy: bind one function and a single "python value" type, dispatch in C++
// Copyright (c) 2020 Jeff Trull

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/init.hpp>
#include <boost/python/module.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/exception_translator.hpp>

#include <cmath>
#include <complex>
#include <any>
#include <iostream>

double mag(std::any v)
{
    // dispatch in C++ from this single function

    auto * d = std::any_cast<double>(&v);
    if (d)
    {
        return (*d < 0.0) ? (-*d) : *d;
    } else {
        // Boost.Python screens the possible values for us
        // via the implicit conversion rules we define below
        // so v must be complex
        auto c = std::any_cast<std::complex<double>>(v);
        return std::sqrt((c.real() * c.real()) + (c.imag() * c.imag()));
    }

}

BOOST_PYTHON_MODULE(strategy2) {
    using namespace boost::python;

    // use std::any as our generic value container
    // specify the types it can be converted from
    implicitly_convertible<double, std::any>();
    implicitly_convertible<std::complex<double>, std::any>();

    // tell it about our function
    def("mag", mag);

}
