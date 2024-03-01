#ifndef MPERF_CORE_HPP
#define MPERF_CORE_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include "MPerf/Lib/Json.hpp"

namespace MPerf {

using json = nlohmann::json;

enum class HLMeasureType {
  InitDummy,
  RegionCPI,
  LibraryCPI,
  SystemInfo,
  LinuxPerfProc
};

 // TODO: Handling of pulses should be in Kokkos Tools, not MPerf.
 // MPerf abstracts away counter/measures subsystems
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

using BaseMeasure = Tracers::Base::Measure;
using BaseTracer = Tracers::Base::Tracer;

class MPerf {
 private:
  std::vector<std::shared_ptr<BaseMeasure>> measures;
  std::unordered_map<MeasurePulse, std::vector<std::shared_ptr<BaseMeasure>>>
      measuresByPulse;

 public:
  MPerf();
  std::shared_ptr<BaseMeasure> AddMeasure(BaseTracer& tracer, HLMeasureType hlType,
                  MeasurePulse mPulse);
  std::vector<std::shared_ptr<BaseMeasure>>& PulseMeasures(MeasurePulse mPulse);
  void PulseDoMeasure(MeasurePulse mPulse);
  void PulseDoNextMeasure(MeasurePulse mPulse);
  std::shared_ptr<BaseMeasure> GetMeasure(HLMeasureType HLType);
};

}  // namespace MPerf
#endif
