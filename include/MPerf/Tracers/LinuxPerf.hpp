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
 public:
  Tracer() {}
  std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType) override;
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
  void PerfEventOpen(uint32_t type, uint64_t config);
  void PerfEventOpen(std::string label, uint32_t type, uint64_t config);
  int leader_fd;

 public:
  Measure(HLMeasureType hlType) : ::MPerf::Measure(hlType) {}

  ~Measure() {
    for (auto &fd : fds) {
      close(fd);
    }
  }

  virtual void DoMeasure();
  virtual json GetJSON();
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
