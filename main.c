#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "cpi_profiler.h"

int main(int argc, char *argv[]) {
    typeof (&func1) func;
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    void *handle = dlopen("./libkp_cpi.so", RTLD_LAZY);
    if (!handle) {
        printf("%s\n", dlerror());
        return -1;
    }

    func = dlsym(handle, "func1");
    printf("func1: %d\n", func(x, y));
    return 0;
}
