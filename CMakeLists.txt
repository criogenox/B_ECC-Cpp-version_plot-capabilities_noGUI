cmake_minimum_required(VERSION 3.16)
project(eqconv)

set(CMAKE_CXX_STANDARD 20)

include_directories(<path-to-python-headers>)  # <== must check!!!
link_directories(<path-to-libs>)               # <== must check!!!

add_executable(eqconv main.cpp
        util/src/input.cpp
        util/util/matplotlibcpp.h
        prepro/switchcase.cpp
        util/util/spline.hpp
        prepro/riemann_sum.cpp
        conicity/calculations.cpp
        postpro/one_matrix.cpp
        conicity/sym.cpp
        conicity/asym.cpp
        prepro/rzero.cpp
)
 
target_link_libraries(eqconv <python-libraries-directory-name>)  # <== must check!!!
