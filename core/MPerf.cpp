#include <memory>

#include "MPerf/Core.hpp"

namespace MPerf {
MPerf::MPerf() {}

std::shared_ptr<BaseMeasure> MPerf::AddMeasure(BaseTracer& tracer, HLMeasureType hlType,
                       MeasurePulse mPulse) {
  auto sptr = std::shared_ptr<BaseMeasure>();
  auto ptr = tracer.MakeMeasure(hlType, mPulse);
  sptr = std::move(ptr);
  measuresByPulse[mPulse].push_back(sptr);
  return sptr;
}

std::vector<std::shared_ptr<BaseMeasure>>& MPerf::PulseMeasures(
    MeasurePulse mPulse) {
  return measuresByPulse[mPulse];
}

void MPerf::PulseDoMeasure(MeasurePulse mPulse) {
  for (auto& measure : PulseMeasures(mPulse)) {
    measure->DoMeasure();
  }
}
};  // namespace MPerf
