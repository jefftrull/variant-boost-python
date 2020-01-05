cmake_minimum_required( VERSION 3.13 )

# create a compilation database for clangd etc.
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set( CMAKE_CXX_STANDARD 17 )

# dependencies
find_package( Boost 1.69 REQUIRED COMPONENTS python37 )
find_package( Python3 COMPONENTS Development )

# C++ code
add_library( vip SHARED variant_in_python.cpp )
target_link_libraries( vip Boost::python37 Python3::Python )
set_target_properties( vip PROPERTIES PREFIX "" )
