#include <asm/unistd_64.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <LinuxPerf.hpp>
#include <MPerf.hpp>
#include <Measure.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

namespace MPerf {
MPerf::MPerf() {
  enum measureEntry { MType, MPulse };
  // Initialize all predefined measures
  for (auto& mapEntry : _measures) {
    auto measureSetting = mapEntry.second;
    auto mType = std::get<MType>(measureSetting);
    auto mPulse = std::get<MPulse>(measureSetting);

    std::shared_ptr<Measure> ptr;
    switch (mType) {
      case MeasureType::Dummy:
        ptr.reset(new DummyMeasure(mType, mPulse));
        break;
      case MeasureType::LinuxPerf:
        ptr.reset(new LinuxPerfMeasure(mType, mPulse));
        break;
      case MeasureType::HWLoc:
        ptr.reset(new HWLocMeasure(mType, mPulse));
        break;
    }
    ptr->Init();
    measures.push_back(ptr);
    measuresByPulse[mPulse].push_back(ptr);
  }
}
std::vector<std::shared_ptr<Measure>>& MPerf::PulseMeasures(
    MeasurePulse mPulse) {
  return measuresByPulse[mPulse];
}
void MPerf::PulseDoMeasure(MPulse mPulse) {
    for (auto &measure : PulseMeasures(mPulse)) {
      measure->DoMeasure();
    }
}

void MPerf::PulseDoNextMeasure(MPulse mPulse) {
    for (auto &measure : PulseMeasures(mPulse)) {
      measure->DoNextMeasure();
    }
}

};  // namespace MPerf
