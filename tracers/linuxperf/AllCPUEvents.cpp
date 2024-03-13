#include "AllCPUEvents.hpp"

#include <err.h>
#include <linux/perf_event.h>

#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

void AllCPUEvents::PerfEventOpen(uint32_t type, uint64_t config) {
  int fd;
  perf_event_attr attr{
      .type = type, .config = config, .read_format = PERF_FORMAT_GROUP};

  if (fds.size() == 0) {
    fd = perf_event_open(&attr, getpid(), -1, -1, 0);
    leader_fd = fd;
  } else {
    fd = perf_event_open(&attr, getpid(), -1, leader_fd, 0);
  }

  if (fd < 0) {
    err(EXIT_FAILURE, "cannot open fd for type %u config %lu", type, config);
  }

  fds.push_back(fd);
}

AllCPUEvents::AllCPUEvents(HLMeasureType hlType) : Measure(hlType) {
  // TODO: Didn't include all configs here, since they're aren't available
  //       So need to add checks
  PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
  PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
  PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_INSTRUCTIONS);
  PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES);
  PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES);
  // PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND);
  // PerfEventOpen(PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND);
}

void AllCPUEvents::DoMeasure() {
  memset(&result, 0, sizeof(result));
  int ret = read(leader_fd, &result, sizeof(result));
  if (ret < 0) {
    exit(EXIT_FAILURE);
  }
}

json AllCPUEvents::GetJSON() {
  json j;
  j["measure_name"] = "Linux Perf CPU Measure";
  j["cycles"] = result.cycles;
  j["insts"] = result.insts;
  j["branch_insts"] = result.branchInsts;
  j["branch_misses"] = result.branchMisses;
  j["bus_cycles"] = result.busCycles;
  // These are not available :(
  // j["stalledCycleFront"] = result.stalledCycleFront;
  // j["stalledCyclesBack"] = result.stalledCyclesBack;
  return j;
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
