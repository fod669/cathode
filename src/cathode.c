
#include "cathode.h"

#include "src/cth_crt.c"

#include "src/cth_string.c"

#if CTH_OS_WINDOWS
	#include "src/platform_win32/cth_os_win32.c"
#elif CTH_OS_APPLE
	#error Not yet implemented!
#elif CTH_OS_ANDROID
	#error Not yet implemented!
#elif CTH_OS_LINUX
	#error Not yet implemented!
#else
	#error Unknown operating system!
#endif

