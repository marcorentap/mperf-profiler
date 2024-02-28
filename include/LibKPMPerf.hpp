#ifndef LIBKPMPERF_HPP
#define LIBKPMPERF_HPP
#include <impl/Kokkos_Profiling_C_Interface.h>
#include <MPerf.hpp>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>
#include <memory>
#include <stack>
#include <string>

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t /*devInfoCount*/,
                                     void * /*deviceInfo*/);

extern "C" void kokkosp_finalize_library();

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID, uint64_t *kID);

extern "C" void kokkosp_end_parallel_for(const uint64_t kID);

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID);

extern "C" void kokkospk_end_parallel_scan(const uint64_t kID);

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID);

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID);

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID);

extern "C" void kokkosp_end_fence(const uint64_t kID);

extern "C" void kokkosp_push_profile_region(char *regionName);

extern "C" void kokkosp_pop_profile_region();

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size);

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size);

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size);

extern "C" void kokkosp_end_deep_copy();

#endif
