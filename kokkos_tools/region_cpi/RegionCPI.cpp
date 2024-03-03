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

constexpr auto outputFileName = "mperf_region_cpi.json";

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
  json patch;
  LinuxTracer linuxTracer;
  outputFile.open(outputFileName, std::ofstream::trunc);

  AddMeasure(HLType::ProcCounters, linuxTracer, KPulse::WholeProfileRegion);

  MKP::DoInitLibrary();

  patch["hook"] = __FUNCTION__;
  patch["loadSeq"] = loadSeq;
  patch["interfaceVer"] = interfaceVer;
  patch["devInfoCount"] = devInfoCount;
  AddPulseMeasuresToJson(KPulse::InitLibrary, patch);
}

extern "C" void kokkosp_finalize_library() {
  // Don't measure anything, just write json to file
  outputFile << rootJson << std::endl;
}


extern "C" void kokkosp_push_profile_region(char *regionName) {
  json patch;

  MKP::DoPushProfileRegion();

  regionNameStack.push(regionName);
  patch["regionName"] = regionName;

  AddPulseMeasuresToJson(KPulse::PushProfileRegion, patch);
}

extern "C" void kokkosp_pop_profile_region() {
  json patch;
  auto regionNameStr = regionNameStack.top();

  MKP::DoPopProfileRegion();

  patch["regionName"] = regionNameStack.top();
  AddPulseMeasuresToJson(KPulse::PopProfileRegion, patch);
  regionNameStack.pop();
}
