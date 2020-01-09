// Demonstrate using std::variant wrapping code

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

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/init.hpp>
#include <boost/python/module.hpp>

#include <string>
#include <variant>
#include <iostream>
#include <functional>

//
// create some wrapping examples involving variants
//

// demonstrate a variant argument
using var_arg_t = std::variant<double, int, bool>;
void doSomething(var_arg_t const & a)
{
    std::visit([](auto const & v) { std::cout << "got a " << typeid(v).name() << std::endl; }, a);
}

// a variant return type
using var_ret_t = std::variant<std::string, double>;
var_ret_t
produceSomething(bool useDouble)
{
    if (useDouble)
        return 12.1;
    else
        return "surprise!";
}

// wrapping a class (to show we can wrap variants of wrapped types)
struct Foo
{
    Foo() = default;
    Foo(int i, std::string s) : i_(i), s_(s) {}

    Foo(Foo const& other) : i_{other.i_}, s_{other.s_}
    {
        std::cout << "copy!\n";  // 3 times if reference_wrapper used; 5 otherwise (-O3)
    }

private:
    int i_;
    std::string s_;
};

// and using that class in a variant
using var_cc_arg_t = std::variant<double, Foo>;
using var_cc_ret_t = std::variant<Foo, std::string>;
var_cc_ret_t
foo(var_cc_arg_t const & var)
{
    return std::visit(
        [](auto const & v) -> var_cc_ret_t {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, double>)
            {
                return Foo{42, "goodbye"};
            } else {
                return "hello variant";
            }
        },
        var);
}

// using std::reference_wrapper
using rw_var_t = std::variant<int, std::reference_wrapper<Foo const>>;
void bar(rw_var_t var)
{
    std::visit([](auto const & v) { std::cout << "got a " << typeid(v).name() << std::endl; },
               var);
}

// using Nones
using maybe_empty_var_t = std::variant<std::monostate, int>;
maybe_empty_var_t baz(maybe_empty_var_t var)
{
    // just echo it back
    return var;
}

BOOST_PYTHON_MODULE(tc) {
    using namespace boost::python;

    // register convertibility of doSomething's variant types from Python
    register_variant_converter<var_arg_t>();

    def("doSomething", doSomething);

    // register conversion of produceSomething's return type back to Python
    variant_to_python_converter<var_ret_t>();

    def("produceSomething", produceSomething);

    // An example using a custom wrapped class
    class_<Foo>("Foo", init<int, std::string>());                 // the class
    register_variant_converter<var_cc_arg_t>();                   // argument variant
    variant_to_python_converter<var_cc_ret_t>();                  // result variant

    def("foo", foo);                                              // function using both

    // (lvalue) reference wrappers
    register_variant_converter<rw_var_t>();
    def("bar", bar);

    // accepting None
    register_variant_converter<maybe_empty_var_t>();
    variant_to_python_converter<maybe_empty_var_t>();
    def("baz", baz);

}
