add_library(LibPFM4Tracer SHARED
  LibPFM4.cpp
)
target_link_libraries(LibPFM4Tracer -lpfm)
