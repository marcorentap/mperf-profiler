cmake_minimum_required(VERSION 3.20)
project(kp_cpi)
add_library(kp_cpi SHARED cpi_profiler.c)
add_executable(main main.c)
