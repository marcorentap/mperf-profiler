#include "L1DCacheEvents.hpp"

#include <err.h>
#include <linux/perf_event.h>

#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

void L1DCacheEvents::PerfEventOpen(uint32_t type, uint64_t config) {
  int fd;
  perf_event_attr attr{
      .type = type, .config = config, .read_format = PERF_FORMAT_GROUP};

  if (fds.size() == 0) {
    fd = perf_event_open(&attr, getpid(), -1, -1, 0);
    leader_fd = fd;
  } else {
    fd = perf_event_open(&attr, getpid(), -1, leader_fd, 0);
  }

  if (fd < 0) {
    err(EXIT_FAILURE, "cannot open fd for type %u config %lu", type, config);
  }

  fds.push_back(fd);
}

L1DCacheEvents::L1DCacheEvents(HLMeasureType hlType) : Measure(hlType) {
  PerfEventOpen(PERF_TYPE_HW_CACHE, MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_ACCESS));
  PerfEventOpen(PERF_TYPE_HW_CACHE, MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ, PERF_COUNT_HW_CACHE_RESULT_MISS));
  PerfEventOpen(PERF_TYPE_HW_CACHE, MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_WRITE, PERF_COUNT_HW_CACHE_RESULT_ACCESS));
  // PerfEventOpen(PERF_TYPE_HW_CACHE, MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_WRITE, PERF_COUNT_HW_CACHE_RESULT_MISS));
  // PerfEventOpen(PERF_TYPE_HW_CACHE, MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_PREFETCH, PERF_COUNT_HW_CACHE_RESULT_ACCESS));
  // PerfEventOpen(PERF_TYPE_HW_CACHE, MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_PREFETCH, PERF_COUNT_HW_CACHE_RESULT_MISS));
}

void L1DCacheEvents::DoMeasure() {
 memset(&result, 0, sizeof(result));
  int ret = read(leader_fd, &result, sizeof(result));
  if (ret < 0) {
    exit(EXIT_FAILURE);
  }
}

json L1DCacheEvents::GetJSON() {
  json j;
  j["measure_name"] = "Linux Perf L1D Cache";
  j["read_access"] = result.read_access;
  j["read_miss"] = result.read_miss;
  j["write_access"] = result.write_access;
  // j["write_miss"] = result.write_miss;
  j["prefetch_access"] = result.prefetch_access;
  j["prefetch_miss"] = result.prefetch_miss;
  return j;
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
