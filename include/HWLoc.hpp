#ifndef HWLOC_HPP
#define HWLOC_HPP
#include <Measure.hpp>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace MPerf {
namespace Subsystem {
namespace HWLoc {

class Measure : public ::MPerf::Measure {
 private:
  int nproc;

 public:
  Measure(HLMeasureType hlType, MeasureType type, MeasurePulse pulse)
      : ::MPerf::Measure(hlType, type, pulse) {}

  void Init();
  void DoMeasure();
  void DoNextMeasure();
  json GetJSON() {
    json j;
    j["name"] = "HWLoc Measure";
    return j;
  }
};

std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType, MeasureType type, MeasurePulse pulse);
}  // namespace HWLoc

}  // namespace Subsystem
}  // namespace MPerf

#endif
