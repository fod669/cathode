
typedef struct CriticalSection
{
	CRITICAL_SECTION data;
} CriticalSection;

typedef struct _OSContext
{
	size_t		pageSize;
	void*		lowestAccessibleAddress;
	void*		highestAccessibleAddress;
	size_t		baseAddressAllocationGranularity;
} _OSContext;

inline u8* os_thread_get_TIB(void)
{
	#if CTH_ARCH_32_BIT
		u8* tib = (u8*)__readfsdword(0x18);
	#elif CTH_ARCH_64_BIT
		u8* tib = (u8*)__readgsqword(0x30);
	#else
		#error Unsupported architecture!
	#endif

	return tib;
}

inline u32 os_thread_get_ID(void)
{
	u32 threadID = *(u32*)(os_thread_get_TIB() + 0x48);
	return threadID;
}

inline void os_critsec_init(CriticalSection* cs)
{
	ASSERT(cs != NULL);
	InitializeCriticalSection(&cs->data);
}

inline void os_critsec_delete(CriticalSection* cs)
{
	ASSERT(cs != NULL);
	DeleteCriticalSection(&cs->data);
}

inline void os_critsec_enter(CriticalSection* cs)
{
	ASSERT(cs != NULL);
	EnterCriticalSection(&cs->data);
}

inline void os_critsec_leave(CriticalSection* cs)
{
	ASSERT(cs != NULL);
	LeaveCriticalSection(&cs->data);
}

inline void os_output_debug_string(const char* buf)
{
	OutputDebugStringA(buf);
}

// TODO: Replace this memset with something faster. Possibly __movsb, explained here: https://hero.handmade.network/forums/code-discussion/t/157
// Maybe __stosd also?
inline void* os_mem_set(void* ptr, u8 val, size_t byteCount)
{
	u8* bytes = (u8*)ptr;
	while (byteCount--)
	{
		*bytes++ = val;
	}
	return ptr;
}

// TODO: Replace with some type of faster ZeroMemory call.
inline void* os_mem_zero(void* ptr, size_t byteCount)
{
	return os_mem_set(ptr, 0, byteCount);
}

// TODO: Replace this with faster memcpy code.
inline void* os_mem_cpy(void* RESTRICT dst, const void* RESTRICT src, size_t byteCount)
{
	u8* dst8 = (u8*)dst;
	const u8* src8 = (const u8*)src;

	// Assert that the ranges don't overlap because we're using RESTRICT.
	ASSERT(((dst8 + byteCount) < src8) || ((src8 + byteCount) < dst8));

	while (byteCount--)
	{
		*dst8++ = *src8++;
	}
	return dst;
}

