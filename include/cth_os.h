
#pragma once
#ifndef CTH_OS_H_
#define CTH_OS_H_

typedef enum eConsoleTextColour
{
	CTC_BLACK,
	CTC_DARKBLUE,
	CTC_DARKGREEN,
	CTC_DARKCYAN,
	CTC_DARKRED,
	CTC_DARKMAGENTA,
	CTC_DARKYELLOW,
	CTC_GREY,
	CTC_DARKGREY,
	CTC_BLUE,
	CTC_GREEN,
	CTC_CYAN,
	CTC_RED,
	CTC_MAGENTA,
	CTC_YELLOW,
	CTC_WHITE,
	
	CTC_RED_BACKGROUND,

	CTC_COUNT
} eConsoleTextColour;

str8_const			os_get_command_line_args_str8(void);
NORETURN void		os_exit_process(int result);

u8*					os_thread_get_TIB(void);		// Thread Information Block (aka Thread Environment Block, or TEB)
u32					os_thread_get_ID(void);

typedef u32 (*ThreadProc)(void* userData);
ThreadHandle		os_thread_create(ThreadProc proc, void* userData);
int					os_thread_join(ThreadHandle* th, u32* returnValue);
void				os_thread_sleep(u32 milliSeconds);

void				os_critsec_init(CriticalSection* cs);
void				os_critsec_delete(CriticalSection* cs);
void				os_critsec_enter(CriticalSection* cs);
void				os_critsec_leave(CriticalSection* cs);

// NOTE: Ensure that this function does not call ASSERT, because ASSERT uses this function to output error strings.
void				os_output_debug_string(const char* buf);

void				os_console_write(const char* buf, int length, eConsoleTextColour colour);

void				os_log_last_error(void);

size_t				os_mem_get_page_size(void);
size_t				os_mem_get_base_granularity(void);

void*				os_mem_reserve(size_t byteCount, void* baseAddress);
void*				os_mem_commit(void* address, size_t byteCount);
bool				os_mem_decommit(void* address, size_t byteCount);
bool				os_mem_release(void* address);

void*				os_mem_set(void* ptr, u8 val, size_t byteCount);
void*				os_mem_zero(void* ptr, size_t byteCount);
void*				os_mem_cpy(void* RESTRICT dst, const void* RESTRICT src, size_t byteCount);

typedef enum eDefaultMessageBoxButton
{
	DMBB_ONE,
	DMBB_TWO,
	DMBB_THREE,

	DMBB_COUNT
} eDefaultMessageBoxButton;

int					os_message_box(const char* title, const char* msg, const char* buttonText1, const char* buttonText2, const char* buttonText3, eDefaultMessageBoxButton defaultButton);

#endif // CTH_OS_H_

