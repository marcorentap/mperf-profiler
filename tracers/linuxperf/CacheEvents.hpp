#ifndef LINUXPERF_CACHEEVENTS_HPP
#define LINUXPERF_CACHEEVENTS_HPP

#include <linux/perf_event.h>

#include <MPerf/Core.hpp>
#include <MPerf/Tracers/LinuxPerf.hpp>
#include <stdexcept>

namespace MPerf {
namespace Tracers {
namespace LinuxPerf {

class CacheEvents : public Measure {
 private:
  std::vector<std::string> op_names = {"read", "write", "prefetch"};
  std::vector<std::string> op_result_names = {"access", "miss"};

 public:
  CacheEvents(std::string cache_name, uint64_t cache_id) {
    uint64_t max_id = PERF_COUNT_HW_CACHE_OP_MAX;
    uint64_t max_result_id = PERF_COUNT_HW_CACHE_RESULT_MAX;
    for (auto op_id = 0; op_id < 2; op_id++) {
      for (auto op_result_id = 0; op_result_id < 2;
           op_result_id++) {
        auto op_name = op_names[op_id];
        auto op_result_name = op_result_names[op_result_id];
        auto event_name = cache_name + "_" + op_name + "_" + op_result_name;
        uint64_t config = MakeCacheConfig(cache_id, op_id, op_result_id);
        try {
          PerfEventOpen(event_name, PERF_TYPE_HW_CACHE, config);

        } catch (std::invalid_argument e) {
          std::cerr << "cannot open cache event " << event_name << "\n";
        }
      }
    }
    // PerfEventOpen(cache_name + "_read_access", PERF_TYPE_HW_CACHE,
    //               MakeCacheConfig(cache_id, PERF_COUNT_HW_CACHE_OP_READ,
    //                               PERF_COUNT_HW_CACHE_RESULT_ACCESS));
    // PerfEventOpen(cache_name + "_read_miss", PERF_TYPE_HW_CACHE,
    //               MakeCacheConfig(cache_id, PERF_COUNT_HW_CACHE_OP_READ,
    //                               PERF_COUNT_HW_CACHE_RESULT_MISS));
    // PerfEventOpen(cache_name + "_write_access", PERF_TYPE_HW_CACHE,
    //               MakeCacheConfig(cache_id, PERF_COUNT_HW_CACHE_OP_WRITE,
    //                               PERF_COUNT_HW_CACHE_RESULT_ACCESS));
    // PerfEventOpen(cache_name + "_write_miss", PERF_TYPE_HW_CACHE,
    //               MakeCacheConfig(cache_id, PERF_COUNT_HW_CACHE_OP_WRITE,
    //                               PERF_COUNT_HW_CACHE_RESULT_MISS));
    // PerfEventOpen(cache_name + "_prefetch_access", PERF_TYPE_HW_CACHE,
    //               MakeCacheConfig(cache_id, PERF_COUNT_HW_CACHE_OP_PREFETCH,
    //                               PERF_COUNT_HW_CACHE_RESULT_ACCESS));
    // PerfEventOpen(cache_name + "_prefetch_miss", PERF_TYPE_HW_CACHE,
    //               MakeCacheConfig(cache_id, PERF_COUNT_HW_CACHE_OP_PREFETCH,
    //                               PERF_COUNT_HW_CACHE_RESULT_MISS));
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
