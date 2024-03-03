#include <fstream>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>
#include <iostream>
#include <sstream>
#include <stack>

#include "MPerf/Core.hpp"
#include "MPerf/KokkosTools.hpp"
#include "MPerf/Tracers/LinuxPerf.hpp"

namespace MKP = MPerf::KokkosTools;
using HLType = MKP::HLType;
using KPulse = MKP::KPulse;

using json = nlohmann::json;
using LinuxTracer = MPerf::Tracers::LinuxPerf::Tracer;

constexpr auto outputFileName = "mperf.json";

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::ofstream outputFile;
json rootJson;

void AddPulseMeasuresToJson(KPulse mPulse, json patch) {
  std::stringstream ss;
  auto mPulseMeasures = MPerf::KokkosTools::measuresByPulse[mPulse];
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
  outputFile.open(outputFileName, std::ofstream::trunc);

  AddMeasure(HLType::ProcCounters, linuxTracer, KPulse::All);

  MKP::DoInitLibrary();

  patch["hook"] = __FUNCTION__;
  patch["loadSeq"] = loadSeq;
  patch["interfaceVer"] = interfaceVer;
  patch["devInfoCount"] = devInfoCount;
  AddPulseMeasuresToJson(KPulse::InitLibrary, patch);
}

extern "C" void kokkosp_finalize_library() {
  json patch;

  MKP::DoFinalizeLibrary();

  patch["hook"] = __FUNCTION__;
  AddPulseMeasuresToJson(KPulse::FinalizeLibrary, patch);
  outputFile << rootJson << std::endl;
}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {
  json patch;

  MKP::DoBeginParallelFor();

  patch["hook"] = __FUNCTION__;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;
  AddPulseMeasuresToJson(KPulse::BeginParFor, patch);
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  json patch;

  MKP::DoEndParallelFor();

  patch["hook"] = __FUNCTION__;
  patch["kID"] = kID;
  AddPulseMeasuresToJson(KPulse::EndParFor, patch);
}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {
  json patch;

  MKP::DoBeginParallelScan();

  patch["hook"] = __FUNCTION__;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;
  AddPulseMeasuresToJson(KPulse::BeginParScan, patch);
}

extern "C" void kokkosp_end_parallel_scan(const uint64_t kID) {
  json patch;

  MKP::DoEndParallelScan();

  patch["hook"] = __FUNCTION__;
  patch["kID"] = kID;
  AddPulseMeasuresToJson(KPulse::EndParScan, patch);
}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {
  json patch;

  MKP::DoBeginParallelReduce();

  patch["hook"] = __FUNCTION__;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;
  AddPulseMeasuresToJson(KPulse::BeginParReduce, patch);
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  json patch;

  MKP::DoEndParallelReduce();

  patch["hook"] = __FUNCTION__;
  patch["kID"] = kID;
  AddPulseMeasuresToJson(KPulse::EndParReduce, patch);
}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {
  json patch;

  MKP::DoBeginFence();

  patch["hook"] = __FUNCTION__;
  patch["name"] = name;
  patch["devID"] = devID;
  patch["kID"] = *kID;
  AddPulseMeasuresToJson(KPulse::BeginFence, patch);
}

extern "C" void kokkosp_end_fence(const uint64_t kID) {
  json patch;

  MKP::DoEndFence();

  patch["hook"] = __FUNCTION__;
  patch["kID"] = kID;
  AddPulseMeasuresToJson(KPulse::EndFence, patch);
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  json patch;

  MKP::DoPushProfileRegion();

  regionNameStack.push(regionName);
  patch["hook"] = __FUNCTION__;
  patch["regionName"] = regionName;

  AddPulseMeasuresToJson(KPulse::PushProfileRegion, patch);
}

extern "C" void kokkosp_pop_profile_region() {
  json patch;
  auto regionNameStr = regionNameStack.top();

  MKP::DoPopProfileRegion();

  patch["hook"] = __FUNCTION__;
  patch["regionName"] = regionNameStack.top();
  AddPulseMeasuresToJson(KPulse::PopProfileRegion, patch);
  regionNameStack.pop();
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {
  json patch;
  std::stringstream handleStream;

  MKP::DoAllocateData();

  handleStream << "0x" << std::hex << ptr;
  patch["hook"] = __FUNCTION__;
  patch["handle"] = handle.name;
  patch["name"] = name;
  patch["ptr"] = handleStream.str();
  patch["size"] = size;

  AddPulseMeasuresToJson(KPulse::AllocateData, patch);
}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {
  json patch;
  std::stringstream handleStream;

  MKP::DoDeallocateData();

  handleStream << "0x" << std::hex << ptr;
  patch["hook"] = __FUNCTION__;
  patch["handle"] = handle.name;
  patch["name"] = name;
  patch["ptr"] = handleStream.str();
  patch["size"] = size;

  AddPulseMeasuresToJson(KPulse::DeallocateData, patch);
}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {
  json patch;
  std::stringstream srcHandleStream;
  std::stringstream dstHandleStream;

  MKP::DoBeginDeepCopy();

  dstHandleStream << "0x" << std::hex << dst_ptr;
  srcHandleStream << "0x" << std::hex << src_ptr;

  patch["hook"] = __FUNCTION__;
  patch["dst_handle"] = dst_handle.name;
  patch["dst_name"] = dst_name;
  patch["dst_ptr"] = dstHandleStream.str();
  patch["src_handle"] = src_handle.name;
  patch["src_name"] = src_name;
  patch["src_ptr"] = srcHandleStream.str();
  patch["size"] = size;

  AddPulseMeasuresToJson(KPulse::BeginDeepCopy, patch);
}

extern "C" void kokkosp_end_deep_copy() {
  json patch;
  MKP::DoEndDeepCopy();

  patch["hook"] = __FUNCTION__;
  AddPulseMeasuresToJson(KPulse::EndDeepCopy, patch);
}
