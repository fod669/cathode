
typedef struct _CathodePrivate
{
	CriticalSection			logCritSec;
} _CathodePrivate;

CathodeContext*					g_crt;
internal_var _CathodePrivate*	g_cp;
_OSContext*						g_os;

internal_func int _cathode_private_init(_CathodePrivate* cp)
{
	ASSERT(cp != NULL);
	os_critsec_init(&cp->logCritSec);
	return 0;
}

internal_func void _cathode_private_shutdown(_CathodePrivate* cp)
{
	ASSERT(cp != NULL);
	ASSERT(g_cp == NULL);
	os_critsec_delete(&cp->logCritSec);
}

internal_func int _os_init(_OSContext *os);
internal_func void _os_shutdown(_OSContext *os);
internal_func bool _os_tls_create(const char* arenaName, u32 tlsIndex);
internal_func void _os_tls_destroy(u32 tlsIndex);

internal_func int _cathode_context_init(CathodeContext* cc)
{
	ASSERT(cc != NULL);
	int result = 0;

	cc->arena = arena_create("CRT", CTH_ARENA_MEM_RESERVE, 0, NULL);
	if(cc->arena == NULL)
	{
		result = EXIT_CODE_ARENA_CREATE_FAIL;
	}

	return result;
}

internal_func void _cathode_context_shutdown(CathodeContext* cc)
{
	ASSERT(cc != NULL);
	ASSERT(g_crt == NULL);
	arena_destroy(cc->arena);
}

// User entry point.
int cth_main(int argc, str8_const argv[]);

NORETURN void STDCALL crt_entry(void)
{
	int result = EXIT_SUCCESS;

	g_crt = NULL;
	g_cp = NULL;
	g_os = NULL;

	_CathodePrivate cathodePrivate = {0};
	result = _cathode_private_init(&cathodePrivate);
	if (result != 0) { goto cathode_private_init_fail; }
	g_cp = &cathodePrivate;

	_OSContext osContext = {0};
	result = _os_init(&osContext);
	if (result != 0) { goto os_init_fail; }
	g_os = &osContext;

	if (_os_tls_create("Main thread", g_os->tlsIndex) == false) { goto tls_init_fail; }

	CathodeContext cathodeContext = {0};
	result = _cathode_context_init(&cathodeContext);
	if (result != 0) { goto cathode_context_init_fail; }
	g_crt = &cathodeContext;

	int argc = 0;
	str8_const* argv = str8_extract_arg_vector(cathodeContext.arena, os_get_command_line_args_str8(), &argc);
	result = cth_main(argc, argv);

	g_crt = NULL;
	_cathode_context_shutdown(&cathodeContext);
	cathode_context_init_fail:

	_os_tls_destroy(g_os->tlsIndex);
	tls_init_fail:

	g_os = NULL;
	_os_shutdown(&osContext);
	os_init_fail:

	g_cp = NULL;
	_cathode_private_shutdown(&cathodePrivate);
	cathode_private_init_fail:

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

