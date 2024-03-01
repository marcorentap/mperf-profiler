add_library(LinuxPerfTracer SHARED Tracer.cpp ProcMeasure.cpp)
target_include_directories(LinuxPerfTracer PRIVATE ../../include)
