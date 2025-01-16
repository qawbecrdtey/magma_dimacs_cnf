cmake_minimum_required(VERSION 3.10)
project(magma_dimacs_cnf)

add_executable(677_search 677/search.cpp)
set_target_properties(677_search PROPERTIES CXX_STANDARD 14)

add_executable(677_search_na255 677/search_na255.cpp)
set_target_properties(677_search_na255 PROPERTIES CXX_STANDARD 14)

add_executable(677_search_a2504 677/search_a2504.cpp)
set_target_properties(677_search_a2504 PROPERTIES CXX_STANDARD 14)