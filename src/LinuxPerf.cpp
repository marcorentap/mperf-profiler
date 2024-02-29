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

void Measure::Init() {
  std::cout << "Initialize LinuxPerf Measure" << std::endl;
}

void Measure::ReadAllFd() {
  for (auto &fd : fds) {
    struct ReadFormat readFormat;
    read(fd, &readFormat, sizeof(readFormat));
    data[fd].push_back(readFormat);
  }
}

// Default result is value from first fd
void Measure::DoMeasure() {
  int firstFd = fds[0];
  std::cout << "LinuxPerf Read Values" << std::endl;
  ReadAllFd();
  result = data.at(firstFd).back().value;
}

// Default result is diff of start and end value of first fd
void Measure::DoNextMeasure() {
  int firstFd = fds[0];
  auto prev = data.at(firstFd).back();

  std::cout << "LinuxPerf Read Next Values" << std::endl;
  // TODO: Skip stack push
  ReadAllFd();
  auto curr = data.at(firstFd).back();
  result = curr.value - prev.value;

  data.at(firstFd).pop_back();
  data.at(firstFd).pop_back();
}

void Measure::WriteResult(void *dest, size_t len) {
  std::memcpy(dest, &result, len);
}

void Measure::WriteResult(std::shared_ptr<void> dest, size_t len) {
  std::memcpy(dest.get(), &result, len);
}

json Measure::GetJSON() {
  json j;
  j["name"] = "Linux Perf Measure";
  return j;
}

Measure::~Measure() {
  for (auto &fd : fds) {
    close(fd);
  }
}
}  // namespace LinuxPerf
}  // namespace Subsystem
}  // namespace MPerf
