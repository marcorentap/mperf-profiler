#include <MPerf/Tracers/LinuxPerf.hpp>

#include "ProcMeasure.hpp"

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

std::unique_ptr<BaseMeasure> Tracer::MakeMeasure(HLMeasureType hlType) {
  using HLType = HLMeasureType;
  std::unique_ptr<BaseMeasure> ptr;

  if (hlType == HLMeasureType::LinuxPerfProc) {
    ptr.reset(new ProcMeasure(hlType));
  } else {
    ptr.reset(new Measure(hlType));
  }
  return ptr;
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
