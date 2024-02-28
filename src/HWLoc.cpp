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
void Measure::ReadValues() { std::cout << "HWLoc Read Values" << std::endl; }
}  // namespace HWLoc
}  // namespace Subsystem
}  // namespace MPerf
