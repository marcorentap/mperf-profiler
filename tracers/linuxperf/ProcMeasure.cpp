#include "Tracer.hpp"
#include "ProcMeasure.hpp"

#include <err.h>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

void ProcMeasure::Init() {
  int inst_fd, cycle_fd;
  perf_event_attr inst_attr, cycle_attr;
  auto className = typeid(*this).name();

  memset(&inst_attr, 0, sizeof(inst_attr));
  memset(&cycle_attr, 0, sizeof(cycle_attr));

  inst_attr.type = PERF_TYPE_HARDWARE;
  inst_attr.config = PERF_COUNT_HW_INSTRUCTIONS;
  inst_attr.read_format = PERF_FORMAT_GROUP;

  cycle_attr.type = PERF_TYPE_HARDWARE;
  cycle_attr.config = PERF_COUNT_HW_CPU_CYCLES;
  cycle_attr.read_format = PERF_FORMAT_GROUP;

  // Use inst_fd as group leader
  // TODO: Do proper exception by skipping the measure instead of exiting
  inst_fd = perf_event_open(&inst_attr, getpid(), -1, -1, 0);
  if (inst_fd < 0) {
    err(EXIT_FAILURE, "%s cannot open fd for instruction count", className);
  }
  leader_fd = inst_fd;

  cycle_fd = perf_event_open(&cycle_attr, getpid(), -1, inst_fd, 0);
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
