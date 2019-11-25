cmake_minimum_required(VERSION 2.8)

include_directories("../include")
include_directories("../../include")
#message(${CMAKE_CXX_FLAGS})
add_library(muduo_curl STATIC Curl.cpp)
