cmake_minimum_required(VERSION 3.20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)

project(MPerf LANGUAGES CXX)
find_package(Kokkos REQUIRED)

add_subdirectory(core)
target_include_directories(MPerf SYSTEM
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set(TRACERS)

# MPerf Tracers
add_subdirectory(tracers/linuxperf)
add_subdirectory(tracers/cppchrono)
list(APPEND TRACERS 
  LinuxPerfTracer
  CPPChronoTracer
)

target_link_libraries(MPerf ${TRACERS})
foreach(TRACER IN LISTS TRACERS)
  target_include_directories(${TRACER} SYSTEM 
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
endforeach()

# Install lib and headers
install(
  TARGETS MPerf ${TRACERS}
  EXPORT MPerfConfig
)
install(
  DIRECTORY include/
  DESTINATION include
)

# Export
# export(
#   TARGETS MPerf ${TRACERS}
#   NAMESPACE MPerf::
#   FILE ${CMAKE_CURRENT_BINARY_DIR}/MPerfConfig.cmake
# )
install(
  EXPORT MPerfConfig
  FILE MPerfConfig.cmake
  NAMESPACE MPerf::
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/MPerf
)
