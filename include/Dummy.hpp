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
  Measure(MeasureType type, MeasurePulse pulse)
      : ::MPerf::Measure(type, pulse) {}
  void Init();
};

}  // namespace Dummy

}  // namespace Subsystem
}  // namespace MPerf

#endif
