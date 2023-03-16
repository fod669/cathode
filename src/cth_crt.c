
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

// User entry point.
int cth_main(Arena* arena, int argc, str8 argv[]);

// Enable floats.
int _fltused = 42;

NORETURN void STDCALL crt_entry(void)
{
	crt_init();
	os_crt_init();
	Arena* arena = arena_create(STR8("CRT"), MEGABYTES(10), 0, NULL);

	int argc = 0;
	str8* argv = str8_parse_command_line(arena, os_get_command_line_args_str8(), &argc);
	int result = cth_main(arena, argc, argv);

	arena_destroy(arena);
	os_crt_shutdown();
	crt_shutdown();

	os_exit_process(result);
}

