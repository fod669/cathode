
#pragma once
#ifndef CTH_STRING_H_
#define CTH_STRING_H_

// You can print all these string types in a format string with "%P".
// eg: log_info("string: %P\n", (str8){ "Hi", 2 });
typedef struct str8
{
	char*			str;	// This is NULL terminated.
	u64				len;
} str8;

typedef struct str8_const
{
	const char*		str;	// This is NULL terminated.
	u64				len;
} str8_const;

typedef struct str8_view
{
	const char*		str;	// This is NOT necessarily NULL terminated.
	u64				len;
} str8_view;

// The above structs always need to be interchangable.
STATIC_ASSERT(sizeof(str8) == sizeof(str8_const));
STATIC_ASSERT(sizeof(str8) == sizeof(str8_view));

#define _S8C(_X)	str8_const_from_cstring(_X)

u64					str8_len(const char* s);

str8_const			str8_const_from_cstring(const char* s);

str8				str8_push(Arena* arena, str8_const s);
str8				str8_push_cstring(Arena* arena, const char* s);

str8_const*			str8_extract_arg_vector(Arena* arena, str8_const cmdLine, int* out_argCount);

str8				str8_printf(Arena* arena, const char* format, ...) STBSP__ATTRIBUTE_FORMAT(2, 3);
str8				str8_printfv(Arena* arena, const char* format, va_list args);

#endif // CTH_STRING_H_

