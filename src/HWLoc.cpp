#include <asm/unistd_64.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <HWLoc.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

namespace MPerf {
namespace Subsystem {
namespace HWLoc {
void Measure::Init() { std::cout << "Initialize HWLoc Measure" << std::endl; }
void Measure::DoMeasure() { std::cout << "HWLoc Read Value" << std::endl; }
void Measure::DoNextMeasure() {
  std::cout << "HWLoc Read Next Value" << std::endl;
}
std::unique_ptr<Measure> MakeMeasure(HLMeasureType hlType, MeasureType type,
                                     MeasurePulse pulse) {
  return std::unique_ptr<Measure>(new Measure(hlType, type, pulse));
}
}  // namespace HWLoc
}  // namespace Subsystem
}  // namespace MPerf
