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
constexpr auto outputFileName = "mperf_resource_vector.json";

std::stack<std::string> regionNameStack;
std::unordered_map<std::string, std::vector<double>> regionCPIs;
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
  auto eventStack = std::stack<json>{};

  // Calculate CPIs per region
  for (auto &event : eventJson) {
    if (event["hook"] == "kokkosp_push_profile_region") {
      eventStack.push(event);
      continue;
    }

    if (event["hook"] == "kokkosp_pop_profile_region") {
      json regionCPI;
      json regionEnd = event;
      json regionBegin = eventStack.top();
      std::string regionName = regionBegin["region_name"];

      uint64_t beginInsts = regionBegin["instructions"];
      uint64_t endInsts = regionEnd["instructions"];
      uint64_t beginCycles = regionBegin["cycles"];
      uint64_t endCycles = regionEnd["cycles"];
      uint64_t instDiff = endInsts - beginInsts;
      uint64_t cycleDiff = endCycles - beginCycles;
      double CPI = (double)cycleDiff / instDiff;

      regionCPIs[regionName].push_back(CPI);
      eventStack.pop();
    }
  }

  // Calculate average CPI
  json outJson;
  for (auto &region : regionCPIs) {
    json regionJson;
    auto CPIs = region.second;
    auto regionName = region.first;
    double averageCPI =
        (double)std::accumulate(CPIs.begin(), CPIs.end(), 0.0) / CPIs.size();

    regionJson["region_name"] = region.first;
    regionJson["cpi"] = averageCPI;
    regionJson["ipc"] = 1.0 / averageCPI;
    outJson.push_back(regionJson);
  }

  outputFile << outJson << std::endl;
}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  json out, cpuEvents;

  cpuEventMeasure->DoMeasure();
  cpuEvents = cpuEventMeasure->GetJSON();

  regionNameStack.push(regionName);
  out["region_name"] = regionName;
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

  out["region_name"] = regionName;
  out["instructions"] = cpuEvents["insts"];
  out["cycles"] = cpuEvents["cycles"];
  out["hook"] = __FUNCTION__;
  eventJson.push_back(out);

  regionNameStack.pop();
}
