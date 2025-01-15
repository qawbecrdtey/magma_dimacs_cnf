cmake_minimum_required(VERSION 3.10)
project(magma_dimacs_cnf)

add_executable(677_search 677/search.cpp)
set_target_properties(677_search PROPERTIES CXX_STANDARD 14)