#ifndef LINUXPERF_L1DCACHEEVENTS_HPP
#define LINUXPERF_L1DCACHEEVENTS_HPP

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class L1DCacheEvents : public Measure {
 public:
  L1DCacheEvents(HLMeasureType hlType);
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
