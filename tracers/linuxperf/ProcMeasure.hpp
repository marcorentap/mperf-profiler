#ifndef LINUXPERF_PROCMEASURE_HPP
#define LINUXPERF_PROCMEASURE_HPP

#include <MPerf/Core.hpp>

#include "MPerf/Tracers/LinuxPerf.hpp"

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

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
  ProcMeasure(HLMeasureType hlType);
  void DoMeasure();
  json GetJSON();
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
