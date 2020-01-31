// std::variant wrapping example from my blog jefftrull.github.io

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

#include <boost/python/to_python_converter.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <string>
#include <variant>
#include <type_traits>

using var_t = std::variant<std::string, int>;

var_t adder(var_t const& var_a, var_t const& var_b)
{
    return std::visit(
        [](auto const & a, auto const & b) -> var_t
        {
            // return integer sum if both are ints, otherwise a string append
            if constexpr (std::is_same_v<decltype(a), decltype(b)>)
            {
                return a + b;          // works for both types
            } else {
                if constexpr (std::is_same_v<std::decay_t<decltype(a)>, std::string>)
                    return a + std::to_string(b);
                else
                    return std::to_string(a) + b;
            }
        },
        var_a,
        var_b);
}

BOOST_PYTHON_MODULE(blog_example) {
    using namespace boost::python;

    // register variant as argument (inbound a.k.a "from" conversion)
    register_variant_converter<var_t>();

    // register it as return type ("to" conversion)
    to_python_converter<var_t, variant_to_pyobj<var_t>>();

    def("adder", adder);
}
