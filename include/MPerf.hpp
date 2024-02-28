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
enum class SubsystemType { Dummy, LinuxPerf, HWLoc };

class MPerf {
  using LinuxPerfMeasure = Subsystem::LinuxPerf::Measure;
  using HWLocMeasure = Subsystem::HWLoc::Measure;
  using DummyMeasure = Subsystem::Dummy::Measure;
  using MType = MeasureType;
  using MPulse = MeasurePulse;
  using SType = SubsystemType;

 private:
  std::vector<std::tuple<SType, MType, MPulse>> _measures = {
      {SType::Dummy, MType::DummyMeasure, MPulse::InitLibrary},
      {SType::LinuxPerf, MType::LinuxPerfCPI, MPulse::InitLibrary},
      {SType::HWLoc, MType::HWLOCSystemInfo, MPulse::InitLibrary},
  };
  std::vector<std::shared_ptr<Measure>> measures;

 public:
  MPerf();
};
}  // namespace MPerf
#endif
