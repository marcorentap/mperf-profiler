#include <MPerf/Core.hpp>
#include <vector>
namespace MPerf {
namespace Tracers {
namespace LibPFM4 {
using uPtrBMeasure = std::unique_ptr<Tracers::Base::Measure>;
using uPtrBTracer = std::unique_ptr<Tracers::Base::Tracer>;
using HLMType = HLMeasureType;

std::unordered_map<HLMType, std::string> hlToEventName = {
    {HLMType::HWInstructions, "INST_RETIRED"},
    {HLMType::HWCycleStallsTotal, "CYCLE_ACTIVITY.STALLS_TOTAL"},
    {HLMeasureType::HWCycleStallsL1DMiss, "CYCLE_ACTIVITY.STALLS_L1D_MISS"},
    {HLMeasureType::HWCycleStallsL2Miss, "CYCLE_ACTIVITY.STALLS_L2_MISS"},
    {HLMeasureType::HWCycleStallsL3Miss, "CYCLE_ACTIVITY.STALLS_L3_MISS"},
    {HLMeasureType::HWCycleStallsMemAny, "CYCLE_ACTIVITY.STALLS_MEM_ANY"},
};

}  // namespace LibPFM4
}  // namespace Tracers
}  // namespace MPerf
