
inline u64 str8_len(const char* s)
{
	ASSERT(s != NULL);

	const char* end = s;
	while (*end) { ++end; }
	return end - s;
}

inline str8_const str8_const_from_cstring(const char* s)
{
	ASSERT(s != NULL);
	str8_const result =
	{
		.str = s,
		.len = str8_len(s)
	};
	ASSERT(result.str[result.len] == '\0');
	return result;
}

inline str8 str8_push(Arena* arena, str8_const s)
{
	ASSERT(s.str != NULL);
	str8 result =
	{
		.str = PUSH_TYPE_ARRAY(arena, char, s.len + 1),
		.len = s.len
	};
	os_mem_cpy(result.str, s.str, s.len + 1);
	ASSERT(result.str[result.len] == '\0');
	return result;
}

inline str8 str8_push_cstring(Arena* arena, const char* s)
{
	ASSERT(s != NULL);
	u64 len = str8_len(s);
	str8 result =
	{
		.str = PUSH_TYPE_ARRAY(arena, char, len + 1),
		.len = len
	};
	os_mem_cpy(result.str, s, len + 1);
	ASSERT(result.str[result.len] == '\0');
	return result;
}

inline str8 str8_printf(Arena* arena, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	str8 s = str8_printfv(arena, format, args);
	va_end(args);
	return s;
}

