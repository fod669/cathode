
#pragma once
#ifndef CTH_OS_H_
#define CTH_OS_H_

typedef struct CritSec CritSec;

typedef enum eConsoleTextColour
{
	CTC_GREEN,
	CTC_YELLOW,
	CTC_RED,
	CTC_RED_BACKGROUND,

	CTC_COUNT
} eConsoleTextColour;

str8_const			os_get_command_line_args_str8(void);
NORETURN void		os_exit_process(int result);

u8*					os_thread_get_TIB(void);		// Thread Information Block
u32					os_thread_get_ID(void);

void				os_critsec_init(CritSec* cs);
void				os_critsec_delete(CritSec* cs);
void				os_critsec_enter(CritSec* cs);
void				os_critsec_leave(CritSec* cs);

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

