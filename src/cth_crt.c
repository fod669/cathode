
// User entry point.
int cth_main(Arena* arena, int argc, str8 argv[]);

NORETURN void STDCALL crt_entry(void)
{
	int result = EXIT_FAILURE;
	int argc = 0;

	Arena* arena = arena_create(STR8("CRT"), MEGABYTES(10), 0, NULL);
	str8* argv = str8_parse_command_line(arena, os_get_command_line_args_str8(), &argc);
	result = cth_main(arena, argc, argv);
	arena_destroy(arena);

	os_exit_process(result);
}

