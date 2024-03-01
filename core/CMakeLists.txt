add_library(MPerfCore SHARED MPerf.cpp)
target_link_libraries(MPerfCore LinuxPerfTracer)
target_include_directories(MPerfCore PRIVATE ../include)
