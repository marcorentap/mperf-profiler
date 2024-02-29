#ifndef LINUXPERF_HPP
#define LINUXPERF_HPP
#include <linux/perf_event.h>

#include <Measure.hpp>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include <unistd.h>

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
  Measure(HLMeasureType hlType, MeasureType type, MeasurePulse pulse)
      : ::MPerf::Measure(hlType, type, pulse) {}

  ~Measure() {
          for (auto &fd : fds) {
            close(fd);
          }
        }

  virtual void Init() {}
  virtual void DoMeasure() {}
  virtual void DoNextMeasure() {}
  virtual void WriteResult(void *dest, size_t len) {}
  virtual void WriteResult(std::shared_ptr<void> dest, size_t len) {}
  virtual json GetJSON() {
    json j;
    j["name"] = "Linux Perf Measure";
    return j;
  }
};

class ProcMeasure : public Measure {
 private:
  // TODO: Derive read_format depend on Init() instead of manually here
  struct read_format {
    uint64_t nr;
    uint64_t instCount;
    uint64_t cycleCount;
  } result;
  int leader_fd;

 public:
  ProcMeasure(HLMeasureType hlType, MeasureType type, MeasurePulse pulse)
      : Measure(hlType, type, pulse) {}
  void Init();
  void DoMeasure();
  json GetJSON();
};

std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType, MeasureType type,
                                     MeasurePulse pulse);

}  // namespace LinuxPerf

}  // namespace Subsystem
}  // namespace MPerf
#endif
