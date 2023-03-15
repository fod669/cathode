
void log_printf(eLogErrorLevel errorLevel, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	log_printfv(errorLevel, format, args);
	va_end(args);
}

void log_printf_dbg(eLogErrorLevel errorLevel, const char* file, u32 line, const char* format, ...)
{
	ASSERT_RAW(g_crt.initialised);

	// Entering a critical section multiple times on the same thread is fine, but we must make sure
	// to leave the critical section the same number of times we entered it.
	os_critsec_enter(&g_crt.logCritSec);

	log_printf(errorLevel, "[%u] %s(%d): ", os_thread_get_ID(), file, line);

	va_list args;
	va_start(args, format);
	log_printfv(errorLevel, format, args);
	va_end(args);

	os_critsec_leave(&g_crt.logCritSec);
}

void log_printfv(eLogErrorLevel errorLevel, const char* format, va_list args)
{
	ASSERT_RAW(g_crt.initialised);

	os_critsec_enter(&g_crt.logCritSec);

/*	WORD colours[LOG_LEVEL_COUNT] =
	{
		FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_INTENSITY,
		BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	};

	PrintfInfo info;
	info.consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (info.consoleHandle != INVALID_HANDLE_VALUE)
	{
		if (info.consoleHandle && info.consoleHandle != INVALID_HANDLE_VALUE)
		{
			if (SetConsoleTextAttribute(info.consoleHandle, colours[errorLevel]) == false)
			{
				log_last_error();
			}
		}

		stbsp_vsprintfcb(log_stb_printf_callback, (void*)&info, info.buffer, format, args);

		if (info.consoleHandle && info.consoleHandle != INVALID_HANDLE_VALUE)
		{
			if (SetConsoleTextAttribute(info.consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) == false)
			{
				log_last_error();
			}
		}
	}*/

	os_critsec_leave(&g_crt.logCritSec);
}
/*
typedef struct _PrintfInfo
{
	char buffer[STB_SPRINTF_MIN];
	HANDLE consoleHandle;
} PrintfInfo;

internal_func char* log_stb_printf_callback(const char* buf, void* user, int len)
{
	PrintfInfo* info = (PrintfInfo*)user;
	if (info->consoleHandle && info->consoleHandle != INVALID_HANDLE_VALUE)
	{
		DWORD charsWritten;
		if (WriteFile(info->consoleHandle, buf, len, &charsWritten, 0) == false)
		{
			log_last_error();
		}
		else
		{
			ASSERT((int)charsWritten == len);
		}
	}

	OutputDebugStringA(buf);
	return info->buffer;
}

void log_last_error()
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
*/
