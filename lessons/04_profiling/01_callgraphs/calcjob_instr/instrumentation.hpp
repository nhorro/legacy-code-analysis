#pragma once

extern "C" void __cyg_profile_func_enter(void* func, void* caller);
extern "C" void __cyg_profile_func_exit(void* func, void* caller);

void resolve_addr2line(void* addr);
