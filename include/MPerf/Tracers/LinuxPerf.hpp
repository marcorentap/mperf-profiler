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
      std::unordered_map<HLMeasureType,
                         std::tuple<std::string, uint32_t, uint64_t>>;
  hlConfigMapType hlArgsMap = {
      {HLMeasureType::HWCPUCycles, {"hw_cpu_cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES}},
      {HLMeasureType::HWInstructions, {"hw_instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS}},
      {HLMeasureType::HWCacheReferences, {"hw_cache_references", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_REFERENCES}},
      {HLMeasureType::HWCacheMisses, {"hw_cache_misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES}},
      {HLMeasureType::HWBranchInstructions, {"hw_branch_instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS}},
      {HLMeasureType::HWStalledCyclesFrontend, {"hw_stalled_cycles_frontend", PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND}},
      {HLMeasureType::HWStalledCyclesBackend, {"hw_stalled_cycles_backend", PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND}},
      {HLMeasureType::HWRefCPUCycles, {"hw_ref_cpu_cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_REF_CPU_CYCLES}},
      };

 public:
  Tracer() {}
  uPtrBMeasure MakeMeasure(HLMeasureType hlType) override;
  uPtrBMeasure MakeMeasure(std::vector<HLMeasureType> hlTypes) override;
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
