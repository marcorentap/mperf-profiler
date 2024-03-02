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


namespace Tracers {
namespace Base {
class Measure {
 public:
  HLMeasureType hlType;
  Measure(HLMeasureType hlType)
      : hlType(hlType) {}
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
  virtual std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType) {
     std::unique_ptr<Measure> ptr;
     ptr.reset(new Measure(hlType));
     return ptr;
    }
};
}  // namespace Base
}  // namespace Tracers

using BaseMeasure = Tracers::Base::Measure;
using BaseTracer = Tracers::Base::Tracer;

}  // namespace MPerf
#endif
