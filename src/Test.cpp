#include <MPerf.hpp>
#include <Dummy.hpp>
#include <Measure.hpp>


int main(int argc, char *argv[]) {
  using LinuxPerfMeasure = MPerf::Subsystem::LinuxPerf::Measure;
  using HWLocMeasure = MPerf::Subsystem::HWLoc::Measure;
  using DummyMeasure = MPerf::Subsystem::Dummy::Measure;
  using Measure = MPerf::Measure;
  using MType = MPerf::MeasureType;
  using MPulse = MPerf::MeasurePulse;
  using SType = MPerf::SubsystemType;
  auto mperf = MPerf::MPerf();

  auto wholeRegionMeasures = mperf.GetMeasuresByPulse(MPulse::WholeProfileRegion);
  auto initLibraryMeasures = mperf.GetMeasuresByPulse(MPulse::InitLibrary);

  std::cout << "Whole Region Measures" << std::endl;
  for (auto &region : wholeRegionMeasures) {
    region->Init();
  }
  std::cout << "Init Library Measures" << std::endl;
  for (auto &region : initLibraryMeasures) {
    region->Init();
  }
  return 0;
}
