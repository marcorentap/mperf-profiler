#include <LibKPMPerf.hpp>
#include <MPerf.hpp>
#include <Measure.hpp>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::vector<std::tuple<std::string, double, double>> regionMeasures;
MPerf::MPerf mperf;

using Measure = MPerf::Measure;
using MType = MPerf::MeasureType;
using MPulse = MPerf::MeasurePulse;
using HLType = MPerf::HLMeasureType;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t /*devInfoCount*/,
                                     void * /*deviceInfo*/) {
  mperf.PulseDoMeasure(MPulse::InitLibrary);
  mperf.PulseDoMeasure(MPulse::WholeLibrary);
}

extern "C" void kokkosp_finalize_library() {
  mperf.PulseDoMeasure(MPulse::FinalizeLibrary);
}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {
  mperf.PulseDoMeasure(MPulse::BeginParFor);
  mperf.PulseDoMeasure(MPulse::WholeParFor);
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  mperf.PulseDoMeasure(MPulse::EndParFor);
}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {
  mperf.PulseDoMeasure(MPulse::BeginParScan);
  mperf.PulseDoMeasure(MPulse::WholeParScan);
}

extern "C" void kokkospk_end_parallel_scan(const uint64_t kID) {
  mperf.PulseDoMeasure(MPulse::EndParScan);
}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {
  mperf.PulseDoMeasure(MPulse::BeginParReduce);
  mperf.PulseDoMeasure(MPulse::WholeParReduce);
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  mperf.PulseDoMeasure(MPulse::EndParReduce);
}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {
  mperf.PulseDoMeasure(MPulse::BeginParFence);
  mperf.PulseDoMeasure(MPulse::WholeParFence);
}

extern "C" void kokkosp_end_fence(const uint64_t kID) {
  mperf.PulseDoMeasure(MPulse::EndParFence);
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  mperf.PulseDoMeasure(MPulse::PushProfileRegion);
  mperf.PulseDoMeasure(MPulse::WholeProfileRegion);
  auto regionNameStr = std::string(regionName);
  regionNameStack.push(regionName);

  auto j = mperf.GetMeasure(HLType::LinuxPerfProc)->GetJSON();
  j["regionName"] = "Begin Profile Region: " + regionNameStr;
  std::cout << j << "\n";
}

extern "C" void kokkosp_pop_profile_region() {
  auto mCPI = mperf.GetMeasure(HLType::LinuxPerfProc);
  mperf.PulseDoMeasure(MPulse::PopProfileRegion);

  auto regionName = regionNameStack.top();
  auto j = mperf.GetMeasure(HLType::LinuxPerfProc)->GetJSON();
  j["regionName"] = "Begin Profile Region " + regionName;
  std::cout << j << "\n";
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {
  mperf.PulseDoMeasure(MPulse::AllocateData);
  mperf.PulseDoMeasure(MPulse::WholeData);
}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {
  mperf.PulseDoMeasure(MPulse::DeallocateData);
}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {
  mperf.PulseDoMeasure(MPulse::BeginDeepCopy);
  mperf.PulseDoMeasure(MPulse::WholeDeepCopy);
}

extern "C" void kokkosp_end_deep_copy() {
  mperf.PulseDoMeasure(MPulse::EndDeepCopy);
}
