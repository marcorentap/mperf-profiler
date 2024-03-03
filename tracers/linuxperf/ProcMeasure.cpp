#include "ProcMeasure.hpp"

#include <err.h>

#include "MPerf/Tracers/LinuxPerf.hpp"

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

ProcMeasure::ProcMeasure(HLMeasureType hlType)
    : Measure(hlType) {
  int inst_fd, cycle_fd;
  perf_event_attr instAttr, cycleAttr;
  auto className = typeid(*this).name();

  memset(&instAttr, 0, sizeof(instAttr));
  memset(&cycleAttr, 0, sizeof(cycleAttr));

  instAttr.type = PERF_TYPE_HARDWARE;
  instAttr.config = PERF_COUNT_HW_INSTRUCTIONS;
  instAttr.read_format = PERF_FORMAT_GROUP;

  cycleAttr.type = PERF_TYPE_HARDWARE;
  cycleAttr.config = PERF_COUNT_HW_CPU_CYCLES;
  cycleAttr.read_format = PERF_FORMAT_GROUP;

  // Use inst_fd as group leader
  // TODO: Do proper exception by skipping the measure instead of exiting
  inst_fd = perf_event_open(&instAttr, getpid(), -1, -1, 0);
  if (inst_fd < 0) {
    err(EXIT_FAILURE, "%s cannot open fd for instruction count", className);
  }
  leader_fd = inst_fd;

  cycle_fd = perf_event_open(&cycleAttr, getpid(), -1, inst_fd, 0);
  if (cycle_fd < 0) {
    err(EXIT_FAILURE, "%s cannot open fd for cycle count", className);
  }
}

void ProcMeasure::DoMeasure() {
  memset(&result, 0, sizeof(result));
  int ret = read(leader_fd, &result, sizeof(result));
  if (ret < 0) {
    exit(EXIT_FAILURE);
  }
}

json ProcMeasure::GetJSON() {
  json j;
  j["name"] = "Linux Perf CPU Measure";
  j["instCount"] = result.instCount;
  j["cycleCount"] = result.cycleCount;

  return j;
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
