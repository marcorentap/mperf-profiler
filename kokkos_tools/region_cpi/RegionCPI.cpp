#include <fstream>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>
#include <iostream>
#include <stack>

#include "MPerf/Core.hpp"
#include "MPerf/Lib/Json.hpp"
#include "MPerf/Tracers/LinuxPerf.hpp"

using HLType = MPerf::HLMeasureType;
using MPulse = MPerf::MeasurePulse;
using json = nlohmann::json;
using LinuxTracer = MPerf::Tracers::LinuxPerf::Tracer;

constexpr auto outputFileName = "mperf.json";

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::vector<std::tuple<std::string, double, double>> regionMeasures;
MPerf::MPerf mperf;
std::ofstream outputFile;
json rootJson;

// TODO: Use name stack for parfence, parfor, parscan, etc.

void PrintMPulseMeasures(MPulse mPulse, json patch = nullptr,
                         bool flush = true) {
  std::stringstream ss;
  auto mPulseMeasures = mperf.PulseMeasures(mPulse);
  for (auto &measure : mPulseMeasures) {
    auto j = measure->GetJSON();
    if (patch != nullptr) {
      j.merge_patch(patch);
    }
    rootJson.push_back(j);
  }
}

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
  json patch;
  LinuxTracer linuxTracer;
  mperf.AddMeasure(linuxTracer, HLType::LinuxPerfProc,
                   MPulse::WholeProfileRegion);
  // TODO: Add deviceInfo
  patch["loadSeq"] = loadSeq;
  patch["interfaceVer"] = interfaceVer;
  patch["devInfoCount"] = devInfoCount;

  outputFile.open(outputFileName, std::ofstream::trunc);

  mperf.PulseDoMeasure(MPulse::InitLibrary);
  mperf.PulseDoMeasure(MPulse::WholeLibrary);
  PrintMPulseMeasures(MPulse::InitLibrary, patch);
  PrintMPulseMeasures(MPulse::InitLibrary, patch);
}

extern "C" void kokkosp_finalize_library() {
  mperf.PulseDoMeasure(MPulse::FinalizeLibrary);
  PrintMPulseMeasures(MPulse::FinalizeLibrary);

  outputFile << rootJson << std::endl;
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
  PrintMPulseMeasures(MPulse::BeginParFor, patch);
  PrintMPulseMeasures(MPulse::WholeParFor, patch);
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParFor);
  PrintMPulseMeasures(MPulse::EndParFor, patch);
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
  PrintMPulseMeasures(MPulse::BeginParScan, patch);
  PrintMPulseMeasures(MPulse::WholeParScan, patch);
}

extern "C" void kokkospk_end_parallel_scan(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParScan);
  mperf.PulseDoMeasure(MPulse::WholeParScan);
  PrintMPulseMeasures(MPulse::EndParScan, patch);
  PrintMPulseMeasures(MPulse::WholeParScan, patch);
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

  PrintMPulseMeasures(MPulse::BeginParReduce, patch);
  PrintMPulseMeasures(MPulse::WholeParReduce, patch);
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParReduce);
  mperf.PulseDoMeasure(MPulse::WholeParReduce);
  PrintMPulseMeasures(MPulse::EndParReduce, patch);
  PrintMPulseMeasures(MPulse::WholeParReduce, patch);
}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {
  json patch;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;

  mperf.PulseDoMeasure(MPulse::BeginParFence);
  mperf.PulseDoMeasure(MPulse::WholeParFence);
  PrintMPulseMeasures(MPulse::BeginParFence, patch);
  PrintMPulseMeasures(MPulse::WholeParFence, patch);
}

extern "C" void kokkosp_end_fence(const uint64_t kID) {
  json patch;
  patch["kID"] = kID;

  mperf.PulseDoMeasure(MPulse::EndParFence);
  mperf.PulseDoMeasure(MPulse::WholeParFence);

  PrintMPulseMeasures(MPulse::EndParFence, patch);
  PrintMPulseMeasures(MPulse::WholeParFence, patch);
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  json patch;
  auto regionNameStr = std::string(regionName);
  regionNameStack.push(regionName);
  patch["regionName"] = regionName;

  mperf.PulseDoMeasure(MPulse::PushProfileRegion);
  mperf.PulseDoMeasure(MPulse::WholeProfileRegion);
  PrintMPulseMeasures(MPulse::PushProfileRegion, patch);
  PrintMPulseMeasures(MPulse::WholeProfileRegion, patch);
}

extern "C" void kokkosp_pop_profile_region() {
  json patch;
  auto regionNameStr = regionNameStack.top();
  regionNameStack.pop();
  patch["regionName"] = regionNameStr;

  mperf.PulseDoMeasure(MPulse::PopProfileRegion);
  mperf.PulseDoMeasure(MPulse::WholeProfileRegion);
  PrintMPulseMeasures(MPulse::PopProfileRegion, patch);
  PrintMPulseMeasures(MPulse::WholeProfileRegion, patch);
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {
  json patch;
  // TODO: Make this work
  // patch["handle"] = handle;
  patch["name"] = name;
  // TODO: Use hex representation
  patch["ptr"] = (uint64_t)ptr;
  patch["size"] = size;

  mperf.PulseDoMeasure(MPulse::AllocateData);
  mperf.PulseDoMeasure(MPulse::WholeData);

  PrintMPulseMeasures(MPulse::AllocateData, patch);
  PrintMPulseMeasures(MPulse::WholeData, patch);
}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {
  json patch;
  // TODO: Make this work
  // patch["handle"] = handle;
  patch["name"] = name;
  // TODO: Use hex representation
  patch["ptr"] = (uint64_t)ptr;
  patch["size"] = size;

  mperf.PulseDoMeasure(MPulse::DeallocateData);
  mperf.PulseDoMeasure(MPulse::WholeData);

  PrintMPulseMeasures(MPulse::DeallocateData, patch);
  PrintMPulseMeasures(MPulse::WholeData, patch);
}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {
  json patch;
  // TODO: Make this work
  // patch["dst_handle"] = dst_handle;
  patch["dst_name"] = dst_name;
  patch["dst_ptr"] = (uint64_t)dst_ptr;
  // patch["src_handle"] = src_handle;
  patch["src_name"] = src_name;
  patch["src_ptr"] = (uint64_t)src_ptr;
  patch["size"] = (uint64_t)size;

  mperf.PulseDoMeasure(MPulse::BeginDeepCopy);
  mperf.PulseDoMeasure(MPulse::WholeDeepCopy);
  PrintMPulseMeasures(MPulse::BeginDeepCopy, patch);
  PrintMPulseMeasures(MPulse::WholeDeepCopy, patch);
}

extern "C" void kokkosp_end_deep_copy() {
  mperf.PulseDoMeasure(MPulse::EndDeepCopy);
  mperf.PulseDoMeasure(MPulse::WholeDeepCopy);

  PrintMPulseMeasures(MPulse::EndDeepCopy);
  PrintMPulseMeasures(MPulse::WholeDeepCopy);
}
