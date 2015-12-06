#pragma once

#ifdef _WIN32
#include <intrin.h>
#define DEBUG_BREAK __debugbreak()
#else
#include <signal.h>
#define DEBUG_BREAK raise(SIGTRAP)
#endif