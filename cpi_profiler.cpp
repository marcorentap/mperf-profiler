#include <asm/unistd.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <impl/Kokkos_Profiling_DeviceInfo.hpp>
#include <impl/Kokkos_Profiling_Interface.hpp>
#include <linux/perf_event.h>
#include <stack>
#include <string>
#include <tuple>
#include <unistd.h>
#include <vector>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

class CPIProfiler {
  private:
    enum { INSTCOUNT, CYCLECOUNT };

    int fd[2];
    uint64_t startInstCount;
    uint64_t endInstCount;
    uint64_t startCycleCount;
    uint64_t endCycleCount;
    struct perf_event_attr attrs[2];

  public:
    CPIProfiler() {
        std::memset(attrs, 0, sizeof(attrs));
        attrs[INSTCOUNT].type = PERF_TYPE_HARDWARE;
        attrs[INSTCOUNT].config = PERF_COUNT_HW_INSTRUCTIONS;
        attrs[CYCLECOUNT].type = PERF_TYPE_HARDWARE;
        attrs[CYCLECOUNT].config = PERF_COUNT_HW_CPU_CYCLES;

        fd[INSTCOUNT] = perf_event_open(&attrs[INSTCOUNT], getpid(), -1, -1, 0);
        if (fd[INSTCOUNT] < 0) {
            perror("cannot open fd for instruction count");
            exit(EXIT_FAILURE);
        }

        fd[CYCLECOUNT] =
            perf_event_open(&attrs[CYCLECOUNT], getpid(), -1, -1, 0);
        if (fd[CYCLECOUNT] < 0) {
            perror("cannot open fd for cycle count");
            exit(EXIT_FAILURE);
        }
    }

    void ReadStartValues() {
        int ret;
        ret = read(fd[INSTCOUNT], &startInstCount, sizeof(startInstCount));
        if (ret < 0) {
            perror("cannot read starting instruction count");
            exit(EXIT_FAILURE);
        }
        ret = read(fd[CYCLECOUNT], &startCycleCount, sizeof(startCycleCount));
        if (ret < 0) {
            perror("cannot read starting cycle count");
            exit(EXIT_FAILURE);
        }
    }

    void ReadEndValues() {
        int ret;
        ret = read(fd[INSTCOUNT], &endInstCount, sizeof(endInstCount));
        if (ret < 0) {
            perror("cannot read ending instruction count");
            exit(EXIT_FAILURE);
        }
        ret = read(fd[CYCLECOUNT], &endCycleCount, sizeof(endCycleCount));
        if (ret < 0) {
            perror("cannot read ending cycle count");
            exit(EXIT_FAILURE);
        }
    }

    double GetCPI() {
        double instDiff = endInstCount - startInstCount;
        double cycleDiff = endCycleCount - startCycleCount;
        return (double)cycleDiff / instDiff;
    }

    double GetIPC() {
        double instDiff = endInstCount - startInstCount;
        double cycleDiff = endCycleCount - startCycleCount;
        return (double)instDiff / cycleDiff;
    }
};

// Region Name, CPI, IPC
std::stack<std::string> regionNameStack;
std::vector<std::tuple<std::string, double, double>> regionMeasures;
CPIProfiler *cpiProfiler;

extern "C" void kokkosp_init_library(const int loadSeq,
                                     const uint64_t interfaceVer,
                                     const uint32_t /*devInfoCount*/,
                                     void * /*deviceInfo*/) {
    cpiProfiler = new CPIProfiler();
}

extern "C" void kokkosp_finalize_library() {
    for (auto &measure : regionMeasures) {
        printf("Region %s, CPI %lf IPC %lf\n", std::get<0>(measure).c_str(),
               std::get<1>(measure), std::get<2>(measure));
    }
    // TODO: Print average CPI, IPC
    delete cpiProfiler;
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
    cpiProfiler->ReadStartValues();
    regionNameStack.push(regionName);
}

extern "C" void kokkosp_pop_profile_region() {
    cpiProfiler->ReadEndValues();
    auto regionName = regionNameStack.top();
    auto CPI = cpiProfiler->GetCPI();
    auto IPC = cpiProfiler->GetIPC();
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
