#ifndef MPERF_CORE_HPP
#define MPERF_CORE_HPP

#include <MPerf/Lib/Json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace MPerf {

using json = nlohmann::json;

enum class HLMeasureType {
  ProcName,
  ProcCounters,
  MemorySize,
  CacheSize,
  CacheCounters,
  CacheL1D,
  CacheL1I,
  CacheLL,
  CacheDTLB,
  CacheITLB,
  CacheBPU,
  CacheNode
};

namespace Tracers {
namespace Base {
class Measure {
 public:
  Measure() {}
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
    ptr.reset(new Measure());
    return ptr;
  }
};
}  // namespace Base
}  // namespace Tracers

using Measure = Tracers::Base::Measure;
using Tracer = Tracers::Base::Tracer;

}  // namespace MPerf
#endif
