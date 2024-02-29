#include <Json.hpp>
#include <LibKPMPerf.hpp>
#include <MPerf.hpp>
#include <Measure.hpp>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::vector<std::tuple<std::string, double, double>> regionMeasures;
auto &outStream = std::cout;
MPerf::MPerf mperf;

using Measure = MPerf::Measure;
using MType = MPerf::MeasureType;
using MPulse = MPerf::MeasurePulse;
using HLType = MPerf::HLMeasureType;
using json = nlohmann::json;

// TODO: Use name stack for parfence, parfor, parscan, etc.

std::stringstream PrintMPulseMeasures(MPulse mPulse, json patch = nullptr,
                                      bool flush = true) {
  std::stringstream ss;
  auto mPulseMeasures = mperf.PulseMeasures(mPulse);
  for (auto &measure : mPulseMeasures) {
    auto j = measure->GetJSON();
    if (patch != nullptr) {
      j.merge_patch(patch);
    }
    ss << j << "\n";
  }

  return ss;
}

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
  json patch;
  patch["loadSeq"] = loadSeq;
  patch["interfaceVer"] = interfaceVer;
  patch["devInfoCount"] = devInfoCount;
  // TODO: Add deviceInfo
  mperf.PulseDoMeasure(MPulse::InitLibrary);
  mperf.PulseDoMeasure(MPulse::WholeLibrary);
  outStream << PrintMPulseMeasures(MPulse::InitLibrary, patch).str();
  outStream << PrintMPulseMeasures(MPulse::InitLibrary, patch).str();
}

extern "C" void kokkosp_finalize_library() {
  mperf.PulseDoMeasure(MPulse::FinalizeLibrary);
  outStream << PrintMPulseMeasures(MPulse::FinalizeLibrary).str();
}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {
  json patch;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;

  mperf.PulseDoMeasure(MPulse::BeginParFor);
  mperf.PulseDoMeasure(MPulse::WholeParFor);
  outStream << PrintMPulseMeasures(MPulse::BeginParFor, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParFor, patch).str();
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParFor);
  outStream << PrintMPulseMeasures(MPulse::EndParFor, patch).str();
}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {
  json patch;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;

  mperf.PulseDoMeasure(MPulse::BeginParScan);
  mperf.PulseDoMeasure(MPulse::WholeParScan);
  outStream << PrintMPulseMeasures(MPulse::BeginParScan, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParScan, patch).str();
}

extern "C" void kokkospk_end_parallel_scan(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParScan);
  mperf.PulseDoMeasure(MPulse::WholeParScan);
  outStream << PrintMPulseMeasures(MPulse::EndParScan, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParScan, patch).str();
}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {
  json patch;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;

  mperf.PulseDoMeasure(MPulse::BeginParReduce);
  mperf.PulseDoMeasure(MPulse::WholeParReduce);

  outStream << PrintMPulseMeasures(MPulse::BeginParReduce, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParReduce, patch).str();
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParReduce);
  mperf.PulseDoMeasure(MPulse::WholeParReduce);
  outStream << PrintMPulseMeasures(MPulse::EndParReduce, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParReduce, patch).str();
}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {
  json patch;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;

  mperf.PulseDoMeasure(MPulse::BeginParFence);
  mperf.PulseDoMeasure(MPulse::WholeParFence);
  outStream << PrintMPulseMeasures(MPulse::BeginParFence, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParFence, patch).str();
}

extern "C" void kokkosp_end_fence(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParFence);
  mperf.PulseDoMeasure(MPulse::WholeParFence);

  outStream << PrintMPulseMeasures(MPulse::EndParFence, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeParFence, patch).str();
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  json patch;
  auto regionNameStr = std::string(regionName);
  regionNameStack.push(regionName);
  patch["regionName"] = regionName;

  mperf.PulseDoMeasure(MPulse::PushProfileRegion);
  mperf.PulseDoMeasure(MPulse::WholeProfileRegion);
  outStream << PrintMPulseMeasures(MPulse::PushProfileRegion, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeProfileRegion, patch).str();
}

extern "C" void kokkosp_pop_profile_region() {
  json patch;
  auto regionNameStr = regionNameStack.top();
  regionNameStack.pop();
  patch["regionName"] = regionNameStr;

  mperf.PulseDoMeasure(MPulse::PopProfileRegion);
  mperf.PulseDoMeasure(MPulse::WholeProfileRegion);
  outStream << PrintMPulseMeasures(MPulse::PopProfileRegion, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeProfileRegion, patch).str();
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {
  json patch;
  // TODO: Make this work
  // patch["handle"] = handle;
  patch["name"] = name;
  // TODO: Use hex representation
  patch["ptr"] = (uint64_t) ptr;
  patch["size"] = size;

  mperf.PulseDoMeasure(MPulse::AllocateData);
  mperf.PulseDoMeasure(MPulse::WholeData);

  outStream << PrintMPulseMeasures(MPulse::AllocateData, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeData, patch).str();
}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {
  json patch;
  // TODO: Make this work
  // patch["handle"] = handle;
  patch["name"] = name;
  // TODO: Use hex representation
  patch["ptr"] = (uint64_t) ptr;
  patch["size"] = size;

  mperf.PulseDoMeasure(MPulse::DeallocateData);
  mperf.PulseDoMeasure(MPulse::WholeData);

  outStream << PrintMPulseMeasures(MPulse::DeallocateData, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeData, patch).str();
}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {
  json patch;
  // TODO: Make this work
  // patch["dst_handle"] = dst_handle;
  patch["dst_name"] = dst_name;
  patch["dst_ptr"] = (uint64_t) dst_ptr;
  // patch["src_handle"] = src_handle;
  patch["src_name"] = src_name;
  patch["src_ptr"] = (uint64_t) src_ptr;
  patch["size"] = (uint64_t) size;

  mperf.PulseDoMeasure(MPulse::BeginDeepCopy);
  mperf.PulseDoMeasure(MPulse::WholeDeepCopy);
  outStream << PrintMPulseMeasures(MPulse::BeginDeepCopy, patch).str();
  outStream << PrintMPulseMeasures(MPulse::WholeDeepCopy, patch).str();
}

extern "C" void kokkosp_end_deep_copy() {
  mperf.PulseDoMeasure(MPulse::EndDeepCopy);
  mperf.PulseDoMeasure(MPulse::WholeDeepCopy);

  outStream << PrintMPulseMeasures(MPulse::EndDeepCopy).str();
  outStream << PrintMPulseMeasures(MPulse::WholeDeepCopy).str();
}
