
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

NORETURN void os_exit_process(int result)
{
	ExitProcess(result);
}

