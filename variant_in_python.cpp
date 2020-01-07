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
void doSomething(std::variant<double, int, bool> const & a)
{
    std::visit([](auto const & v) { std::cout << "got a " << typeid(v).name() << "\n"; }, a);
}

// a variant return type
std::variant<std::string, double>
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
std::variant<Foo, std::string>
foo(std::variant<double, Foo> const & var)
{
    return std::visit(
        [](auto const & v) -> std::variant<Foo, std::string> {
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
void bar(std::variant<int, std::reference_wrapper<Foo const>> var)
{
    std::visit([](auto const & v) { std::cout << "got a " << typeid(v).name() << "\n"; },
               var);
}

BOOST_PYTHON_MODULE(vip) {
    using namespace boost::python;

    // register convertibility of doSomething's variant types from Python
    register_variant_converter<double, int, bool>();

    def("doSomething", doSomething);

    // register conversion of produceSomething's return type back to Python
    to_python_converter<std::variant<std::string, double>,
                        variant_to_pyobj<std::string, double>>();

    def("produceSomething", produceSomething);

    // An example using a custom wrapped class
    class_<Foo>("Foo", init<int, std::string>());                 // the class
    register_variant_converter<double, Foo>();                    // argument variant
    to_python_converter<std::variant<Foo, std::string>,
                        variant_to_pyobj<Foo, std::string>>();    // result variant

    def("foo", foo);                                              // function using both

    // (lvalue) reference wrappers
    register_variant_converter<int, std::reference_wrapper<Foo const>>();
    def("bar", bar);

}
