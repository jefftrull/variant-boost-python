// new strategy using variant: dispatch to overloads with std::visit
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

#include "variant_in_python.hpp"

#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <cmath>
#include <complex>

// overloads to be dispatched to

double mag(double v)
{
    return std::fabs(v);  // from <cmath>
}

double mag(std::complex<double> v)
{
    return std::abs(v);   // the overload in <complex>
}


// to be bound into Python
double magImpl(std::variant<double, std::complex<double>> var)
{
    // compare vs. the use of std::any_cast in strategy1.cpp
    return std::visit([](auto const & v) -> double
                      {
                          return mag(v);
                      },
                      var);
}

BOOST_PYTHON_MODULE(using_variant1) {
    using namespace boost::python;

    // register variant
    register_variant_converter<std::variant<double, std::complex<double>>>();

    // tell it about our function
    def("mag", magImpl);

}
