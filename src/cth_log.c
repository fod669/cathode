
void log_printf(eLogErrorLevel errorLevel, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	log_printfv(errorLevel, format, args);
	va_end(args);
}

void log_printf_dbg(eLogErrorLevel errorLevel, const char* file, u32 line, const char* format, ...)
{
	ASSERT(g_cp != NULL);

	// Entering a critical section multiple times on the same thread is fine, but we must make sure
	// to leave the critical section the same number of times we entered it.
	os_critsec_enter(&g_cp->logCritSec);

	log_printf(errorLevel, "[%u] %s(%d): ", os_thread_get_ID(), file, line);

	va_list args;
	va_start(args, format);
	log_printfv(errorLevel, format, args);
	va_end(args);

	os_critsec_leave(&g_cp->logCritSec);
}

typedef struct _LogPrintfInfo
{
	eLogErrorLevel	errorLevel;
	char			buffer[STB_SPRINTF_MIN + 1];	// Space for a NULL terminator.
} _LogPrintfInfo;

internal_func char* _log_printf_callback(const char* buf, void* user, int len)
{
	eConsoleTextColour textColours[] =
	{
		CTC_GREEN,				// LOG_SUCCESS
		CTC_GREY,				// LOG_INFO
		CTC_YELLOW,				// LOG_WARNING
		CTC_RED,				// LOG_ERROR
		CTC_RED_BACKGROUND		// LOG_CRITICAL
	};
	STATIC_ASSERT(countof(textColours) == LOG_LEVEL_COUNT);

	_LogPrintfInfo* info = (_LogPrintfInfo*)user;
	ASSERT(info->errorLevel >= 0 && info->errorLevel < LOG_LEVEL_COUNT);

	// Make sure it's NULL terminated. Also fuck const.
	((char*)buf)[len] = '\0';
	os_console_write(buf, len, textColours[info->errorLevel]);
	os_output_debug_string(buf);

	return info->buffer;
}

void log_printfv(eLogErrorLevel errorLevel, const char* format, va_list args)
{
	ASSERT(g_cp != NULL);

	_LogPrintfInfo info =
	{
		.errorLevel = errorLevel
	};

	os_critsec_enter(&g_cp->logCritSec);
	stbsp_vsprintfcb(_log_printf_callback, (void*)&info, info.buffer, format, args);
	os_critsec_leave(&g_cp->logCritSec);
}

