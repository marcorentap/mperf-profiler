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

class AllCPUEvents;

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

class Tracer : public ::MPerf::Tracer {
 public:
  Tracer() {}
  std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType) override;
};

class Measure : public ::MPerf::Measure{
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
  std::vector<int> fds;

 public:
  Measure(HLMeasureType hlType) : ::MPerf::Measure(hlType) {}

  ~Measure() {
    for (auto &fd : fds) {
      close(fd);
    }
  }

  virtual void DoMeasure() {}
  virtual json GetJSON() {
    json j;
    j["measureName"] = "Default Linux Perf Measure";
    return j;
  }
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
