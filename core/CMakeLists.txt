add_library(MPerf SHARED MPerf.cpp)
target_link_libraries(MPerf LinuxPerfTracer)
target_include_directories(MPerf PRIVATE ../include)

install(TARGETS MPerf)
