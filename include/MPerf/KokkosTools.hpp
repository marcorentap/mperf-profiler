#include <memory>
#include <unordered_map>

#include "MPerf/Core.hpp"

namespace MPerf {
namespace KokkosTools {

enum class KokkosPulse {
  InitLibrary,
  FinalizeLibrary,
  WholeLibrary,
  BeginParFor,
  EndParFor,
  WholeParFor,
  BeginParScan,
  EndParScan,
  WholeParScan,
  BeginParReduce,
  EndParReduce,
  WholeParReduce,
  BeginFence,
  EndFence,
  WholeFence,
  PushProfileRegion,
  PopProfileRegion,
  WholeProfileRegion,
  AllocateData,
  DeallocateData,
  WholeData,
  BeginDeepCopy,
  EndDeepCopy,
  WholeDeepCopy,
  All,
};

using BaseMeasure = ::MPerf::BaseMeasure;
using BaseTracer = ::MPerf::BaseTracer;
using HLType = ::MPerf::HLMeasureType;
using KPulse = KokkosPulse;

static auto measures = std::vector<std::shared_ptr<BaseMeasure>>{};
static auto measuresByPulse =
    std::unordered_map<KPulse, std::vector<std::shared_ptr<BaseMeasure>>>{};

inline BaseMeasure &KokkosPAddMeasure(HLType hlType, BaseTracer &tracer,
                                      KPulse pulse) {
  auto sptr = std::shared_ptr<BaseMeasure>();
  sptr = tracer.MakeMeasure(hlType);

  measures.push_back(sptr);
  measuresByPulse[pulse].push_back(sptr);

  // TODO: Refactor this Great Wall of China
  if (pulse == KPulse::All || pulse == KPulse::WholeLibrary) {
    measuresByPulse[KPulse::InitLibrary].push_back(sptr);
    measuresByPulse[KPulse::FinalizeLibrary].push_back(sptr);
  }
  if (pulse == KPulse::All || pulse == KPulse::WholeParFor) {
    measuresByPulse[KPulse::BeginParFor].push_back(sptr);
    measuresByPulse[KPulse::EndParFor].push_back(sptr);
  }
  if (pulse == KPulse::All || pulse == KPulse::WholeParScan) {
    measuresByPulse[KPulse::BeginParScan].push_back(sptr);
    measuresByPulse[KPulse::EndParScan].push_back(sptr);
  }
  if (pulse == KPulse::All || pulse == KPulse::WholeParReduce) {
    measuresByPulse[KPulse::BeginParReduce].push_back(sptr);
    measuresByPulse[KPulse::EndParReduce].push_back(sptr);
  }
  if (pulse == KPulse::All || pulse == KPulse::WholeFence) {
    measuresByPulse[KPulse::BeginFence].push_back(sptr);
    measuresByPulse[KPulse::EndFence].push_back(sptr);
  }
  if (pulse == KPulse::All || pulse == KPulse::WholeProfileRegion) {
    measuresByPulse[KPulse::PushProfileRegion].push_back(sptr);
    measuresByPulse[KPulse::PopProfileRegion].push_back(sptr);
  }
  if (pulse == KPulse::All | pulse == KPulse::WholeData) {
    measuresByPulse[KPulse::AllocateData].push_back(sptr);
    measuresByPulse[KPulse::DeallocateData].push_back(sptr);
  }
  if (pulse == KPulse::All || pulse == KPulse::WholeDeepCopy) {
    measuresByPulse[KPulse::BeginDeepCopy].push_back(sptr);
    measuresByPulse[KPulse::EndDeepCopy].push_back(sptr);
  }

  return *measures.back();
}

inline void PulseDoMeasure(KPulse pulse) {
  // Fast exit
  if (measuresByPulse[pulse].size() == 0) return;
  for (auto &measure : measuresByPulse[pulse]) {
    measure->DoMeasure();
  }
}

inline void KokkosPInitLibrary() { PulseDoMeasure(KPulse::InitLibrary); }

inline void KokkosPFinalizeLibrary() {
  PulseDoMeasure(KPulse::FinalizeLibrary);
}
inline void KokkosPBeginParallelFor() { PulseDoMeasure(KPulse::BeginParFor); }
inline void KokkosPEndParallelFor() { PulseDoMeasure(KPulse::EndParFor); }
inline void KokkosPBeginParallelScan() { PulseDoMeasure(KPulse::BeginParScan); }
inline void KokkosPEndParallelScan() { PulseDoMeasure(KPulse::EndParScan); }
inline void KokkosPBeginParallelReduce() {
  PulseDoMeasure(KPulse::BeginParReduce);
}
inline void KokkosPEndParallelReduce() { PulseDoMeasure(KPulse::EndParReduce); }
inline void KokkosPBeginFence() { PulseDoMeasure(KPulse::BeginFence); }
inline void KokkosPEndFence() { PulseDoMeasure(KPulse::EndFence); }
inline void KokkosPPushProfileRegion() {
  PulseDoMeasure(KPulse::PushProfileRegion);
}
inline void KokkosPPopProfileRegion() {
  PulseDoMeasure(KPulse::PopProfileRegion);
}
inline void KokkosPAllocateData() { PulseDoMeasure(KPulse::AllocateData); }
inline void KokkosPDeallocateData() { PulseDoMeasure(KPulse::DeallocateData); }
inline void KokkosPBeginDeepCopy() { PulseDoMeasure(KPulse::BeginDeepCopy); }
inline void KokkosPEndDeepCopy() { PulseDoMeasure(KPulse::EndDeepCopy); }

}  // namespace KokkosTools
}  // namespace MPerf
