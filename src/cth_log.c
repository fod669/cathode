
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

static char* printf_callback(const char* buf, void* user, int len)
{
	os_output_debug_string(buf);
	return g_crt.sprintfBuffer;
}

void log_printfv(eLogErrorLevel errorLevel, const char* format, va_list args)
{
	ASSERT_RAW(g_crt.initialised);

	os_critsec_enter(&g_crt.logCritSec);
	stbsp_vsprintfcb(printf_callback, (void*)errorLevel, g_crt.sprintfBuffer, format, args);
	os_critsec_leave(&g_crt.logCritSec);
}

