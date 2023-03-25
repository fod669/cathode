
CathodeContext* g_crt;

typedef struct _CathodePrivate
{
	CriticalSection			logCritSec;
} _CathodePrivate;

internal_var _CathodePrivate* g_cp;

internal_func void _cathode_private_init(_CathodePrivate* cp)
{
	ASSERT_RAW(cp != NULL);
	g_cp = NULL;
	os_critsec_init(&cp->logCritSec);
	g_cp = cp;
}

internal_func void _cathode_private_shutdown(_CathodePrivate* cp)
{
	ASSERT_RAW(cp != NULL);
	g_cp = NULL;
	os_critsec_delete(&cp->logCritSec);
}

internal_func void _os_init(void);
internal_func void _os_shutdown(void);

// User entry point.
int cth_main(Arena* arena, int argc, str8_const argv[]);

NORETURN void STDCALL crt_entry(void)
{
	int result = EXIT_SUCCESS;

	g_crt = NULL;

	_CathodePrivate cathodePrivate = {0};
	_cathode_private_init(&cathodePrivate);
	_os_init();

	// TODO: Need an init/shutdown function for CathodeContext

	CathodeContext cathodeContext =
	{
		.arena = arena_create("CRT", MEGABYTES(10), 0, NULL)
	};

	if(cathodeContext.arena == NULL)
	{
		result = EXIT_CODE_ARENA_CREATE_FAIL;
	}
	else
	{
		g_crt = &cathodeContext;

		int argc = 0;
		str8_const* argv = str8_extract_arg_vector(cathodeContext.arena, os_get_command_line_args_str8(), &argc);
		result = cth_main(cathodeContext.arena, argc, argv);

		g_crt = NULL;
		arena_destroy(cathodeContext.arena);
	}

	_os_shutdown();
	_cathode_private_shutdown(&cathodePrivate);

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

