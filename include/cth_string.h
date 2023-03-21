
#pragma once
#ifndef CTH_STRING_H_
#define CTH_STRING_H_

// NOTE: These strings are not null terminated.
// You can print these in a format string with "%P".
typedef struct _str8
{
	char*			str;
	u64				len;
} str8;

typedef struct _str8_c
{
	const char*		str;
	u64				len;
} str8_c;

// The above structs always need to be interchangable.
STATIC_ASSERT(sizeof(str8) == sizeof(str8_c));

#define STR8C(_X)	str8_c_from_cstring(_X)

u64					str8_len(const char* s);

str8				str8_from_cstring(char* s);
str8_c				str8_c_from_cstring(const char* s);

str8				str8_push(Arena* arena, str8_c s);
str8				str8_push_cstring(Arena* arena, const char* s);

str8_c*				str8_extract_arg_vector(Arena* arena, str8_c cmdLine, int* out_argCount);

#endif // CTH_STRING_H_

