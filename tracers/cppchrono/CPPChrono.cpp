#include <err.h>

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/CPPChrono.hpp>

namespace MPerf {
namespace Tracers {
namespace CPPChrono {

uPtrBMeasure Tracer::MakeMeasure(std::vector<HLMType> hlTypes) {
  // Right now, only supports Time
  if (hlTypes.size() > 1 || hlTypes.at(0) != HLMType::Time) {
    errx(EXIT_FAILURE, "CPPChrono only support measure '%s'",
         HLTypeLabels[HLMType::Time].c_str());
  }
  return MakeMeasure(hlTypes.at(0));
}

uPtrBMeasure Tracer::MakeMeasure(HLMType hlType) {
  if (hlType != HLMType::Time) {
    errx(EXIT_FAILURE, "CPPChrono only support measure '%s'",
         HLTypeLabels[HLMType::Time].c_str());
  }

  uPtrBMeasure ptr;
  ptr.reset(new Measure());
  return ptr;
}

void Measure::DoMeasure() { result = cppclock::now(); }

json Measure::GetJSON() {
  json j;
  auto time = result.time_since_epoch().count();
  auto label = HLTypeLabels.at(HLMeasureType::Time);
  j[label] = time;
  return j;
}

}  // namespace CPPChrono
}  // namespace Tracers
}  // namespace MPerf
