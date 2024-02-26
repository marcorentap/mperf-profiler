#include <cstdint>
#include <linux/perf_event.h>
#include <vector>
#include <unordered_map>

namespace MPerf {

class MPerf {
private:
  enum PerfAttrConfig {
    PERF_COUNT_HW_CPU_CYCLES,
    PERF_COUNT_HW_INSTRUCTIONS,
    PERF_COUNT_HW_CACHE_REFERENCES,
    PERF_COUNT_HW_CACHE_MISSES,
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    PERF_COUNT_HW_BRANCH_MISSES,
    PERF_COUNT_HW_BUS_CYCLES,
    PERF_COUNT_HW_STALLED_CYCLES_FRONTEND,
    PERF_COUNT_HW_STALLED_CYCLES_BACKEND,
    PERF_COUNT_HW_REF_CPU_CYCLES,
    PERF_COUNT_SW_CPU_CLOCK,
    PERF_COUNT_SW_TASK_CLOCK,
    PERF_COUNT_SW_PAGE_FAULTS,
    PERF_COUNT_SW_CONTEXT_SWITCHES,
    PERF_COUNT_SW_CPU_MIGRATIONS,
    PERF_COUNT_SW_PAGE_FAULTS_MIN,
    PERF_COUNT_SW_PAGE_FAULTS_MAJ,
    PERF_COUNT_SW_ALIGNMENT_FAULTS,
    PERF_COUNT_SW_EMULATION_FAULTS,
    PERF_COUNT_SW_DUMMY
  };

  enum PerfAttrType {
    PERF_TYPE_HARDWARE,
    PERF_TYPE_SOFTWARE,
    PERF_TYPE_TRACEPOINT,
    PERF_TYPE_HW_CACHE,
    PERF_TYPE_RAW,
    PERF_TYPE_BREAKPOINT
  };

  struct PerfEventAttr {
    PerfAttrType type;
    PerfAttrConfig config;
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

  enum PerfMeasureType {
      InstCount,
      CycleCount,
      NPerfMeasureType
    };

  std::unordered_map<int, PerfMeasure> measureMap;

public:
  MPerf();
  void ReadStartValues();
  void ReadStartValue(PerfMeasureType type);

  void ReadEndValues();
  void ReadEndValue(PerfMeasureType type);

  double GetCPI();

  double GetIPC();
};
} // namespace CPI
