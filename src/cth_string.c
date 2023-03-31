
// This function takes a command line string and converts it into an array of arguments (ie: argv, argc).
// This is how windows parses the command line string. We don't follow that because, well, to me it seems stupid.
// https://www.daviddeley.com/autohotkey/parameters/parameters.htm
// This is how powershell parses the command line string. I'm doing it more this way, using ` as the escape character.
// https://ss64.com/ps/syntax-esc.html
str8_const* str8_extract_arg_vector(Arena* arena, str8_const cmdLine, int* out_argCount)
{
	str8_const* argv = NULL;
	*out_argCount = 0;

	if (cmdLine.len > 0)
	{
		// Memory for all the argument strings.
		char* argMem = PUSH_TYPE_ARRAY(arena, char, cmdLine.len + 1);

		// First argument.
		argv = PUSH_TYPE(arena, str8_const);
		argv->str = argMem;

		str8* currentArg = (str8*)argv;
		++(*out_argCount);

		bool argEnded = false;
		bool inQuotes = false;
		for (u64 i = 0; i < cmdLine.len; ++i)
		{
			char c = cmdLine.str[i];
			switch (c)
			{
				case ' ':
				case '\t':
				{
					if (inQuotes == false)
					{
						if (currentArg->len > 0)
						{
							argEnded = true;
						}
						continue;
					}
					break;
				}
				case '\"':
				{
					inQuotes = !inQuotes;
					continue;
				}
				case '`':
				{
					// Peek at the next character to check if this is a valid escape sequence that needs replacing.
					switch (cmdLine.str[i + 1])
					{
						case '\"':	++i;	c = '\"';	break;
						case '0':	++i;	c = '\0';	break;
						case 'n':	++i;	c = '\n';	break;
						case 'r':	++i;	c = '\r';	break;
						case 't':	++i;	c = '\t';	break;
						case '`':	++i;	c = '`';	break;
					}
					break;
				}
			}

			if (argEnded)
			{
				argEnded = false;
				currentArg->str[currentArg->len] = '\0';
				str8* newArg = PUSH_TYPE(arena, str8);
				newArg->str = currentArg->str + currentArg->len + 1;

				currentArg = newArg;
				++(*out_argCount);
			}

			currentArg->str[currentArg->len++] = c;
		}

		#if CTH_DEBUG
			for (int i = 0; i < *out_argCount; ++i)
			{
				ASSERT(argv[i].str[argv[i].len] == '\0');
			}
			str8_const* lastArg = &argv[*out_argCount - 1];
			ASSERT((lastArg->str + lastArg->len) <= (argMem + cmdLine.len));
		#endif
	}

	return argv;
}

typedef struct _Str8PrintfInfo
{
	Arena*			arena;
	u64				stringLength;
	u64				totalPushed;
} _Str8PrintfInfo;

internal_func char* _str8_printf_callback(const char* buf, void* user, int len)
{
	_Str8PrintfInfo* info = (_Str8PrintfInfo*)user;

	info->stringLength += len;

	char* stringBuffer = NULL;
	if (len >= STB_SPRINTF_MIN)
	{
		stringBuffer = PUSH_TYPE_ARRAY(info->arena, char, STB_SPRINTF_MIN);
		info->totalPushed += STB_SPRINTF_MIN;
	}
	return stringBuffer;
}

str8 str8_printfv(Arena* arena, const char* format, va_list args)
{
	char* stringBuffer = PUSH_TYPE_ARRAY(arena, char, STB_SPRINTF_MIN);

	_Str8PrintfInfo info =
	{
		.arena			= arena,
		.stringLength	= 0,
		.totalPushed	= STB_SPRINTF_MIN
	};

	stbsp_vsprintfcb(_str8_printf_callback, &info, stringBuffer, format, args);
	ASSERT(info.totalPushed > info.stringLength);
	POP_BYTES(arena, info.totalPushed - info.stringLength - 1, false);

	str8 s =
	{
		.str = stringBuffer,
		.len = info.stringLength
	};
	s.str[s.len] = '\0';
	return s;
}

