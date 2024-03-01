find_package(Kokkos REQUIRED)

add_library(kp_mperf_cpi SHARED RegionCPI.cpp)
target_include_directories(kp_mperf_cpi PRIVATE ../../include)
target_link_libraries(kp_mperf_cpi PRIVATE MPerfCore)
target_link_libraries(kp_mperf_cpi PRIVATE Kokkos::kokkos)
