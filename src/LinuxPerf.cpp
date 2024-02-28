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

  // Create fd for CPU cycles
  memset(&attr, 0, sizeof(attr));
  attr.config = PERF_COUNT_HW_CPU_CYCLES;
  attr.type = PERF_TYPE_HARDWARE;
  fd = perf_event_open(&attr, getpid(), -1, -1, 0);
  if (fd < 0) {
    perror("cannot open fd for CPU cycles");
    exit(EXIT_FAILURE);
  }

  // Create fd for instruction count
  memset(&attr, 0, sizeof(attr));
  attr.config = PERF_COUNT_HW_INSTRUCTIONS;
  attr.type = PERF_TYPE_HARDWARE;
  fd = perf_event_open(&attr, getpid(), -1, -1, 0);
  if (fd < 0) {
    perror("cannot open fd for instruction count");
    exit(EXIT_FAILURE);
  }
}

void Measure::Init() {
  std::cout << "Initialize LinuxPerf Measure" << std::endl;
  switch (type) {
    case MeasureType::LinuxPerf:
      InitCPI();
      break;
    default:
      break;
  }
}

void Measure::ReadAllFd() {
  for (auto &fd : fds) {
    struct ReadFormat readFormat;
    read(fd, &readFormat, sizeof(readFormat));
    data[fd].push_back(readFormat);
  }
}

void Measure::DoMeasure() {
  std::cout << "LinuxPerf Read Values" << std::endl;
  ReadAllFd();
}

void Measure::DoNextMeasure() {
  for (auto &fd : fds) {
    struct ReadFormat readFormat;
    read(fd, &readFormat, sizeof(readFormat));
    data[fd].push_back(readFormat);
  }
}

void Measure::WriteResult(std::shared_ptr<void> dest, size_t len) {
    std::memcpy(dest.get(), &result, len);
}

Measure::~Measure() {
  for (auto &fd : fds) {
    close(fd);
  }
}
}  // namespace LinuxPerf
}  // namespace Subsystem
}  // namespace MPerf
