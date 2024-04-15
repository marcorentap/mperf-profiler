#ifndef MPERF_CORE_HPP
#define MPERF_CORE_HPP

#include <MPerf/Lib/Json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace MPerf {

using json = nlohmann::json;

enum class HLMeasureType {
  HWCPUCycles,
  HWInstructions,
  HWCacheReferences,
  HWCacheMisses,
  HWBranchInstructions,
  HWStalledCyclesFrontend,
  HWStalledCyclesBackend,
  HWRefCPUCycles,

  HWCacheL1DReadAccess,
  HWCacheL1DReadMiss,
  HWCacheL1DWriteAccess,
  HWCacheL1DPrefetchAccess,
  HWCacheL1DPrefetchMiss,

  HWCacheL1IReadAccess,
  HWCacheL1IReadMiss,
  HWCacheL1IWriteAccess,
  HWCacheL1IPrefetchAccess,
  HWCacheL1IPrefetchMiss,

  HWCacheLLReadAccess,
  HWCacheLLReadMiss,
  HWCacheLLWriteAccess,
  HWCacheLLPrefetchAccess,
  HWCacheLLPrefetchMiss,

  HWCacheDTLBReadAccess,
  HWCacheDTLBReadMiss,
  HWCacheDTLBWriteAccess,
  HWCacheDTLBPrefetchAccess,
  HWCacheDTLBPrefetchMiss,

  HWCacheITLBReadAccess,
  HWCacheITLBReadMiss,
  HWCacheITLBWriteAccess,
  HWCacheITLBPrefetchAccess,
  HWCacheITLBPrefetchMiss,

  HWCacheBPUReadAccess,
  HWCacheBPUReadMiss,
  HWCacheBPUWriteAccess,
  HWCacheBPUPrefetchAccess,
  HWCacheBPUPrefetchMiss,

  HWCacheNodeReadAccess,
  HWCacheNodeReadMiss,
  HWCacheNodeWriteAccess,
  HWCacheNodePrefetchAccess,
  HWCacheNodePrefetchMiss,

  SWPageFaults,
  SWPageFaultsMin,
  SWPageFaultsMaj,
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
