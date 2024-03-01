#ifndef MPERF_HPP
#define MPERF_HPP
#include <linux/perf_event.h>

#include <cstdint>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "Json.hpp"

namespace MPerf {
using json = nlohmann::json;

enum class HLMeasureType {
  InitDummy,
  RegionCPI,
  LibraryCPI,
  SystemInfo,
  LinuxPerfProc
};

enum class MeasurePulse {
  InitLibrary,
  FinalizeLibrary,
  WholeLibrary,
  BeginParFor,
  EndParFor,
  WholeParFor,
  BeginParScan,
  EndParScan,
  WholeParScan,
  BeginParReduce,
  EndParReduce,
  WholeParReduce,
  BeginParFence,
  EndParFence,
  WholeParFence,
  PushProfileRegion,
  PopProfileRegion,
  WholeProfileRegion,
  AllocateData,
  DeallocateData,
  WholeData,
  BeginDeepCopy,
  EndDeepCopy,
  WholeDeepCopy,
};

namespace Tracers {
namespace Base {

class Measure {
 public:
  HLMeasureType hlType;
  MeasurePulse pulse;
  Measure(HLMeasureType hlType, MeasurePulse pulse)
      : hlType(hlType), pulse(pulse) {}
  virtual void Init() {}
  virtual void DoMeasure() {}
  virtual json GetJSON() {
    json j;
    j["name"] = "Default Measure";
    return j;
  }
};

class Tracer {
 public:
  virtual std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType,
                                               MeasurePulse mPulse) {
    std::unique_ptr<Measure> ptr;
    ptr.reset(new Measure(hlType, mPulse));
    return ptr;
  }
};
}  // namespace Base
}  // namespace Tracers
using BaseTracer = Tracers::Base::Tracer;
using BaseMeasure = Tracers::Base::Measure;

class MPerf {
 private:
  using HLType = HLMeasureType;
  using MPulse = MeasurePulse;
  std::vector<std::tuple<BaseTracer, HLMeasureType, MPulse>> __measures;
  std::unordered_map<MPulse, std::vector<std::shared_ptr<BaseMeasure>>>
      measuresByPulse;

 public:
  MPerf();
  void AddMeasure(BaseTracer&, HLMeasureType, MPulse);
  std::vector<std::shared_ptr<BaseMeasure>>& PulseMeasures(MPulse mPulse);
  void PulseDoMeasure(MPulse mPulse);
  void PulseDoNextMeasure(MPulse mPulse);
  std::shared_ptr<BaseMeasure> GetMeasure(HLMeasureType HLType);
};
}  // namespace MPerf
#endif
