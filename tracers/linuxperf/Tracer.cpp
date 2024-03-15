#include <err.h>

#include <MPerf/Tracers/LinuxPerf.hpp>
#include <cstdio>
#include <stdexcept>

#include "AllCPUEvents.hpp"
#include "L1DCacheEvents.hpp"

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

void Measure::PerfEventOpen(uint32_t type, uint64_t config) {
  auto typeString = std::to_string(type);
  auto configString = std::to_string(config);
  PerfEventOpen("unknown" + typeString + configString, type, config);
}

void Measure::PerfEventOpen(std::string label, uint32_t type, uint64_t config) {
  int fd;
  perf_event_attr attr{
      .type = type, .config = config, .read_format = PERF_FORMAT_GROUP};

  if (fds.size() == 0) {
    fd = perf_event_open(&attr, getpid(), -1, -1, 0);
    leader_fd = fd;
  } else {
    fd = perf_event_open(&attr, getpid(), -1, leader_fd, 0);
  }

  // Skip if event not available
  if (fd < 0) {
    std::cerr << "WARNING: " << __FUNCTION__ << " cannot open fd ";
    std::cerr << " for type " << type;
    std::cerr << " config " << config << "\n";
    return;
  }

  labelToResultIndex[label] = fds.size();
  fds.push_back(fd);
}

std::unique_ptr<::MPerf::Measure> Tracer::MakeMeasure(HLMeasureType hlType) {
  using HLType = HLMeasureType;
  std::unique_ptr<::MPerf::Measure> ptr;

  if (hlType == HLMeasureType::ProcCounters) {
    ptr.reset(new AllCPUEvents(hlType));
  } else if (hlType == HLMeasureType::CacheL1D) {
    ptr.reset(new L1DCacheEvents(hlType));
  } else {
    throw std::invalid_argument("Unimplemented HLType");
  }

  return ptr;
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
