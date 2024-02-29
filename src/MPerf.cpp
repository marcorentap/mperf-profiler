#include <asm/unistd_64.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <Dummy.hpp>
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
    auto hlType = mapEntry.first;
    auto mType = std::get<MType>(measureSetting);
    auto mPulse = std::get<MPulse>(measureSetting);

    std::shared_ptr<Measure> ptr;

    if (mType == MeasureType::LinuxPerf) {
      std::shared_ptr<Measure> measure =
          Subsystem::LinuxPerf::MakeMeasure(hlType, mType, mPulse);
      ptr = measure;
    } else {
          Subsystem::Dummy::MakeMeasure(hlType, mType, mPulse);
    }
    ptr->Init();
    measures[hlType] = ptr;
    measuresByPulse[mPulse].push_back(ptr);
  }
}
std::vector<std::shared_ptr<Measure>>& MPerf::PulseMeasures(
    MeasurePulse mPulse) {
  return measuresByPulse[mPulse];
}
void MPerf::PulseDoMeasure(MPulse mPulse) {
  for (auto& measure : PulseMeasures(mPulse)) {
    measure->DoMeasure();
  }
}

void MPerf::PulseDoNextMeasure(MPulse mPulse) {
  for (auto& measure : PulseMeasures(mPulse)) {
    measure->DoNextMeasure();
  }
}

std::shared_ptr<Measure> MPerf::GetMeasure(HLMeasureType HLType) {
  return measures.at(HLType);
}

};  // namespace MPerf
