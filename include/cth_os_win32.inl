
typedef struct _CritSec
{
	CRITICAL_SECTION data;
} CritSec;

inline u32 os_thread_get_ID()
{
	// TODO: Implement me!
	return 0;
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

