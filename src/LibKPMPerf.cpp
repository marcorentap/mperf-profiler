#include <LibKPMPerf.hpp>
#include <MPerf.hpp>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::vector<std::tuple<std::string, double, double>> regionMeasures;
MPerf::MPerf mperf;

using Measure = MPerf::Measure;
using MType = MPerf::MeasureType;
using MPulse = MPerf::MeasurePulse;
using SType = MPerf::SubsystemType;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t /*devInfoCount*/,
                                     void * /*deviceInfo*/) {
  mperf.PulseReadValues(MPulse::InitLibrary);
}

extern "C" void kokkosp_finalize_library() {
  mperf.PulseReadValues(MPulse::FinalizeLibrary);
}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {
  mperf.PulseReadValues(MPulse::BeginParFor);
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  mperf.PulseReadValues(MPulse::EndParFor);
}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {
  mperf.PulseReadValues(MPulse::BeginParScan);
  mperf.PulseReadValues(MPulse::WholeParScan);
}

extern "C" void kokkospk_end_parallel_scan(const uint64_t kID) {
  mperf.PulseReadValues(MPulse::EndParScan);
  mperf.PulseReadValues(MPulse::WholeParScan);
}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {
  mperf.PulseReadValues(MPulse::BeginParReduce);
  mperf.PulseReadValues(MPulse::WholeParReduce);
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  mperf.PulseReadValues(MPulse::EndParReduce);
  mperf.PulseReadValues(MPulse::WholeParReduce);
}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {
  mperf.PulseReadValues(MPulse::BeginParFence);
  mperf.PulseReadValues(MPulse::WholeParFence);
}

extern "C" void kokkosp_end_fence(const uint64_t kID) {
  mperf.PulseReadValues(MPulse::EndParFence);
  mperf.PulseReadValues(MPulse::WholeParFence);
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  mperf.PulseReadValues(MPulse::PushProfileRegion);
  mperf.PulseReadValues(MPulse::WholeProfileRegion);
}

extern "C" void kokkosp_pop_profile_region() {
  mperf.PulseReadValues(MPulse::PopProfileRegion);
  mperf.PulseReadValues(MPulse::WholeProfileRegion);
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {
  mperf.PulseReadValues(MPulse::AllocateData);
  mperf.PulseReadValues(MPulse::WholeData);
}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {
  mperf.PulseReadValues(MPulse::DeallocateData);
  mperf.PulseReadValues(MPulse::WholeData);
}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {
  mperf.PulseReadValues(MPulse::BeginDeepCopy);
  mperf.PulseReadValues(MPulse::WholeDeepCopy);
}

extern "C" void kokkosp_end_deep_copy() {
  mperf.PulseReadValues(MPulse::EndDeepCopy);
  mperf.PulseReadValues(MPulse::WholeDeepCopy);
}
