#ifndef LINUXPERF_ALLCPUEVENTS_HPP
#define LINUXPERF_ALLCPUEVENTS_HPP

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class AllCPUEvents : public Measure {
 public:
  AllCPUEvents(HLMeasureType hlType);
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
