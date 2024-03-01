add_library(mkp_region_cpi SHARED RegionCPI.cpp)
target_link_libraries(mkp_region_cpi PUBLIC Kokkos::kokkos)
target_link_libraries(mkp_region_cpi PUBLIC MPerf LinuxPerfTracer)
install(TARGETS mkp_region_cpi)
