
inline u64 str8_len(const char* s)
{
	ASSERT(s != NULL);

	const char* end = s;
	while (*end) { ++end; }
	return end - s;
}

