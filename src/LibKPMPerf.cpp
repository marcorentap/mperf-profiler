#include <LibKPMPerf.hpp>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::vector<std::tuple<std::string, double, double>> regionMeasures;
std::unique_ptr<MPerf::MPerf> mperf;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t /*devInfoCount*/,
                                     void * /*deviceInfo*/) {
  mperf.reset(new MPerf::MPerf());
}

extern "C" void kokkosp_finalize_library() {
  double IPCsum = 0, CPIsum = 0;
  double IPCavg, CPIavg;

  if (regionMeasures.size() <= 0) {
    return;
  }
  for (auto &measure : regionMeasures) {
    std::string regionName = std::get<0>(measure);
    double CPI = std::get<1>(measure);
    double IPC = std::get<2>(measure);
    CPIsum += CPI;
    IPCsum += IPC;
    printf("Region %s: CPI %lf IPC %lf\n", regionName.c_str(), CPI, IPC);
  }

  CPIavg = CPIsum / regionMeasures.size();
  IPCavg = IPCsum / regionMeasures.size();
  printf("Average CPI: %lf\nAverage IPC: %lf\n", CPIavg, IPCavg);
}

extern "C" void kokkosp_begin_parallel_for(const char *name,
                                           const uint32_t devID,
                                           uint64_t *kID) {}

extern "C" void kokkosp_end_parallel_for(const uint64_t kID) {}

extern "C" void kokkosp_begin_parallel_scan(const char *name,
                                            const uint32_t devID,
                                            uint64_t *kID) {}

extern "C" void kokkospk_end_parallel_scan(const uint64_t kID) {}

extern "C" void kokkosp_begin_parallel_reduce(const char *name,
                                              const uint32_t devID,
                                              uint64_t *kID) {}

extern "C" void kokkosp_end_parallel_reduce(const uint64_t kID) {}

extern "C" void kokkosp_begin_fence(const char *name, const uint32_t devID,
                                    uint64_t *kID) {}

extern "C" void kokkosp_end_fence(const uint64_t kID) {}

extern "C" void kokkosp_push_profile_region(char *regionName) {
  mperf->ReadStartValues();
  regionNameStack.push(regionName);
}

extern "C" void kokkosp_pop_profile_region() {
  mperf->ReadEndValues();
  auto regionName = regionNameStack.top();
  auto CPI = mperf->GetCPI<double>();
  auto IPC = mperf->GetIPC<double>();
  regionMeasures.push_back({regionName, CPI, IPC});
  regionNameStack.pop();
}

extern "C" void kokkosp_allocate_data(Kokkos::Profiling::SpaceHandle handle,
                                      const char *name, void *ptr,
                                      uint64_t size) {}

extern "C" void kokkosp_deallocate_data(Kokkos::Profiling::SpaceHandle handle,
                                        const char *name, void *ptr,
                                        uint64_t size) {}

extern "C" void kokkosp_begin_deep_copy(
    Kokkos::Profiling::SpaceHandle dst_handle, const char *dst_name,
    const void *dst_ptr, Kokkos::Profiling::SpaceHandle src_handle,
    const char *src_name, const void *src_ptr, uint64_t size) {}
