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
  Measure(MeasureType type, MeasurePulse pulse)
      : ::MPerf::Measure(type, pulse) {}

  void Init();
  void DoMeasure();
  void DoNextMeasure();
};

}  // namespace HWLoc

}  // namespace Subsystem
}  // namespace MPerf

#endif
