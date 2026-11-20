#ifndef _SM64_NX_H_
#define _SM64_NX_H_

// Wrapper around <switch.h> (libnx) for the Nintendo Switch build.
//
// libnx and the N64 SDK headers both define the 64-bit integer typedefs, but
// with different underlying types (long vs long long), which is an error in C.
// Both are 64 bits wide on the Switch, so libnx's versions are simply renamed
// while its headers are being read.

#ifdef __SWITCH__

#define u64  nx_u64
#define s64  nx_s64
#define vu64 nx_vu64
#define vs64 nx_vs64

#include <switch.h>

#undef u64
#undef s64
#undef vu64
#undef vs64

#endif // __SWITCH__

#endif // _SM64_NX_H_
