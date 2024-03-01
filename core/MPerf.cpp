#include "MPerf.hpp"

#include <memory>

namespace MPerf {
MPerf::MPerf() {}

void MPerf::AddMeasure(BaseTracer& tracer, HLMeasureType hlType,
                       MPulse mPulse) {
  for (auto& measure : __measures) {
    auto sptr = std::shared_ptr<BaseMeasure>();
    auto ptr = tracer.MakeMeasure(hlType, mPulse);
    sptr = std::move(ptr);
    measuresByPulse[mPulse].push_back(sptr);
  }
}

std::vector<std::shared_ptr<BaseMeasure>>& MPerf::PulseMeasures(
    MeasurePulse mPulse) {
  return measuresByPulse[mPulse];
}

void MPerf::PulseDoMeasure(MPulse mPulse) {
  for (auto& measure : PulseMeasures(mPulse)) {
    measure->DoMeasure();
  }
}
};  // namespace MPerf
