#ifndef PROCMEASURE_HPP
#define PROCMEASURE_HPP

#include <MPerf.hpp>

#include "Tracer.hpp"

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
  ProcMeasure(HLMeasureType hlType, MeasurePulse pulse)
      : Measure(hlType, pulse) {}
  void Init();
  void DoMeasure();
  json GetJSON();
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
