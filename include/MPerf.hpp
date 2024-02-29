#ifndef MPERF_HPP
#define MPERF_HPP
#include <linux/perf_event.h>

#include <Dummy.hpp>
#include <HWLoc.hpp>
#include <LinuxPerf.hpp>
#include <Measure.hpp>
#include <cstdint>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace MPerf {

class MPerf {
  using LinuxPerfMeasure = Subsystem::LinuxPerf::Measure;
  using HWLocMeasure = Subsystem::HWLoc::Measure;
  using DummyMeasure = Subsystem::Dummy::Measure;
  using HLType = HLMeasureType;
  using MType = MeasureType;
  using MPulse = MeasurePulse;

 private:
  std::unordered_map<HLMeasureType, std::tuple<MType, MPulse>> _measures = {
      // {HLType::InitDummy, {MType::Dummy, MPulse::InitLibrary}},
      {HLType::LinuxPerfProc, {MType::LinuxPerf, MPulse::WholeProfileRegion}},
      // {HLType::LibraryCPI, {MType::LinuxPerf, MPulse::WholeLibrary}},
      // {HLType::SystemInfo, {MType::HWLoc, MPulse::InitLibrary}},
  };
  std::unordered_map<HLType, std::shared_ptr<Measure>> measures;
  std::unordered_map<MPulse, std::vector<std::shared_ptr<Measure>>>
      measuresByPulse;

 public:
  MPerf();
  std::vector<std::shared_ptr<Measure>>& PulseMeasures(MPulse mPulse);
  void PulseDoMeasure(MPulse mPulse);
  void PulseDoNextMeasure(MPulse mPulse);
  std::shared_ptr<Measure> GetMeasure(HLMeasureType HLType);
};
}  // namespace MPerf
#endif
