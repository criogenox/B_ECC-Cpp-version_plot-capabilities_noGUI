cmake_minimum_required(VERSION 3.28)
set(PROJECT_NAME "eqcon")
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
set(default_build_type "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(TARGET ${PROJECT_NAME})
set(TARGET_SRC ${PROJECT_NAME}_src)
set(TARGET_TEST "${PROJECT_NAME}Test")
#set(TARGET_LIB "${PROJECT_NAME}_lib")
#-----------#-----------#-----------#

#include(FetchContent)                      # (3) Gtest with linked library
#FetchContent_Declare(googletest
#        GIT_REPOSITORY https://github.com/google/googletest
#        GIT_TAG v1.14.0)
#FetchContent_MakeAvailable(googletest)
#-----------#-----------#-----------#

#set(LIBS_PATH "${PROJECT_SOURCE_DIR}/libs")
set(PYLIBS_PATH "/home/<user_name>/.local/lib")
set(HEADERS_PATH "${PROJECT_SOURCE_DIR}/headers")
set(PYHS_PATH "/usr/include/python3.10/")
set(SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(TEST_PATH "${PROJECT_SOURCE_DIR}/test") # (3) Gtest with linked library
set(DATA_PATH "${PROJECT_SOURCE_DIR}/data")
#-----------#-----------#-----------#

file(COPY ${DATA_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE CPPS "${SRC_PATH}/*.cpp")
file(GLOB_RECURSE CPPT "${TEST_PATH}/*.cpp")
file(GLOB_RECURSE CPPTR ${CPPT} ${CPP})     # (3) Gtest with linked library
#-----------#-----------#-----------#

include_directories(${HEADERS_PATH} ${PYHS_PATH})
link_directories(${PYLIBS_PATH})
#-----------#-----------#-----------#

add_executable(${TARGET_SRC} main/main.cpp ${CPPS})      # (0) compile with complete source code
target_link_libraries(${TARGET_SRC} python3.10)
#-----------#-----------#-----------#

#add_executable(${TARGET_TEST} ${CPPTR} ${CPPS})    # (3) Gtest with linked library
#target_link_libraries(${TARGET_TEST} python3.10 gtest_main gmock_main)
#-----------#-----------#-----------#
