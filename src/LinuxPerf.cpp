#include <asm/unistd.h>
#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <LinuxPerf.hpp>
#include <cstring>
#include <memory>

#include "Measure.hpp"

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
  return ret;
}

namespace MPerf {
namespace Subsystem {
namespace LinuxPerf {

std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType, MeasureType type,
                                     MeasurePulse pulse) {
  std::unique_ptr<Measure> ptr;
  if (hlType == HLMeasureType::LinuxPerfProc) {
    ptr.reset(new ProcMeasure(hlType, type, pulse));
  } else {
    ptr.reset(new Measure(hlType, type, pulse));
  }

  return ptr;
}

}  // namespace LinuxPerf
}  // namespace Subsystem
}  // namespace MPerf
