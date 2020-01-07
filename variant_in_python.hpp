// Demonstrate how to pass various types of Python objects to a C++ variant of those types

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

#include <boost/python/object.hpp>
#include <boost/python/implicit.hpp>

#include <boost/python/converter/registry.hpp>
#include <boost/python/converter/rvalue_from_python_data.hpp>
#include <boost/python/converter/registrations.hpp>

#include <boost/mp11/list.hpp>
#include <boost/mp11/algorithm.hpp>

#include <variant>
#include <type_traits>

namespace {

//
// converting arguments
//

template<typename T>
PyTypeObject const *
numeric_expected_pytype()
{
    if constexpr (std::is_same_v<T, bool>)
        return &PyBool_Type;

    if constexpr (std::is_integral_v<T>)
        return &PyLong_Type;

    if constexpr (std::is_floating_point_v<T>)
        return &PyFloat_Type;
}

template <typename T, typename Variant>
void* numeric_convertible(PyObject* obj)
{
    if constexpr (std::is_same_v<T, bool>)
        return PyBool_Check(obj) ? obj : 0;

    if constexpr (std::is_integral_v<T>)
        // PyLong_Check will return true for Python bools!
        return (PyLong_Check(obj) && !PyBool_Check(obj)) ? obj : 0;

    if constexpr (std::is_floating_point_v<T>)
        return PyFloat_Check(obj) ? obj : 0;

    // TODO std::complex

}

template <typename T, typename Variant>
void strict_numeric_convertible()
{
    using namespace boost::python;

    // register a conservative converter to Variant from T
    // a small variation on the "implicit" converter
    converter::registry::push_back(
          &numeric_convertible<T, Variant>             // our own conversion tester
        , &converter::implicit<T, Variant>::construct  // the implicit constructor
        , type_id<Variant>()
        , &numeric_expected_pytype<T>                  // our code
        );

}

// utility metafunction
template <typename T>
struct remove_reference_wrapper
{
    using type = T;
};

template <typename T>
struct remove_reference_wrapper<typename std::reference_wrapper<T>>
{
    using type = T;
};

template <typename T>
using remove_reference_wrapper_t = typename remove_reference_wrapper<T>::type;

}

template <typename... T>
void register_variant_converter()
{
    using namespace boost;
    using var_t = std::variant<T...>;

    mp11::mp_for_each<
        mp11::mp_transform<
            std::add_pointer_t,      // T* can always be default constructed
            mp11::mp_list<T...>>>(
            [](auto t){
                using arg_t = std::decay_t<std::remove_pointer_t<decltype(t)>>;

                // variant<reference_wrapper<T>> can accept T directly
                using val_t = remove_reference_wrapper_t<arg_t>;

                // all the numeric types are super comfy converting to each other
                // and it produces unexpected behavior
                // use our own conservative conversion instead
                if constexpr (std::is_arithmetic_v<val_t>)
                {
                    strict_numeric_convertible<val_t, var_t>();
                }
                else
                {
                    // otherwise take advantage of the existing built-in conversions
                    python::implicitly_convertible<val_t, var_t>();
                }
            });
}

//
// converting return values
//

template <typename... T>
struct variant_to_pyobj
{
    static PyObject* convert(std::variant<T...> const & var)
    {
        // convert variant by recursively visiting stored types T...
        return std::visit(
            [](auto const & v){
                // convert this type
                return boost::python::to_python_value<decltype(v)>()(v);
            },
            var);
    }

    // we cannot have a "get_pytype" method because we may return any
    // of the variant types.
};

