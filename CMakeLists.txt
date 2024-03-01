cmake_minimum_required(VERSION 3.20)
project(kp_mperf LANGUAGES CXX)

find_package(Kokkos REQUIRED)

add_subdirectory(core)
add_subdirectory(tracers/linuxperf)
add_subdirectory(kokkos_tools)

# add_library(mperf SHARED ...)
# target_link_libraries(mperf SHARED core)

# add_library(kp_mperf SHARED src/MPerf.cpp src/LibKPMPerf.cpp src/LinuxPerf.cpp
# src/LinuxPerfProc.cpp src/HWLoc.cpp src/Dummy.cpp)
# target_include_directories(kp_mperf PRIVATE include/)
# target_link_libraries(kp_mperf PRIVATE Kokkos::kokkos)
# target_compile_features(kp_mperf PRIVATE cxx_std_17)

# add_executable(test_kp_mperf src/Measure.cpp src/MPerf.cpp src/LinuxPerf.cpp
#                src/LinuxPerfProc.cpp src/HWLoc.cpp src/Dummy.cpp src/Test.cpp)
# target_include_directories(test_kp_mperf PRIVATE include/)
# target_link_libraries(test_kp_mperf PRIVATE Kokkos::kokkos)
# target_compile_features(test_kp_mperf PRIVATE cxx_std_17)

# install(TARGETS kp_mperf)
