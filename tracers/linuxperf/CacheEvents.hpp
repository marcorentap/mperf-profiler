#ifndef LINUXPERF_CACHEEVENTS_HPP
#define LINUXPERF_CACHEEVENTS_HPP

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class CacheEvents : public Measure {
 public:
  CacheEvents(std::string name, uint64_t id) {
    PerfEventOpen(name + "_read_access", PERF_TYPE_HW_CACHE,
                  MakeCacheConfig(id, PERF_COUNT_HW_CACHE_OP_READ,
                                  PERF_COUNT_HW_CACHE_RESULT_ACCESS));
    PerfEventOpen(name + "_read_miss", PERF_TYPE_HW_CACHE,
                  MakeCacheConfig(id, PERF_COUNT_HW_CACHE_OP_READ,
                                  PERF_COUNT_HW_CACHE_RESULT_MISS));
    PerfEventOpen(name + "_write_access", PERF_TYPE_HW_CACHE,
                  MakeCacheConfig(id, PERF_COUNT_HW_CACHE_OP_WRITE,
                                  PERF_COUNT_HW_CACHE_RESULT_ACCESS));
    PerfEventOpen(name + "_write_miss", PERF_TYPE_HW_CACHE,
                  MakeCacheConfig(id, PERF_COUNT_HW_CACHE_OP_WRITE,
                                  PERF_COUNT_HW_CACHE_RESULT_MISS));
    PerfEventOpen(name + "_prefetch_access", PERF_TYPE_HW_CACHE,
                  MakeCacheConfig(id, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                                  PERF_COUNT_HW_CACHE_RESULT_ACCESS));
    PerfEventOpen(name + "_prefetch_miss", PERF_TYPE_HW_CACHE,
                  MakeCacheConfig(id, PERF_COUNT_HW_CACHE_OP_PREFETCH,
                                  PERF_COUNT_HW_CACHE_RESULT_MISS));
  }
};

class L1DCacheEvents : public CacheEvents {
 public:
  L1DCacheEvents() : CacheEvents("l1d", PERF_COUNT_HW_CACHE_L1D) {}
};

class L1ICacheEvents : public CacheEvents {
 public:
  L1ICacheEvents() : CacheEvents("l1i", PERF_COUNT_HW_CACHE_L1I) {}
};

class LLCacheEvents : public CacheEvents {
 public:
  LLCacheEvents() : CacheEvents("ll", PERF_COUNT_HW_CACHE_LL) {}
};

class DTLBCacheEvents : public CacheEvents {
 public:
  DTLBCacheEvents() : CacheEvents("dtlb", PERF_COUNT_HW_CACHE_DTLB) {}
};

class ITLBCacheEvents : public CacheEvents {
 public:
  ITLBCacheEvents() : CacheEvents("itlb", PERF_COUNT_HW_CACHE_ITLB) {}
};

class BPUCacheEvents : public CacheEvents {
 public:
  BPUCacheEvents() : CacheEvents("bpu", PERF_COUNT_HW_CACHE_BPU) {}
};

class NodeCacheEvents : public CacheEvents {
 public:
  NodeCacheEvents() : CacheEvents("node", PERF_COUNT_HW_CACHE_NODE) {}
};

}  // namespace LinuxPerf
}  // namespace Tracers
}  // namespace MPerf
#endif
