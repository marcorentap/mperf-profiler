#include "L1DCacheEvents.hpp"

#include <err.h>
#include <linux/perf_event.h>

#include <MPerf/Tracers/LinuxPerf.hpp>
#include <string>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

L1DCacheEvents::L1DCacheEvents(HLMeasureType hlType)
    : Measure(hlType) {
  PerfEventOpen(
      "cache_l1d_read_access", PERF_TYPE_HW_CACHE,
      MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ,
                      PERF_COUNT_HW_CACHE_RESULT_ACCESS));
  PerfEventOpen(
      "cache_l1d_read_miss", PERF_TYPE_HW_CACHE,
      MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_READ,
                      PERF_COUNT_HW_CACHE_RESULT_MISS));
  PerfEventOpen(
      "cache_l1d_write_access", PERF_TYPE_HW_CACHE,
      MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_WRITE,
                      PERF_COUNT_HW_CACHE_RESULT_ACCESS));
  PerfEventOpen(
      "cache_l1d_write_miss", PERF_TYPE_HW_CACHE,
      MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_WRITE,
                      PERF_COUNT_HW_CACHE_RESULT_MISS));
  PerfEventOpen(
      "cache_l1d_prefetch_access", PERF_TYPE_HW_CACHE,
      MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                      PERF_COUNT_HW_CACHE_RESULT_ACCESS));
  PerfEventOpen(
      "cache_l1d_prefetch_miss", PERF_TYPE_HW_CACHE,
      MakeCacheConfig(PERF_COUNT_HW_CACHE_L1D, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                      PERF_COUNT_HW_CACHE_RESULT_MISS));
}

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
