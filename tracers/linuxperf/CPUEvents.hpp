#ifndef LINUXPERF_ALLCPUEVENTS_HPP
#define LINUXPERF_ALLCPUEVENTS_HPP

#include <linux/perf_event.h>

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>
#include <stdexcept>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class CPUEvents : public Measure {
 private:
  std::vector<std::string> event_names = {
      "hw_cpu_cycles",
      "hw_instructions",
      "hw_cache_references",
      "hw_cache_misses",
      "hw_branch_instructions",
      "hw_branch_misses",
      "hw_bus_cycles",
      "hw_stalled_cycles_frontend",
      "hw_stalled_cycles_backend",
      "hw_ref_cpu_cycles",
  };
  std::vector<std::string> config_names = {
      "PERF_COUNT_HW_CPU_CYCLES",
      "PERF_COUNT_HW_INSTRUCTIONS",
      "PERF_COUNT_HW_CACHE_REFERENCES",
      "PERF_COUNT_HW_CACHE_MISSES",
      "PERF_COUNT_HW_BRANCH_INSTRUCTIONS",
      "PERF_COUNT_HW_BRANCH_MISSES",
      "PERF_COUNT_HW_BUS_CYCLES",
      "PERF_COUNT_HW_STALLED_CYCLES_FRONTEND",
      "PERF_COUNT_HW_STALLED_CYCLES_BACKEND",
      "PERF_COUNT_HW_REF_CPU_CYCLES",
  };

 public:
  CPUEvents() {
    for (auto config = 0; config < PERF_COUNT_HW_MAX; config++) {
      if (config == PERF_COUNT_HW_CACHE_REFERENCES ||
          config == PERF_COUNT_HW_CACHE_MISSES)
        continue;

      try {
        PerfEventOpen(event_names[config], PERF_TYPE_HARDWARE, config);
      } catch (std::invalid_argument e) {
        std::cerr << "Cannot open CPUEvent " << event_names[config] << "\n";
      }
    }
  }
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
