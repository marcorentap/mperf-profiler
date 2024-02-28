#include <asm/unistd_64.h>
#include <linux/perf_event.h>
#include <unistd.h>

#include <Dummy.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

namespace MPerf {
namespace Subsystem {
namespace Dummy {
void Measure::Init() { std::cout << "Initialize Dummy Measure" << std::endl; }
void Measure::ReadValues() { std::cout << "Dummy Read Values" << std::endl; }
}  // namespace Dummy
}  // namespace Subsystem
}  // namespace MPerf
