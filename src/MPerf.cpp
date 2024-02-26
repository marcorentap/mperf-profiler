#include <MPerf.hpp>
#include <asm/unistd_64.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <linux/perf_event.h>
#include <type_traits>
#include <unistd.h>
#include <vector>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
  return ret;
}

namespace MPerf {
MPerf::MPerf() {
  // For now, only measure CPI and IPC of all core
  std::vector<PerfEventAttr> attributes(PerfMeasureType::NPerfMeasureType);
  attributes[PerfMeasureType::InstCount] = {
      PerfAttrType::PERF_TYPE_HARDWARE,
      PerfAttrConfig::PERF_COUNT_HW_INSTRUCTIONS};
  attributes[PerfMeasureType::CycleCount] = {
      PerfAttrType::PERF_TYPE_HARDWARE,
      PerfAttrConfig::PERF_COUNT_HW_CPU_CYCLES};

  // Open events
  for (int i = 0; i < PerfMeasureType::NPerfMeasureType; i++) {
    struct PerfMeasure measure;
    struct perf_event_attr attr;
    memset(&attr, 0, sizeof(attr));
    memset(&measure, 0, sizeof(measure));

    auto attribute = attributes[i];
    measure.attribute = attribute;

    attr.type = attribute.type;
    attr.config = attribute.config;

    measure.fd = perf_event_open(&attr, getpid(), -1, -1, 0);
    if (measure.fd < 0) {
      perror("cannot open fd for instruction count");
      exit(EXIT_FAILURE);
    }

    measureMap[i] = measure;
  }
}

// Read start value of all measures
void MPerf::ReadStartValues() {
  for (auto &entry : measureMap) {
    auto &measure = entry.second;
    int ret = read(measure.fd, &measure.readStart, sizeof(measure.readStart));
    if (ret < 0) {
      std::cerr << "cannot read starting value of measure " << entry.first
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

// Read start value of a measure
void MPerf::ReadStartValue(PerfMeasureType type) {
  auto &measure = measureMap[type];
  int ret = read(measure.fd, &measure.readStart, sizeof(measure.readStart));
  if (ret < 0) {
    std::cerr << "cannot read starting value of measure " << type << std::endl;
    exit(EXIT_FAILURE);
  }
}

// Read end value of all measures
void MPerf::ReadEndValues() {
  for (auto &entry : measureMap) {
    auto &measure = entry.second;
    int ret = read(measure.fd, &measure.readEnd, sizeof(measure.readEnd));
    if (ret < 0) {
      std::cerr << "cannot read ending value of measure " << entry.first
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

// Read end value of one measure
void MPerf::ReadEndValue(PerfMeasureType type) {
  auto &measure = measureMap[type];
  int ret = read(measure.fd, &measure.readEnd, sizeof(measure.readEnd));
  if (ret < 0) {
    std::cerr << "cannot read ending value of measure " << type << std::endl;
    exit(EXIT_FAILURE);
  }
}

double MPerf::GetCPI() {
  auto instMeasure = measureMap[PerfMeasureType::InstCount];
  auto cycleMeasure = measureMap[PerfMeasureType::CycleCount];

  auto instStart = instMeasure.readStart.value;
  auto instEnd = instMeasure.readEnd.value;
  auto cycleStart = cycleMeasure.readStart.value;
  auto cycleEnd = cycleMeasure.readEnd.value;

  auto instDiff = instEnd - instStart;
  auto cycleDiff = cycleEnd - cycleStart;
  return (double)cycleDiff / instDiff;
}

double MPerf::GetIPC() {

  auto instMeasure = measureMap[PerfMeasureType::InstCount];
  auto cycleMeasure = measureMap[PerfMeasureType::CycleCount];

  auto instStart = instMeasure.readStart.value;
  auto instEnd = instMeasure.readEnd.value;
  auto cycleStart = cycleMeasure.readStart.value;
  auto cycleEnd = cycleMeasure.readEnd.value;

  auto instDiff = instEnd - instStart;
  auto cycleDiff = cycleEnd - cycleStart;
  return (double)instDiff / cycleDiff;
}

}; // namespace MPerf
