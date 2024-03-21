#ifndef MPERF_CORE_HPP
#define MPERF_CORE_HPP

#include <MPerf/Lib/Json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace MPerf {

using json = nlohmann::json;

enum class HLMeasureType {
  // These will be deprecated
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
  CacheNode,

  // Fine grained types from now on
  HWCPUCycles,
  HWInstructions,
  HWCacheReferences,
  HWCacheMisses,
  HWBranchInstructions,
  HWStalledCyclesFrontend,
  HWStalledCyclesBackend,
  HWRefCPUCycles,
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
using bMeasurePtr = std::unique_ptr<Measure>;

class Tracer {
 public:
  virtual bMeasurePtr MakeMeasure(HLMeasureType hlType) {
    bMeasurePtr ptr;
    ptr.reset(new Measure());
    return ptr;
  }
  virtual bMeasurePtr MakeMeasure(std::vector<HLMeasureType> hlTypes) {
    bMeasurePtr ptr;
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
