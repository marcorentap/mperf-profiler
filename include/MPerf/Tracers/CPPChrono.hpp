#ifndef CPPCHRONO_TRACER_HPP
#define CPPCHRONO_TRACER_HPP

#include <MPerf/Core.hpp>
#include <chrono>

namespace MPerf {
namespace Tracers {
namespace CPPChrono {

using uPtrBMeasure = std::unique_ptr<Tracers::Base::Measure>;
using uPtrBTracer = std::unique_ptr<Tracers::Base::Tracer>;
using HLMType = HLMeasureType;

class Tracer : public ::MPerf::Tracer {
 public:
  Tracer() {}
  uPtrBMeasure MakeMeasure(HLMType hlType) override;
  uPtrBMeasure MakeMeasure(std::vector<HLMType> hlTypes) override;
};

class Measure : public ::MPerf::Measure {
 private:
  using cppclock = std::chrono::steady_clock;
  using time_point = std::chrono::time_point<cppclock, cppclock::duration>;
  using display_unit = std::chrono::nanoseconds;
  time_point result;

 public:
  virtual void DoMeasure();
  virtual json GetJSON();
};

}  // namespace CPPChrono
}  // namespace Tracers
}  // namespace MPerf
#endif
