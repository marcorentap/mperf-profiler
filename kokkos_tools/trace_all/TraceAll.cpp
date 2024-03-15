#include <MPerf/Core.hpp>
#include <MPerf/KokkosTools.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>
#include <fstream>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>
#include <iostream>
#include <sstream>
#include <stack>

namespace MKP = MPerf::KokkosTools;
using HLType = MKP::HLType;
using KPulse = MKP::KPulse;

using json = nlohmann::json;
using LinuxPerfTracer = MPerf::Tracers::LinuxPerf::Tracer;
auto &measuresByPulse = MPerf::KokkosTools::measuresByPulse;

constexpr auto outputFileName = "mperf_trace_all.json";

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::ofstream outputFile;
json rootJson;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
  json j;
  LinuxPerfTracer linuxPerfTracer;
  outputFile.open(outputFileName, std::ofstream::trunc);

  AddMeasure(HLType::CacheL1D, linuxPerfTracer, KPulse::All);
  AddMeasure(HLType::ProcCounters, linuxPerfTracer, KPulse::All);
  // AddMeasure(HLType::CacheL1D, linuxPerfTracer, KPulse::All);
  // AddMeasure(HLType::CacheL1I, linuxPerfTracer, KPulse::All);
  AddMeasure(HLType::CacheDTLB, linuxPerfTracer, KPulse::All);
  // AddMeasure(HLType::CacheITLB, linuxPerfTracer, KPulse::All);
  // AddMeasure(HLType::CacheBPU, linuxPerfTracer, KPulse::All);
  // TODO: These cause 'Kokkos::Cuda ERROR: Failed to call Kokkos::Cuda::finalize()'
  // AddMeasure(HLType::CacheLL, linuxPerfTracer, KPulse::All);
  // AddMeasure(HLType::CacheNode, linuxPerfTracer, KPulse::All);

  MKP::PulseInitLibrary();

  j["hook"] = __FUNCTION__;
  j["loadSeq"] = loadSeq;
  j["interfaceVer"] = interfaceVer;
  j["devInfoCount"] = devInfoCount;
  for (auto &measure : measuresByPulse[KPulse::InitLibrary]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_finalize_library() {
  json j;

  MKP::PulseFinalizeLibrary();

  j["hook"] = __FUNCTION__;
  for (auto &measure : measuresByPulse[KPulse::FinalizeLibrary]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
  outputFile << rootJson << std::endl;
}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {
  json j;

  MKP::PulseBeginParallelFor();

  j["hook"] = __FUNCTION__;
  j["name"] = name;
  j["devID"] = devID;
  j["kID"] = *kID;
  for (auto &measure : measuresByPulse[KPulse::BeginParFor]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {
  json j;

  MKP::PulseEndParallelFor();

  j["hook"] = __FUNCTION__;
  j["kID"] = kID;
  for (auto &measure : measuresByPulse[KPulse::EndParFor]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {
  json j;

  MKP::PulseBeginParallelScan();

  j["hook"] = __FUNCTION__;
  j["name"] = name;
  j["devID"] = devID;
  j["kID"] = *kID;
  for (auto &measure : measuresByPulse[KPulse::BeginParScan]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_end_parallel_scan(const uint64_t kID) {
  json j;

  MKP::PulseEndParallelScan();

  j["hook"] = __FUNCTION__;
  j["kID"] = kID;
  for (auto &measure : measuresByPulse[KPulse::EndParScan]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {
  json j;

  MKP::PulseBeginParallelReduce();

  j["hook"] = __FUNCTION__;
  j["name"] = name;
  j["devID"] = devID;
  j["kID"] = *kID;
  for (auto &measure : measuresByPulse[KPulse::BeginParReduce]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {
  json j;

  MKP::PulseEndParallelReduce();

  j["hook"] = __FUNCTION__;
  j["kID"] = kID;
  for (auto &measure : measuresByPulse[KPulse::EndParReduce]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {
  json j;

  MKP::PulseBeginFence();

  j["hook"] = __FUNCTION__;
  j["name"] = name;
  j["devID"] = devID;
  j["kID"] = *kID;
  for (auto &measure : measuresByPulse[KPulse::BeginFence]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_end_fence(const uint64_t kID) {
  json j;

  MKP::PulseEndFence();

  j["hook"] = __FUNCTION__;
  j["kID"] = kID;
  for (auto &measure : measuresByPulse[KPulse::EndFence]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  json j;

  MKP::PulsePushProfileRegion();

  regionNameStack.push(regionName);
  j["hook"] = __FUNCTION__;
  j["regionName"] = regionName;
  for (auto &measure : measuresByPulse[KPulse::PushProfileRegion]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_pop_profile_region() {
  json j;
  auto regionNameStr = regionNameStack.top();

  MKP::PulsePopProfileRegion();

  j["hook"] = __FUNCTION__;
  j["regionName"] = regionNameStack.top();
  for (auto &measure : measuresByPulse[KPulse::PopProfileRegion]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
  regionNameStack.pop();
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {
  json j;
  std::stringstream handleStream;

  MKP::PulseAllocateData();

  handleStream << "0x" << std::hex << ptr;
  j["hook"] = __FUNCTION__;
  j["handle"] = handle.name;
  j["name"] = name;
  j["ptr"] = handleStream.str();
  j["size"] = size;
  for (auto &measure : measuresByPulse[KPulse::AllocateData]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {
  json j;
  std::stringstream handleStream;

  MKP::PulseDeallocateData();

  handleStream << "0x" << std::hex << ptr;
  j["hook"] = __FUNCTION__;
  j["handle"] = handle.name;
  j["name"] = name;
  j["ptr"] = handleStream.str();
  j["size"] = size;
  for (auto &measure : measuresByPulse[KPulse::DeallocateData]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {
  json j;
  std::stringstream srcHandleStream;
  std::stringstream dstHandleStream;

  MKP::PulseBeginDeepCopy();

  dstHandleStream << "0x" << std::hex << dst_ptr;
  srcHandleStream << "0x" << std::hex << src_ptr;

  j["hook"] = __FUNCTION__;
  j["dst_handle"] = dst_handle.name;
  j["dst_name"] = dst_name;
  j["dst_ptr"] = dstHandleStream.str();
  j["src_handle"] = src_handle.name;
  j["src_name"] = src_name;
  j["src_ptr"] = srcHandleStream.str();
  j["size"] = size;
  for (auto &measure : measuresByPulse[KPulse::BeginDeepCopy]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}

extern "C" void kokkosp_end_deep_copy() {
  json j;
  MKP::PulseEndDeepCopy();

  j["hook"] = __FUNCTION__;
  for (auto &measure : measuresByPulse[KPulse::EndDeepCopy]) {
    j.update(measure->GetJSON());
  }
  rootJson.push_back(j);
}
