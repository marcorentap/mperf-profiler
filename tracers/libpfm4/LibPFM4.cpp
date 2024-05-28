#include <err.h>
#include <perfmon/pfmlib.h>
#include <perfmon/pfmlib_perf_event.h>

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LibPFM4.hpp>
#include <cstring>

// TODO: Include based on CPU architecture
#include "IntelSkylake.hpp"

namespace MPerf {
namespace Tracers {
namespace LibPFM4 {
using uPtrBMeasure = std::unique_ptr<Tracers::Base::Measure>;
using uPtrBTracer = std::unique_ptr<Tracers::Base::Tracer>;
using HLMType = HLMeasureType;

Tracer::Tracer() {
  int ret;
  ret = pfm_initialize();
  if (ret != PFM_SUCCESS) {
    errx(EXIT_FAILURE, "cannot initialize. %s", pfm_strerror(ret));
  }
}

uPtrBMeasure Tracer::MakeMeasure(std::vector<HLMType> hlTypes) {
  return MakeMeasure(hlTypes.at(0));
}

uPtrBMeasure Tracer::MakeMeasure(HLMType hlType) {
  using uPtrPfmMeasure = std::unique_ptr<Tracers::LibPFM4::Measure>;
  uPtrPfmMeasure ptr;
  ptr.reset(new Measure());
  ptr->Open(hlType);
  return ptr;
}

Measure::Measure() {}

void Measure::Open(HLMType hlType) {
  this->hlType = hlType;
  int ret;
  auto eventName = hlToEventName.at(hlType);

  std::memset(&attr, 0, sizeof(attr));
  std::memset(&args, 0, sizeof(args));
  args.attr = &attr;
  args.size = sizeof(pfm_perf_encode_arg_t);
  args.fstr = &fstr;

  // Get attr for perf_event_open
  ret = pfm_get_os_event_encoding(eventName.c_str(), PFM_PLM3 | PFM_PLM0,
                                  PFM_OS_PERF_EVENT, &args);
  if (ret != PFM_SUCCESS) {
    errx(EXIT_FAILURE, "cannot get event encoding. %s", pfm_strerror(ret));
  }

  // Open fd
  attr.exclude_kernel = 1,
  attr.exclude_hv = 1,
  attr.exclude_idle = 1,
  fd = perf_event_open(&attr, 0, -1, -1, 0);
  if (fd < 0) {
    err(EXIT_FAILURE, "cannot open perf event");
  }
}

void Measure::DoMeasure() {
  int ret = read(fd, &result, sizeof(result));
  if (ret < 0) {
    err(EXIT_FAILURE, "mperf libpfm4 cannot read fd");
  }
}

json Measure::GetJSON() {
  json j;
  auto label = HLTypeLabels.at(hlType);
  j[label] = result.value;
  return j;
}

}  // namespace LibPFM4
}  // namespace Tracers
}  // namespace MPerf
