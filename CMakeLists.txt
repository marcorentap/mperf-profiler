cmake_minimum_required(VERSION 3.20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

project(MPerf LANGUAGES CXX)
find_package(Kokkos REQUIRED)

add_subdirectory(core)
target_include_directories(MPerfCore PRIVATE include)

set(KOKKOS_TOOLS)
set(TRACERS)

add_subdirectory(tracers/linuxperf)
list(APPEND TRACERS LinuxPerfTracer)

add_subdirectory(kokkos_tools/perf_proc)
list(APPEND KOKKOS_TOOLS mkt_perf_proc)


target_link_libraries(MPerfCore ${TRACERS})
foreach(TRACER IN LISTS TRACERS)
  target_include_directories(${TRACER} PRIVATE include)
endforeach()

foreach(TOOL IN LISTS KOKKOS_TOOLS)
  target_include_directories(${TOOL} PRIVATE include)
  target_link_libraries(${TOOL} MPerfCore ${TRACERS})
endforeach()

set_target_properties(${KOKKOS_TOOLS} PROPERTIES
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS ${KOKKOS_TOOLS})
