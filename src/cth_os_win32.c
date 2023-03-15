
str8 os_get_command_line_args_str8(void)
{
	return STR8(GetCommandLineA());
}

NORETURN void os_exit_process(int result)
{
	ExitProcess(result);
}

void os_console_write(const char* buf, int length, eConsoleTextColour colour)
{
	WORD colours[CTC_COUNT] =
	{
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_RED,
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED,
	};

	// TODO: Save the std handle in the context maybe?
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (consoleHandle && consoleHandle != INVALID_HANDLE_VALUE)
	{
		if (SetConsoleTextAttribute(consoleHandle, colours[colour]) == false)
		{
			// TODO: Write this function.
			//log_last_error();
		}

		DWORD charsWritten;
		if (WriteFile(consoleHandle, buf, length, &charsWritten, 0) == false)
		{
			// TODO: Write this function.
			//log_last_error();
		}
		else
		{
			ASSERT((int)charsWritten == length);
		}

		if (SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) == false)
		{
			// TODO: Write this function.
			//log_last_error();
		}
	}
}

