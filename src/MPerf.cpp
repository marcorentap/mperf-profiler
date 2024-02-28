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
  enum measureEntry { SType, MType, MPulse };
  // Initialize all predefined measures
  for (auto& _measure : _measures) {
    auto sType = std::get<SType>(_measure);
    auto mType = std::get<MType>(_measure);
    auto mPulse = std::get<MPulse>(_measure);
    std::shared_ptr<Measure> ptr;

    switch (sType) {
      case SubsystemType::Dummy:
        ptr.reset(new DummyMeasure(mType, mPulse));
        break;
      case SubsystemType::LinuxPerf:
        ptr.reset(new LinuxPerfMeasure(mType, mPulse));
        break;
      case SubsystemType::HWLoc:
        ptr.reset(new HWLocMeasure(mType, mPulse));
        break;
    }
    ptr->Init();
    measures.push_back(ptr);
    measuresByPulse[mPulse].push_back(ptr);
  }
}
std::vector<std::shared_ptr<Measure>>& MPerf::GetMeasuresByPulse(
    MeasurePulse mPulse) {
  return measuresByPulse[mPulse];
}

};  // namespace MPerf
