#include <fstream>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>

#include "MPerf/Core.hpp"
#include "MPerf/Tracers/LinuxPerf.hpp"

using json = nlohmann::json;
using Measure = MPerf::Measure;
using PerfTracer = MPerf::Tracers::LinuxPerf::Tracer;
constexpr auto outputFileName = "mperf_region_cpi.json";

std::stack<std::string> regionNameStack;
std::ofstream outputFile;
json eventJson, cpiJson;
std::unique_ptr<Measure> cpuEventMeasure;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t devInfoCount,
                                     void *deviceInfo) {
  // Measure processor events with linux's perf 
  auto perfTracer = PerfTracer();
  cpuEventMeasure = perfTracer.MakeMeasure(MPerf::HLMeasureType::ProcCounters);
  outputFile.open(outputFileName, std::ofstream::trunc);
}

extern "C" void kokkosp_finalize_library() {
  outputFile << eventJson << std::endl;
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  json out, cpuEvents;

  cpuEventMeasure->DoMeasure();
  cpuEvents = cpuEventMeasure->GetJSON();

  regionNameStack.push(regionName);
  out["regionName"] = regionName;
  out["instructions"] = cpuEvents["insts"];
  out["cycles"] = cpuEvents["cycles"];
  out["hook"] = __FUNCTION__;
  eventJson.push_back(out);
}

extern "C" void kokkosp_pop_profile_region() {
  json out, cpuEvents;
  std::string regionName;

  cpuEventMeasure->DoMeasure();
  cpuEvents = cpuEventMeasure->GetJSON();
  regionName = regionNameStack.top();

  out["regionName"] = regionName;
  out["instructions"] = cpuEvents["insts"];
  out["cycles"] = cpuEvents["cycles"];
  out["hook"] = __FUNCTION__;
  eventJson.push_back(out);

  regionNameStack.pop();
}
