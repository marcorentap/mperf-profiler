cmake_minimum_required(VERSION 3.20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

project(MPerf LANGUAGES CXX)
find_package(Kokkos REQUIRED)

add_subdirectory(core)
target_include_directories(mperfprofiler SYSTEM PRIVATE include)
set(MPERF_PUBLIC_HEADERS
  "include/MPerf/Core.hpp"
  "include/MPerf/Lib/Json.hpp"
  "include/MPerf/Tracers/LinuxPerf.hpp"
)
set_target_properties(mperfprofiler PROPERTIES PUBLIC_HEADER "${MPERF_PUBLIC_HEADERS}")

set(KOKKOS_TOOLS)
set(TRACERS)

# MPerf Tracers
add_subdirectory(tracers/linuxperf)
list(APPEND TRACERS LinuxPerfTracer)

# MPerf Kokkos Tools
add_subdirectory(kokkos_tools/perf_proc)
list(APPEND KOKKOS_TOOLS mkt_perf_proc)

add_subdirectory(kokkos_tools/region_cpi)
list(APPEND KOKKOS_TOOLS mkt_region_cpi)

add_subdirectory(kokkos_tools/resource_vector)
list(APPEND KOKKOS_TOOLS mkt_resource_vector)


target_link_libraries(mperfprofiler ${TRACERS})
foreach(TRACER IN LISTS TRACERS)
  target_include_directories(${TRACER} SYSTEM PRIVATE include)
endforeach()

foreach(TOOL IN LISTS KOKKOS_TOOLS)
  target_include_directories(${TOOL} SYSTEM PRIVATE include)
  target_link_libraries(${TOOL} mperfprofiler ${TRACERS})
endforeach()

set_target_properties(${KOKKOS_TOOLS} PROPERTIES
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS ${KOKKOS_TOOLS} mperfprofiler)
install(DIRECTORY include
  DESTINATION include
)
