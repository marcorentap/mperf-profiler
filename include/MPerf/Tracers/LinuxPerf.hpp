#ifndef LINUXPERF_TRACER_HPP
#define LINUXPERF_TRACER_HPP

#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <MPerf/Core.hpp>
#include <iostream>

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
