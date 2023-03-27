
#pragma once
#ifndef CATHODE_H_
#define CATHODE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// Make sure cth_defines.h is always first.
#include "include/cth_defines.h"

#if CTH_C_VER < CTH_C11
	#error Need to compile with c11 or higher!
#endif

// Third party libs used by cathode go here.
#include "third_party/stb/stb_sprintf.h"

// Make sure cth_crt.h is before any other cth_* includes (apart from cth_defines.h).
#include "include/cth_crt.h"

#include "include/cth_assert.h"
#include "include/cth_log.h"
#include "include/cth_string.h"
#include "include/cth_arena.h"
#include "include/cth_os.h"

// Inlines go here.
#include "include/cth_string.inl"

#if CTH_OS_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
	#include "include/cth_os_win32.inl"
#elif CTH_OS_APPLE
	#error Not yet implemented!
#elif CTH_OS_ANDROID
	#error Not yet implemented!
#elif CTH_OS_LINUX
	#error Not yet implemented!
#else
	#error Unknown operating system!
#endif

#endif // CATHODE_H_

/*

TODO:

Coding standards:
- Make sure all internal functions, structs, and variables start with an underscore.
- Make sure to properly label all internal stuff with the internal_func, internal_var etc keywords.
- Make sure keywords like restrict are used when appropriate.
- Make sure to use ASSERTS and ERROR_FATAL appropriately.
- Look out for const correctness.
- Zero is initialisation everywhere.

Tools:
+ Create a batch script that contains all the compiler settings necessary to build a cathode project (removing the CRT etc).
- Document what the enabled and disabled warnings do.
- Deterministic builds.
- Set initial CRT memory arena size using a #define.
- Look into valgrind to find memory bugs.
- Look into clang-tidy for static analysis.
- Maybe look into clang-format?
- Build script stuff.
	- debug/release options
	- output debug/release builds to different directories
	- output object files to obj directory
	- clean option
	- no CRT options
	- errors as warnings
	- most warnings enabled
	- global user settings (clang directory etc)
	- project user settings (debug/release option etc)

Tech:
- Look up how to integrate unicode support into strings.
- Look into the interplay between str8 and unicode.
- Think of a way to have temporary memory pushed onto arenas.
- Think of a way to virtual alloc more memory onto the end of an allocated region so memory can always increase.

Code:
- Add an offsetof macro.
- Add string functions for str8_view.
- Write tests for all CRT code.
- Make arena memory allocation thread safe.

Features:
- Console input.
- File IO.
- Logging to a file.
- More extensive string manipulation library.
- Timers.
- Window management.
- File watcher.
- Code hot reloading.
- DLL loading.
- Threading & thread contexts.
	- Do this next because we'll need to think carefully about how threads handle their own memory, and also
	  we need threads to add thread safety to memory allocation.
	- Look up TlsAlloc().
	- Mutexes.

*/


/*

DONE:

Features:
- CRT replacement.
- Virtual memory allocation with arenas.
- Asserts with "ignore, debug, continue" options.
- Fatal error popups.
- Console output for info/warnings/errors etc.
- Basic string support (str8).
- Support for sprintf with str8 (str8_printf).
- Basic OS message boxes (windows).
- Critical sections (windows).

*/

