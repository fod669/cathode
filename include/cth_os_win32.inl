
typedef struct _CritSec
{
	CRITICAL_SECTION data;
} CritSec;

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

inline void os_critsec_init(CritSec* cs)
{
	ASSERT(cs != NULL);
	InitializeCriticalSection(&cs->data);
}

inline void os_critsec_delete(CritSec* cs)
{
	ASSERT(cs != NULL);
	DeleteCriticalSection(&cs->data);
}

inline void os_critsec_enter(CritSec* cs)
{
	ASSERT(cs != NULL);
	EnterCriticalSection(&cs->data);
}

inline void os_critsec_leave(CritSec* cs)
{
	ASSERT(cs != NULL);
	LeaveCriticalSection(&cs->data);
}

inline void os_output_debug_string(const char* buf)
{
	OutputDebugStringA(buf);
}

