#ifndef MPERF_CORE_HPP
#define MPERF_CORE_HPP

#include <MPerf/Lib/Json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace MPerf {

using json = nlohmann::json;

enum class HLMeasureType {
  Time,
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

  HWCycleStallsTotal,
  HWCycleStallsL1DMiss,
  HWCycleStallsL2Miss,
  HWCycleStallsL3Miss,
  HWCycleStallsMemAny,
};

using HLTypeLabelMapType = std::unordered_map<HLMeasureType, std::string>;
static HLTypeLabelMapType HLTypeLabels = {
    {HLMeasureType::Time, "time"},
    {HLMeasureType::HWCPUCycles, "hw_cpu_cycles"},
    {HLMeasureType::HWInstructions, "hw_instructions"},
    {HLMeasureType::HWCacheReferences, "hw_cache_references"},
    {HLMeasureType::HWCacheMisses, "hw_cache_misses"},
    {HLMeasureType::HWBranchInstructions, "hw_branch_instructions"},
    {HLMeasureType::HWStalledCyclesFrontend, "hw_stalled_cycles_frontend"},
    {HLMeasureType::HWStalledCyclesBackend, "hw_stalled_cycles_backend"},
    {HLMeasureType::HWRefCPUCycles, "hw_ref_cpu_cycles"},

    {HLMeasureType::HWCacheL1DReadAccess, "hw_cache_l1d_read_access"},
    {HLMeasureType::HWCacheL1DReadMiss, "hw_cache_l1d_read_miss"},
    {HLMeasureType::HWCacheL1DWriteAccess, "hw_cache_l1d_write_access"},
    {HLMeasureType::HWCacheL1DPrefetchAccess, "hw_cache_l1d_prefetch_access"},
    {HLMeasureType::HWCacheL1DPrefetchMiss, "hw_cache_l1d_prefetch_miss"},

    {HLMeasureType::HWCacheL1IReadAccess, "hw_cache_l1i_read_access"},
    {HLMeasureType::HWCacheL1IReadMiss, "hw_cache_l1i_read_miss"},
    {HLMeasureType::HWCacheL1IWriteAccess, "hw_cache_l1i_write_access"},
    {HLMeasureType::HWCacheL1IPrefetchAccess, "hw_cache_l1i_prefetch_access"},
    {HLMeasureType::HWCacheL1IPrefetchMiss, "hw_cache_l1i_prefetch_miss"},

    {HLMeasureType::HWCacheLLReadAccess, "hw_cache_ll_read_access"},
    {HLMeasureType::HWCacheLLReadMiss, "hw_cache_ll_read_miss"},
    {HLMeasureType::HWCacheLLWriteAccess, "hw_cache_ll_write_access"},
    {HLMeasureType::HWCacheLLPrefetchAccess, "hw_cache_ll_prefetch_access"},
    {HLMeasureType::HWCacheLLPrefetchMiss, "hw_cache_ll_prefetch_miss"},

    {HLMeasureType::HWCacheDTLBReadAccess, "hw_cache_dtlb_read_access"},
    {HLMeasureType::HWCacheDTLBReadMiss, "hw_cache_dtlb_read_miss"},
    {HLMeasureType::HWCacheDTLBWriteAccess, "hw_cache_dtlb_write_access"},
    {HLMeasureType::HWCacheDTLBPrefetchAccess, "hw_cache_dtlb_prefetch_access"},
    {HLMeasureType::HWCacheDTLBPrefetchMiss, "hw_cache_dtlb_prefetch_miss"},

    {HLMeasureType::HWCacheITLBReadAccess, "hw_cache_itlb_read_access"},
    {HLMeasureType::HWCacheITLBReadMiss, "hw_cache_itlb_read_miss"},
    {HLMeasureType::HWCacheITLBWriteAccess, "hw_cache_itlb_write_access"},
    {HLMeasureType::HWCacheITLBPrefetchAccess, "hw_cache_itlb_prefetch_access"},
    {HLMeasureType::HWCacheITLBPrefetchMiss, "hw_cache_itlb_prefetch_miss"},

    {HLMeasureType::HWCacheBPUReadAccess, "hw_cache_bpu_read_access"},
    {HLMeasureType::HWCacheBPUReadMiss, "hw_cache_bpu_read_miss"},
    {HLMeasureType::HWCacheBPUWriteAccess, "hw_cache_bpu_write_access"},
    {HLMeasureType::HWCacheBPUPrefetchAccess, "hw_cache_bpu_prefetch_access"},
    {HLMeasureType::HWCacheBPUPrefetchMiss, "hw_cache_bpu_prefetch_miss"},

    {HLMeasureType::HWCacheNodeReadAccess, "hw_cache_node_read_access"},
    {HLMeasureType::HWCacheNodeReadMiss, "hw_cache_node_read_miss"},
    {HLMeasureType::HWCacheNodeWriteAccess, "hw_cache_node_write_access"},
    {HLMeasureType::HWCacheNodePrefetchAccess, "hw_cache_node_prefetch_access"},
    {HLMeasureType::HWCacheNodePrefetchMiss, "hw_cache_node_prefetch_miss"},

    {HLMeasureType::SWPageFaults, "sw_page_faults"},
    {HLMeasureType::SWPageFaultsMin, "sw_page_faults_min"},
    {HLMeasureType::SWPageFaultsMaj, "sw_page_faults_maj"},

    {HLMeasureType::HWCycleStallsTotal, "hw_cycle_stalls_total"},
    {HLMeasureType::HWCycleStallsL1DMiss, "hw_cycle_stalls_l1d_miss"},
    {HLMeasureType::HWCycleStallsL2Miss, "hw_cycle_stalls_l2_miss"},
    {HLMeasureType::HWCycleStallsL3Miss, "hw_cycle_stalls_l3_miss"},
    {HLMeasureType::HWCycleStallsMemAny, "hw_cycle_stalls_mem_any"},
};

namespace Tracers {
namespace Base {
class Measure {
 public:
  Measure() {}
  virtual void Reset() {}
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
