#include <MPerf/Tracers/LinuxPerf.hpp>
#include <stdexcept>

#include "AllCPUEvents.hpp"

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

std::unique_ptr<BaseMeasure> Tracer::MakeMeasure(HLMeasureType hlType) {
  using HLType = HLMeasureType;
  std::unique_ptr<BaseMeasure> ptr;

  if (hlType == HLMeasureType::ProcCounters) {
    ptr.reset(new AllCPUEvents(hlType));
  } else {
    throw std::invalid_argument("Unimplemented HLType");
  }

  return ptr;
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
