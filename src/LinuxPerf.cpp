#include <asm/unistd_64.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <LinuxPerf.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
  return ret;
}

namespace MPerf {
namespace Subsystem {
namespace LinuxPerf {

void Measure::Init() {
  std::cout << "Initialize LinuxPerf Measure" << std::endl;
}
}  // namespace LinuxPerf
}  // namespace Subsystem
}  // namespace MPerf
