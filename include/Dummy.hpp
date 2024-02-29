#ifndef DUMMY_HPP
#define DUMMY_HPP
#include <Measure.hpp>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

namespace MPerf {
namespace Subsystem {
namespace Dummy {

class Measure : public ::MPerf::Measure {
 private:
  int nproc;

 public:
  Measure(HLMeasureType hlType, MeasureType type, MeasurePulse pulse)
      : ::MPerf::Measure(hlType, type, pulse) {}
  void Init();
  void DoMeasure();
  void DoNextMeasure();
};

std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType, MeasureType type, MeasurePulse pulse);
}  // namespace Dummy

}  // namespace Subsystem
}  // namespace MPerf

#endif
