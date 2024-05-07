#ifndef LINUXPERF_TRACER_HPP
#define LINUXPERF_TRACER_HPP

#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <MPerf/Core.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

using uPtrBMeasure = std::unique_ptr<Tracers::Base::Measure>;
using uPtrBTracer = std::unique_ptr<Tracers::Base::Tracer>;
using HLMType = HLMeasureType;

class CPUEvents;

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

static uint64_t MakeCacheConfig(uint64_t perf_hw_cache_id,
                                uint64_t perf_hw_cache_op_id,
                                uint64_t perf_hw_cache_op_result_id) {
  return (perf_hw_cache_id) | (perf_hw_cache_op_id << 8) |
         (perf_hw_cache_op_result_id << 16);
}

class Tracer : public ::MPerf::Tracer {
  // Mapping from hlType to label, config and type
 private:
  using hlConfigMapType =
      std::unordered_map<HLMType, std::tuple<std::string, uint32_t, uint64_t>>;
  using hlToLabelType = std::unordered_map<HLMType, std::string>;
  using hlToTypeType = std::unordered_map<HLMType, uint32_t>;
  using hlToConfigType = std::unordered_map<HLMType, uint64_t>;

  hlToLabelType hlTolabel = {
      {HLMType::HWCPUCycles, "hw_cpu_cycles"},
      {HLMType::HWInstructions, "hw_instructions"},
      {HLMType::HWCacheReferences, "hw_cache_references"},
      {HLMType::HWCacheMisses, "hw_cache_misses"},
      {HLMType::HWBranchInstructions, "hw_branch_instructions"},
      {HLMType::HWStalledCyclesFrontend, "hw_stalled_cycles_frontend"},
      {HLMType::HWStalledCyclesBackend, "hw_stalled_cycles_backend"},
      {HLMType::HWRefCPUCycles, "hw_ref_cpu_cycles"},

      {HLMType::HWCacheL1DReadAccess, "hw_cache_l1d_read_access"},
      {HLMType::HWCacheL1DReadMiss, "hw_cache_l1d_read_miss"},
      {HLMType::HWCacheL1DWriteAccess, "hw_cache_l1d_write_access"},
      {HLMType::HWCacheL1DPrefetchAccess, "hw_cache_l1d_prefetch_access"},
      {HLMType::HWCacheL1DPrefetchMiss, "hw_cache_l1d_prefetch_miss"},

      {HLMType::HWCacheL1IReadAccess, "hw_cache_l1i_read_access"},
      {HLMType::HWCacheL1IReadMiss, "hw_cache_l1i_read_miss"},
      {HLMType::HWCacheL1IWriteAccess, "hw_cache_l1i_write_access"},
      {HLMType::HWCacheL1IPrefetchAccess, "hw_cache_l1i_prefetch_access"},
      {HLMType::HWCacheL1IPrefetchMiss, "hw_cache_l1i_prefetch_miss"},

      {HLMType::HWCacheLLReadAccess, "hw_cache_ll_read_access"},
      {HLMType::HWCacheLLReadMiss, "hw_cache_ll_read_miss"},
      {HLMType::HWCacheLLWriteAccess, "hw_cache_ll_write_access"},
      {HLMType::HWCacheLLPrefetchAccess, "hw_cache_ll_prefetch_access"},
      {HLMType::HWCacheLLPrefetchMiss, "hw_cache_ll_prefetch_miss"},

      {HLMType::HWCacheDTLBReadAccess, "hw_cache_dtlb_read_access"},
      {HLMType::HWCacheDTLBReadMiss, "hw_cache_dtlb_read_miss"},
      {HLMType::HWCacheDTLBWriteAccess, "hw_cache_dtlb_write_access"},
      {HLMType::HWCacheDTLBPrefetchAccess, "hw_cache_dtlb_prefetch_access"},
      {HLMType::HWCacheDTLBPrefetchMiss, "hw_cache_dtlb_prefetch_miss"},

      {HLMType::HWCacheITLBReadAccess, "hw_cache_itlb_read_access"},
      {HLMType::HWCacheITLBReadMiss, "hw_cache_itlb_read_miss"},
      {HLMType::HWCacheITLBWriteAccess, "hw_cache_itlb_write_access"},
      {HLMType::HWCacheITLBPrefetchAccess, "hw_cache_itlb_prefetch_access"},
      {HLMType::HWCacheITLBPrefetchMiss, "hw_cache_itlb_prefetch_miss"},

      {HLMType::HWCacheBPUReadAccess, "hw_cache_bpu_read_access"},
      {HLMType::HWCacheBPUReadMiss, "hw_cache_bpu_read_miss"},
      {HLMType::HWCacheBPUWriteAccess, "hw_cache_bpu_write_access"},
      {HLMType::HWCacheBPUPrefetchAccess, "hw_cache_bpu_prefetch_access"},
      {HLMType::HWCacheBPUPrefetchMiss, "hw_cache_bpu_prefetch_miss"},

      {HLMType::HWCacheNodeReadAccess, "hw_cache_node_read_access"},
      {HLMType::HWCacheNodeReadMiss, "hw_cache_node_read_miss"},
      {HLMType::HWCacheNodeWriteAccess, "hw_cache_node_write_access"},
      {HLMType::HWCacheNodePrefetchAccess, "hw_cache_node_prefetch_access"},
      {HLMType::HWCacheNodePrefetchMiss, "hw_cache_node_prefetch_miss"},

      {HLMType::SWPageFaults, "sw_page_faults"},
      {HLMType::SWPageFaultsMin, "sw_page_faults_min"},
      {HLMType::SWPageFaultsMaj, "sw_page_faults_maj"},
  };

  hlToTypeType hlToType = {
      {HLMType::HWCPUCycles, PERF_TYPE_HARDWARE},
      {HLMType::HWInstructions, PERF_TYPE_HARDWARE},
      {HLMType::HWCacheReferences, PERF_TYPE_HARDWARE},
      {HLMType::HWCacheMisses, PERF_TYPE_HARDWARE},
      {HLMType::HWBranchInstructions, PERF_TYPE_HARDWARE},
      {HLMType::HWStalledCyclesFrontend, PERF_TYPE_HARDWARE},
      {HLMType::HWStalledCyclesBackend, PERF_TYPE_HARDWARE},
      {HLMType::HWRefCPUCycles, PERF_TYPE_HARDWARE},

      {HLMType::HWCacheL1DReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1DReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1DWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1DPrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1DPrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::HWCacheL1IReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1IReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1IWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1IPrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheL1IPrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::HWCacheLLReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheLLReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheLLWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheLLPrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheLLPrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::HWCacheDTLBReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheDTLBReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheDTLBWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheDTLBPrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheDTLBPrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::HWCacheITLBReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheITLBReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheITLBWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheITLBPrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheITLBPrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::HWCacheBPUReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheBPUReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheBPUWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheBPUPrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheBPUPrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::HWCacheNodeReadAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheNodeReadMiss, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheNodeWriteAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheNodePrefetchAccess, PERF_TYPE_HW_CACHE},
      {HLMType::HWCacheNodePrefetchMiss, PERF_TYPE_HW_CACHE},

      {HLMType::SWPageFaults, PERF_TYPE_SOFTWARE},
      {HLMType::SWPageFaultsMin, PERF_TYPE_SOFTWARE},
      {HLMType::SWPageFaultsMaj, PERF_TYPE_SOFTWARE},
  };

  hlToConfigType hlToConfig = {
      {HLMType::HWCPUCycles, PERF_COUNT_HW_CPU_CYCLES},
      {HLMType::HWInstructions, PERF_COUNT_HW_INSTRUCTIONS},
      {HLMType::HWCacheReferences, PERF_COUNT_HW_CACHE_REFERENCES},
      {HLMType::HWCacheMisses, PERF_COUNT_HW_CACHE_MISSES},
      {HLMType::HWBranchInstructions, PERF_COUNT_HW_BRANCH_INSTRUCTIONS},
      {HLMType::HWStalledCyclesFrontend, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND},
      {HLMType::HWStalledCyclesBackend, PERF_COUNT_HW_STALLED_CYCLES_BACKEND},
      {HLMType::HWRefCPUCycles, PERF_COUNT_HW_REF_CPU_CYCLES},

      {HLMType::HWCacheL1DReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheL1DReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheL1DWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheL1DPrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheL1DPrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::HWCacheL1IReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheL1IReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheL1IWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheL1IPrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheL1IPrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_L1I, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::HWCacheLLReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheLLReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheLLWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheLLPrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheLLPrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_LL, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::HWCacheDTLBReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_DTLB, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheDTLBReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_DTLB, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheDTLBWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_DTLB, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheDTLBPrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_DTLB,
                       PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheDTLBPrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_DTLB,
                       PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::HWCacheITLBReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_ITLB, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheITLBReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_ITLB, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheITLBWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_ITLB, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheITLBPrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_ITLB,
                       PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheITLBPrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_ITLB,
                       PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::HWCacheBPUReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_BPU, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheBPUReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_BPU, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheBPUWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_BPU, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheBPUPrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_BPU, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheBPUPrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_BPU, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::HWCacheNodeReadAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_NODE, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheNodeReadMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_NODE, PERF_COUNT_HW_CACHE_OP_READ,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},
      {HLMType::HWCacheNodeWriteAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_NODE, PERF_COUNT_HW_CACHE_OP_WRITE,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheNodePrefetchAccess,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_NODE,
                       PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_ACCESS)},
      {HLMType::HWCacheNodePrefetchMiss,
       MakeCacheConfig(PERF_COUNT_HW_CACHE_NODE,
                       PERF_COUNT_HW_CACHE_OP_PREFETCH,
                       PERF_COUNT_HW_CACHE_RESULT_MISS)},

      {HLMType::SWPageFaults, PERF_COUNT_SW_PAGE_FAULTS},
      {HLMType::SWPageFaultsMin, PERF_COUNT_SW_PAGE_FAULTS_MIN},
      {HLMType::SWPageFaultsMaj, PERF_COUNT_SW_PAGE_FAULTS_MAJ},
  };

 public:
  Tracer() {}
  uPtrBMeasure MakeMeasure(HLMType hlType) override;
  uPtrBMeasure MakeMeasure(std::vector<HLMType> hlTypes) override;
};

class Measure : public ::MPerf::Measure {
 protected:
  struct PerfEventAttr {
    uint64_t type;
    uint64_t config;
  };
  struct ReadFormat {
    uint64_t nr;                      // Number of events
    std::array<uint64_t, 64> values;  // Allow maximum of 64 events per
                                      // group and per measure
  } result;
  std::vector<int> fds;
  // For use in fd and result.values
  std::unordered_map<std::string, int> labelToResultIndex;
  int leader_fd;

 public:
  void PerfEventOpen(uint32_t type, uint64_t config);
  void PerfEventOpen(std::string label, uint32_t type, uint64_t config);
  int GetOpenFDCount();

  ~Measure() {
    for (auto &fd : fds) {
      close(fd);
    }
  }

  virtual void DoMeasure();
  virtual json GetJSON();
};

using uPtrLinuxMeasure = std::unique_ptr<Tracers::LinuxPerf::Measure>;
using uPtrLinuxTracer = std::unique_ptr<Tracers::LinuxPerf::Tracer>;
}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
