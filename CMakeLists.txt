cmake_minimum_required(VERSION 3.20)
project(kp_cpi LANGUAGES CXX)

find_package(Kokkos REQUIRED)
add_library(kp_cpi SHARED cpi_profiler.cpp)
target_link_libraries(kp_cpi PRIVATE Kokkos::kokkos)
target_compile_features(kp_cpi PRIVATE cxx_std_17)

install(TARGETS kp_cpi)
