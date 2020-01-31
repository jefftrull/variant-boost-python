cmake_minimum_required( VERSION 3.13 )

# create a compilation database for clangd etc.
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_CXX_STANDARD 17 )

add_compile_options(-Wall -Wextra -Werror)

# dependencies
find_package( Boost 1.69 REQUIRED COMPONENTS python37 )
find_package( Python3 COMPONENTS Development )

#
# C++ code
#

# old approaches for contrast with std::variant
add_library( strategy1 SHARED strategy1.cpp )
target_link_libraries( strategy1 Boost::python37 Python3::Python )
set_target_properties( strategy1 PROPERTIES PREFIX "" )

# this one has a Python wrapper
add_library( s2 SHARED strategy2.cpp )
target_link_libraries( s2 Boost::python37 Python3::Python )
set_target_properties( s2 PROPERTIES PREFIX "" )

# the old approach examples redone using std::variant
add_library( using_variant1 SHARED using_variant1.cpp )
target_link_libraries( using_variant1 Boost::python37 Python3::Python )
set_target_properties( using_variant1 PROPERTIES PREFIX "" )

add_library( using_variant2 SHARED using_variant2.cpp )
target_link_libraries( using_variant2 Boost::python37 Python3::Python )
set_target_properties( using_variant2 PROPERTIES PREFIX "" )


# more complex example using std::variant described in blog
add_library( blog_example SHARED blog_example.cpp )
target_link_libraries( blog_example Boost::python37 Python3::Python )
set_target_properties( blog_example PROPERTIES PREFIX "" )

# examples to bind into Python and test from there
add_library( tc SHARED testcases.cpp )
target_link_libraries( tc Boost::python37 Python3::Python )
set_target_properties( tc PROPERTIES PREFIX "" )

enable_testing()

# a test to exercise the binding test cases
add_test(
  NAME basic_functionality
  COMMAND pytest-3
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
