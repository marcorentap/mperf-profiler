#ifndef LINUXPERF_ALLCPUEVENTS_HPP
#define LINUXPERF_ALLCPUEVENTS_HPP

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class CPUEvents : public Measure {
 public:
  CPUEvents(HLMeasureType hlType) : Measure::Measure(hlType) {
    PerfEventOpen("hw_cpu_cycles", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_CPU_CYCLES);
    PerfEventOpen("hw_instructions", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_INSTRUCTIONS);
    PerfEventOpen("hw_cache_references", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_CACHE_REFERENCES);
    PerfEventOpen("hw_cache_misses", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_CACHE_MISSES);
    PerfEventOpen("hw_branch_instructions", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_BRANCH_INSTRUCTIONS);
    PerfEventOpen("hw_stalled_cycles_frontend", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_STALLED_CYCLES_FRONTEND);
    PerfEventOpen("hw_stalled_cycles_backend", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_STALLED_CYCLES_BACKEND);
    PerfEventOpen("hw_ref_cpu_cycles", PERF_TYPE_HARDWARE,
                  PERF_COUNT_HW_REF_CPU_CYCLES);
    // TODO: When these are enabled, measurement results from read() are always
    // the same PerfEventOpen("hw_branch_misses", PERF_TYPE_HARDWARE,
    // PERF_COUNT_HW_BRANCH_MISSES); PerfEventOpen("hw_bus_cycles",
    // PERF_TYPE_HARDWARE, PERF_COUNT_HW_BUS_CYCLES);
  }
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
