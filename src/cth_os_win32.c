
str8 os_get_command_line_args_str8(void)
{
	return STR8(GetCommandLineA());
}

NORETURN void os_exit_process(int result)
{
	ExitProcess(result);
}

