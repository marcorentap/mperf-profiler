#include <asm/unistd.h>
#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <LinuxPerf.hpp>
#include <cstring>

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

void InitCPI() {
  int fd;
  struct perf_event_attr attr;
  auto CycleConfig = PerfEventAttr{.type = PERF_TYPE_HARDWARE,
                                   .config = PERF_COUNT_HW_CPU_CYCLES};
  auto InstConfig = PerfEventAttr{.type = PERF_TYPE_HARDWARE,
                                  .config = PERF_COUNT_HW_INSTRUCTIONS};

  memset(&attr, 0, sizeof(attr));
  attr.config = CycleConfig.config;
  attr.type = CycleConfig.type;
  fd = perf_event_open(&attr, getpid(), -1, -1, 0);
  if (fd < 0) {
    perror("cannot open fd for CPU cycles");
    exit(EXIT_FAILURE);
  }

  memset(&attr, 0, sizeof(attr));
  attr.config = InstConfig.config;
  attr.type = InstConfig.type;
  fd = perf_event_open(&attr, getpid(), -1, -1, 0);
  if (fd < 0) {
    perror("cannot open fd for instruction count");
    exit(EXIT_FAILURE);
  }
}

void Measure::Init() {
  std::cout << "Initialize LinuxPerf Measure" << std::endl;
  if (type == MeasureType::LinuxPerfCPI) {
    InitCPI();
  }
}
void Measure::ReadValues() {
  std::cout << "LinuxPerf Read Values" << std::endl;
}

Measure::~Measure() {
  for (auto &fd : fds) {
    close(fd);
  }
}
}  // namespace LinuxPerf
}  // namespace Subsystem
}  // namespace MPerf
