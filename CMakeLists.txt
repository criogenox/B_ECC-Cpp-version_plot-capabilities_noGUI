cmake_minimum_required(VERSION 3.16)
project(eqconv)

set(CMAKE_CXX_STANDARD 20)

include_directories(<path-to-python-headers>)  # <== must check!!!
link_directories(<path-to-libs>)               # <== must check!!!

add_executable(eqconv main.cpp
        input.cpp
        matplotlibcpp.h
        switchcase.cpp
        spline.hpp
        riemann_sum.cpp
        calculations.cpp
        one_matrix.cpp
        sym.cpp
        asym.cpp
        rzero.cpp
)
 
target_link_libraries(eqconv <python-libraries-directory-name>)  # <== must check!!!
