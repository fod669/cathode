
#include "cathode.h"

// Make sure cth_crt.c is before any other cth_* source files.
#include "src/cth_crt.c"

#include "src/cth_log.c"
#include "src/cth_string.c"
#include "src/cth_arena.c"

#if CTH_OS_WINDOWS
	#include "src/cth_os_win32.c"
#elif CTH_OS_APPLE
	#error Not yet implemented!
#elif CTH_OS_ANDROID
	#error Not yet implemented!
#elif CTH_OS_LINUX
	#error Not yet implemented!
#else
	#error Unknown operating system!
#endif

#define STB_SPRINTF_IMPLEMENTATION
#include "third_party/stb/stb_sprintf.h"

