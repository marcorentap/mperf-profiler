#ifndef LINUXPERF_HPP
#define LINUXPERF_HPP
#include <linux/perf_event.h>

#include <Measure.hpp>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace MPerf {
namespace Subsystem {
namespace LinuxPerf {

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

struct PerfMeasure {
  PerfEventAttr attribute;
  int fd;
  ReadFormat readStart;
  ReadFormat readEnd;
};

class Measure : public ::MPerf::Measure {
 private:
  std::vector<int> fds;
  // Each fd has a deque that can act as a
  // queue (for begin/end measures) or a stack (for whole measures)
  std::unordered_map<int, std::deque<ReadFormat>> data;
  uint64_t result;
  void ReadAllFd();

 public:
  Measure(MeasureType type, MeasurePulse pulse)
      : ::MPerf::Measure(type, pulse) {}

  ~Measure();

  void Init();
  void DoMeasure();
  void DoNextMeasure();
  void WriteResult(std::shared_ptr<void> dest, size_t len);
};

}  // namespace LinuxPerf

}  // namespace Subsystem
}  // namespace MPerf
#endif
