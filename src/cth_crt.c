
typedef struct _CathodeContext
{
	bool			initialised;
	CritSec			logCritSec;
	char			sprintfBuffer[STB_SPRINTF_MIN];
} CathodeContext;

CathodeContext g_crt;

void os_crt_init(void);
void os_crt_shutdown(void);

void crt_init(void)
{
	os_critsec_init(&g_crt.logCritSec);
	g_crt.initialised = true;
}

void crt_shutdown(void)
{
	g_crt.initialised = false;
	os_critsec_delete(&g_crt.logCritSec);
}

NORETURN void STDCALL crt_entry(void)
{
	int result = EXIT_SUCCESS;

	crt_init();
	os_crt_init();

	Arena* arena = arena_create(STR8C("CRT"), MEGABYTES(10), 0, NULL);
	if(arena == NULL)
	{
		result = EXIT_CODE_ARENA_CREATE_FAIL;
	}
	else
	{
		int argc = 0;
		str8_c* argv = str8_parse_command_line(arena, os_get_command_line_args_str8(), &argc);
		result = cth_main(arena, argc, argv);

		arena_destroy(arena);
	}

	os_crt_shutdown();
	crt_shutdown();

	os_exit_process(result);
}

// CRT functionality
// ================================================================================================

// Enable floats.
int _fltused ATTR_USED = 42;

// TODO: Replace this memset with something faster. Possibly __movsb, explained here: https://hero.handmade.network/forums/code-discussion/t/157
void* memset(void* ptr, int value, size_t byteCount)
{
	u8 val = (u8)value;
	u8* bytes = (u8*)ptr;
	while (byteCount--)
	{
		*bytes++ = val;
	}
	return ptr;
}

// TODO: Replace this with faster memcpy code.
void* memcpy(void* dst, const void* src, size_t byteCount)
{
	u8* dst8 = (u8*)dst;
	const u8* src8 = (const u8*)src;
	while (byteCount--)
	{
		*dst8++ = *src8++;
	}
	return dst;
}

