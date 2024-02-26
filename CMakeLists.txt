cmake_minimum_required(VERSION 3.20)
project(kp_mperf LANGUAGES CXX)

find_package(Kokkos REQUIRED)
add_library(kp_mperf SHARED src/MPerf.cpp src/LibKPMPerf.cpp)
target_include_directories(kp_mperf PRIVATE include/)
target_link_libraries(kp_mperf PRIVATE Kokkos::kokkos)
target_compile_features(kp_mperf PRIVATE cxx_std_17)

install(TARGETS kp_mperf)
