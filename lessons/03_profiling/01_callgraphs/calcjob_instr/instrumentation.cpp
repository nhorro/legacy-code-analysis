#include "instrumentation.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <cstdint>
#include <cstdio>

__attribute__((no_instrument_function))
uint64_t get_timestamp_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

__attribute__((no_instrument_function))
pid_t get_tid() {
    return syscall(SYS_gettid);  // Para distinguir threads
}

__attribute__((no_instrument_function))
void write_trace(const char* event, void* func, void* caller) {
    static int fd = -1;
    if (fd == -1) {
        fd = open("trace.csv", O_CREAT | O_WRONLY | O_APPEND, 0644);
    }

    char buf[256];
    uint64_t ts = get_timestamp_ns();
    pid_t tid = get_tid();
    int len = snprintf(buf, sizeof(buf),
        "%lu,%d,%s,%p,%p\n",
        ts, tid, event, func, caller);

    write(fd, buf, len);
}

extern "C" __attribute__((no_instrument_function))
void __cyg_profile_func_enter(void* func, void* caller) {
    static thread_local bool in_hook = false;
    if (in_hook) return;
    in_hook = true;

    write_trace("ENTER", func, caller);

    in_hook = false;
}

extern "C" __attribute__((no_instrument_function))
void __cyg_profile_func_exit(void* func, void* caller) {
    static thread_local bool in_hook = false;
    if (in_hook) return;
    in_hook = true;

    write_trace("EXIT", func, caller);

    in_hook = false;
}