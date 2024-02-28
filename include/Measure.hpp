#ifndef MEASURE_HPP
#define MEASURE_HPP
#include <iostream>
#include <memory>
namespace MPerf {

enum class HLMeasureType {InitDummy, RegionCPI, LibraryCPI, SystemInfo};
enum class MeasureType { Dummy, HWLoc, LinuxPerf };
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
  virtual void DoMeasure() { std::cout << "Default ReadValue" << std::endl;}
  virtual void DoNextMeasure() { std::cout << "Default ReadNextValue" << std::endl;}
  virtual void WriteResult(std::shared_ptr<void> dest, size_t len) { std::cout << "Default WriteResult" << std::endl;};
};
}  // namespace MPerf
#endif
