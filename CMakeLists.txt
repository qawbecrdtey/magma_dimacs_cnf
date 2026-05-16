cmake_minimum_required(VERSION 4.0)
project(magma_dimacs_cnf)

find_package(Boost REQUIRED)

set(CADICAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cadical")

add_library(cadical STATIC IMPORTED)
set_target_properties(cadical PROPERTIES
        IMPORTED_LOCATION "${CADICAL_DIR}/build/libcadical.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CADICAL_DIR}/src")

add_executable(677_search ${CMAKE_CURRENT_SOURCE_DIR}/src/677_search.cpp)
set_target_properties(677_search PROPERTIES CXX_STANDARD 26)
target_include_directories(677_search PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CADICAL_DIR}/src)
target_link_libraries(677_search PRIVATE cadical ${Boost_LIBRARIES})
