[![Build Status](https://travis-ci.org/jefftrull/variant-boost-python.svg?branch=master)](https://travis-ci.org/jefftrull/variant-boost-python)

# variant-boost-python
Demonstrates passing and returning std::variant using Boost.Python

## Motivation
In Python I enjoy the freedom of dynamic typing, but in C++ I like strong, compiler-checked types. Boost.Python provides a way to cleanly cross that boundary. But it doesn't have built-in support for every type or design pattern.

In particular it's often the case that Python values supplied to, or returned from, C++ classes and methods can only be one of a small number of types. One way to express this is to use overloads for each possible type. Another is to pass and return sum types like `std::variant`.

## Goals
1. For a parameter of C++ type `std::variant<A, B, C>` I want Python users to be able to pass any of the Python equivalents of `A`, `B`, or `C`
2. An exception should be thrown if the Python value is not one of the variant's types
3. The Python value `None` should only be accepted if `std::monostate` is one of the member types
4. Member types wrapped with `std::reference_wrapper` should be accepted without copying the wrapped type
5. A variant return value should be converted to whichever of the variant types was present

In this way the use of `variant` should be invisible to Python users (outside of type checking) and
appear like the dynamic typing they are used to.

