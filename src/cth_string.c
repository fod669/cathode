
// This function takes a command line string and converts it into an array of arguments (ie: argv, argc).
// This is how windows parses the command line string. We don't follow that because, well, to me it seems stupid.
// https://www.daviddeley.com/autohotkey/parameters/parameters.htm
// This is how powershell parses the command line string. I'm doing it more this way, using ` as the escape character.
// https://ss64.com/ps/syntax-esc.html
str8_c* str8_extract_arg_vector(Arena* arena, str8_c cmdLine, int* out_argCount)
{
	str8_c* argv = NULL;
	*out_argCount = 0;

	if (cmdLine.str)
	{
		// Memory for all the argument strings.
		char* argMem = PUSH_TYPE_ARRAY(arena, char, cmdLine.len);

		// First argument.
		argv = PUSH_TYPE(arena, str8_c);
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

				str8* newArg = PUSH_TYPE(arena, str8);
				newArg->str = currentArg->str + currentArg->len;

				currentArg = newArg;
				++(*out_argCount);
			}

			currentArg->str[currentArg->len++] = c;
		}
	}

	return argv;
}

