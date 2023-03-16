
void os_crt_init(void)
{

}

void os_crt_shutdown(void)
{

}

str8 os_get_command_line_args_str8(void)
{
	char* cmdLine = GetCommandLineA();
	ASSERT(cmdLine != NULL);
	return STR8(cmdLine);
}

NORETURN void os_exit_process(int result)
{
	ExitProcess(result);
}

void os_console_write(const char* buf, int length, eConsoleTextColour colour)
{
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOut != NULL)
	{
		if (stdOut == INVALID_HANDLE_VALUE)
		{
			os_log_last_error();
		}
		else
		{
			CONSOLE_SCREEN_BUFFER_INFO csbInfo;
			if (!GetConsoleScreenBufferInfo(stdOut, &csbInfo))
			{
				os_log_last_error();
			}

			const WORD colours[CTC_COUNT] =
			{
				FOREGROUND_INTENSITY | FOREGROUND_GREEN,										// CTC_GREEN
				FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,						// CTC_YELLOW
				FOREGROUND_INTENSITY | FOREGROUND_RED,											// CTC_RED
				FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED,		// CTC_RED_BACKGROUND
			};
			if (!SetConsoleTextAttribute(stdOut, (colour == CTC_RED_BACKGROUND) ? colours[colour] : ((csbInfo.wAttributes & ~0xF) | colours[colour])))
			{
				os_log_last_error();
			}

			DWORD charsWritten;
			if (!WriteFile(stdOut, buf, length, &charsWritten, 0))
			{
				os_log_last_error();
			}
			else
			{
				ASSERT((int)charsWritten == length);
			}

			if (!SetConsoleTextAttribute(stdOut, csbInfo.wAttributes))
			{
				os_log_last_error();
			}
		}
	}
}

void os_log_last_error(void)
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID msgBuf;
		DWORD bufLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
									 FORMAT_MESSAGE_FROM_SYSTEM |
									 FORMAT_MESSAGE_IGNORE_INSERTS,
									 NULL,
									 error,
									 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									 (LPTSTR)&msgBuf,
									 0,
									 NULL);
		if (bufLen)
		{
			char* msgString = (char*)msgBuf;
			log_critical("GetLastError() returned: \"%s\"\n", msgString);
			LocalFree(msgBuf);
		}
	}
}

size_t os_mem_get_page_size(void)
{
	// TODO: Implement me!
	return 0;
}

size_t os_mem_get_base_granularity(void)
{
	// TODO: Implement me!
	return 0;
}

void* os_mem_reserve(size_t byteCount, void* baseAddress)
{
	// TODO: Implement me!
	return NULL;
}

void* os_mem_commit(void* address, size_t byteCount)
{
	// TODO: Implement me!
	return NULL;
}

bool os_mem_decommit(void* address, size_t byteCount)
{
	// TODO: Implement me!
	return false;
}

bool os_mem_release(void* address)
{
	// TODO: Implement me!
	return false;
}

