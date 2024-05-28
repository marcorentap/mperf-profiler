#ifndef LIBPFM4_TRACER_HPP
#define LIBPFM4_TRACER_HPP

#include <perfmon/pfmlib_perf_event.h>

#include <MPerf/Core.hpp>

namespace MPerf {
namespace Tracers {
namespace LibPFM4 {

using uPtrBMeasure = std::unique_ptr<Tracers::Base::Measure>;
using uPtrBTracer = std::unique_ptr<Tracers::Base::Tracer>;
using HLMType = HLMeasureType;

class Tracer : public ::MPerf::Tracer {
 public:
  Tracer();
  uPtrBMeasure MakeMeasure(HLMType hlType) override;
  uPtrBMeasure MakeMeasure(std::vector<HLMType> hlTypes) override;
};

class Measure : public ::MPerf::Measure {
 private:
  struct ReadFormat {
    uint64_t value;        /* The value of the event */
    uint64_t time_enabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    uint64_t time_running; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    uint64_t id;           /* if PERF_FORMAT_ID */
    uint64_t lost;         /* if PERF_FORMAT_LOST */
  } result;
  perf_event_attr attr;
  pfm_perf_encode_arg_t args;
  char *fstr;
  int fd;

 public:
  Measure();
  void Open(HLMType hlType);
  virtual void DoMeasure();
  virtual json GetJSON();
};

}  // namespace LibPFM4
}  // namespace Tracers
}  // namespace MPerf
#endif
