#ifndef LINUXPERF_L1DCACHEEVENTS_HPP
#define LINUXPERF_L1DCACHEEVENTS_HPP

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class L1DCacheEvents: public Measure {
 private:
  void PerfEventOpen(uint32_t type, uint64_t config);
  struct read_format {
     uint64_t read_access;
     uint64_t read_miss;
     uint64_t write_access;
     // uint64_t write_miss;
     uint64_t prefetch_access;
     uint64_t prefetch_miss;
  } result;

 public:
  L1DCacheEvents(HLMeasureType hlType);
  void DoMeasure();
  json GetJSON();
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
