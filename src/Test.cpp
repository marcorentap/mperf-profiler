#include <Dummy.hpp>
#include <MPerf.hpp>
#include <Measure.hpp>
#include <iostream>

#include "LinuxPerf.hpp"

template <typename... Ts>
int func(Ts... args) {
  std::cout << "func" << std::endl;
  return 0;
}

using LinuxPerfMeasure = MPerf::Subsystem::LinuxPerf::Measure;
using HWLocMeasure = MPerf::Subsystem::HWLoc::Measure;
using DummyMeasure = MPerf::Subsystem::Dummy::Measure;
using HLType = MPerf::HLMeasureType;
using MType = MPerf::MeasureType;
using MPulse = MPerf::MeasurePulse;

int main(int argc, char *argv[]) {
  auto measure = MPerf::Subsystem::LinuxPerf::MakeMeasure(
      HLType::LinuxPerfProc, MType::LinuxPerf, MPulse::WholeProfileRegion);
  measure->Init();
  measure->DoMeasure();
  std::cout << measure->GetJSON() << "\n";
  for (int i = 0; i < 1000; i++) {
    printf("X\n");
  }
  measure->DoMeasure();
  std::cout << measure->GetJSON() << "\n";

  std::cout << std::flush;
  return 0;
}
