#ifndef LINUXPERF_TRACER_HPP
#define LINUXPERF_TRACER_HPP

#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <unistd.h>
#include <iostream>

#include "MPerf/Core.hpp"

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class ProcMeasure;

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

class Tracer : public BaseTracer {
 public:
  Tracer() { std::cout << "Make LinuxTracer" << std::endl; }
  std::unique_ptr<BaseMeasure> MakeMeasure(HLMeasureType hlType,
                                           MeasurePulse mPulse);
};

class Measure : public BaseMeasure {
 protected:
  struct PerfEventAttr {
    uint64_t type;
    uint64_t config;
  };
  struct ReadFormat {
    uint64_t value;        /* The value of the event */
    uint64_t time_enabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    uint64_t time_running; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    uint64_t id;           /* if PERF_FORMAT_ID */
  };

 private:
  std::vector<int> fds;

 public:
  Measure(HLMeasureType hlType, MeasurePulse pulse)
      : BaseMeasure(hlType, pulse) {}

  ~Measure() {
    for (auto &fd : fds) {
      close(fd);
    }
  }

  virtual void Init() {}
  virtual void DoMeasure() {}
  virtual void DoNextMeasure() {}
  virtual void WriteResult(void *dest, size_t len) {}
  virtual void WriteResult(std::shared_ptr<void> dest, size_t len) {}
  virtual json GetJSON() {
    json j;
    j["name"] = "Linux Perf Measure";
    return j;
  }
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
