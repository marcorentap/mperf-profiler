find_package(Kokkos REQUIRED)

add_library(MPerfKP_RegionCPI SHARED RegionCPI.cpp)
target_include_directories(MPerfKP_RegionCPI PRIVATE ../../include)
target_link_libraries(MPerfKP_RegionCPI PRIVATE MPerfCore)
target_link_libraries(MPerfKP_RegionCPI PRIVATE Kokkos::kokkos)

install(TARGETS MPerfKP_RegionCPI)
