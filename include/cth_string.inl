
inline u64 str8_len(const char* s)
{
	ASSERT(s != NULL);

	const char* end = s;
	while (*end) { ++end; }
	return end - s;
}

inline str8 str8_from_cstring(char* s)
{
	ASSERT(s != NULL);
	str8 result =
	{
		.str = s,
		.len = str8_len(s)
	};
	return result;
}

inline str8_c str8_c_from_cstring(const char* s)
{
	ASSERT(s != NULL);
	str8_c result =
	{
		.str = s,
		.len = str8_len(s)
	};
	return result;
}

inline str8 str8_push(Arena* arena, str8_c s)
{
	ASSERT(s.str != NULL);
	str8 result =
	{
		.str = PUSH_TYPE_ARRAY(arena, char, s.len),
		.len = s.len
	};
	memcpy(result.str, s.str, s.len);
	return result;
}

inline str8 str8_push_cstring(Arena* arena, const char* s)
{
	ASSERT(s != NULL);
	u64 len = str8_len(s);
	str8 result =
	{
		.str = PUSH_TYPE_ARRAY(arena, char, len),
		.len = len
	};
	memcpy(result.str, s, result.len);
	return result;
}

