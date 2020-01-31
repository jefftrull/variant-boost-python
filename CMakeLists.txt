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
add_library( s1 SHARED strategy1.cpp )
target_link_libraries( s1 Boost::python37 Python3::Python )
set_target_properties( s1 PROPERTIES PREFIX "" )

add_library( strategy2 SHARED strategy2.cpp )
target_link_libraries( strategy2 Boost::python37 Python3::Python )
set_target_properties( strategy2 PROPERTIES PREFIX "" )

# example using std::variant described in blog
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
