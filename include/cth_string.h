
#pragma once
#ifndef CTH_STRING_H_
#define CTH_STRING_H_

typedef struct _str8
{
	char*	str;
	u64		len;
} str8;

#define STR8(_X)	str8_from_cstring(_X)

u64					str8_len(const char* s);

str8				str8_from_cstring(char* s);

str8*				str8_parse_command_line(Arena* arena, str8 commandLine, int* out_argCount);

str8				str8_push(Arena* arena, str8 s);

#endif // CTH_STRING_H_

