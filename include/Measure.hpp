#ifndef MEASURE_HPP
#define MEASURE_HPP
#include <iostream>
#include <memory>
namespace MPerf {

enum class MeasureType { DummyMeasure, HWLOCSystemInfo, LinuxPerfCPI };
enum class MeasurePulse {
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
  BeginParFence,
  EndParFence,
  WholeParFence,
  PushProfileRegion,
  PopProfileRegion,
  WholeProfileRegion,
  AllocateData,
  DeallocateData,
  WholeData,
  BeginDeepCopy,
  EndDeepCopy,
  WholeDeepCopy,
};

class Measure {
 public:
  MeasureType type;
  MeasurePulse pulse;
  Measure(MeasureType type, MeasurePulse pulse) : type(type), pulse(pulse) {}
  virtual void Init() { std::cout << "Default Measure Init" << std::endl; }
};
}  // namespace MPerf
#endif
