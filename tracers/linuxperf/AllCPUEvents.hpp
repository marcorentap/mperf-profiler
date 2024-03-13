#ifndef LINUXPERF_ALLCPUEVENTS_HPP
#define LINUXPERF_ALLCPUEVENTS_HPP

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class AllCPUEvents : public Measure {
 private:
  void PerfEventOpen(uint32_t type, uint64_t config);
  struct read_format {
    uint64_t nr;
    uint64_t cycles;
    uint64_t insts;
    uint64_t branchInsts;
    uint64_t branchMisses;
    uint64_t busCycles;
    // These are not available :(
    // uint64_t stalledCycleFront;
    // uint64_t stalledCyclesBack;
  } result;
  int leader_fd;

 public:
  AllCPUEvents(HLMeasureType hlType);
  void DoMeasure();
  json GetJSON();
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
