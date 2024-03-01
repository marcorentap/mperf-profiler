cmake_minimum_required(VERSION 3.20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
project(MPerf LANGUAGES CXX)

find_package(Kokkos REQUIRED)
add_subdirectory(core)

add_subdirectory(tracers/linuxperf)
target_include_directories(LinuxPerfTracer PRIVATE include)

add_subdirectory(kokkos_tools/region_cpi)
target_include_directories(mkp_region_cpi PRIVATE include)

# include(GNUInstallDirs)
# install(DIRECTORY include/MPerf
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )
