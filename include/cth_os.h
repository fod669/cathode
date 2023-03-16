
#pragma once
#ifndef CTH_OS_H_
#define CTH_OS_H_

typedef struct _CritSec CritSec;

typedef enum eConsoleTextColour
{
	CTC_GREEN,
	CTC_YELLOW,
	CTC_RED,
	CTC_RED_BACKGROUND,

	CTC_COUNT
} eConsoleTextColour;

str8				os_get_command_line_args_str8(void);
NORETURN void		os_exit_process(int result);

u32					os_thread_get_ID(void);

void				os_critsec_init(CritSec* cs);
void				os_critsec_delete(CritSec* cs);
void				os_critsec_enter(CritSec* cs);
void				os_critsec_leave(CritSec* cs);

void				os_output_debug_string(const char* buf);

void				os_console_write(const char* buf, int length, eConsoleTextColour colour);

void				os_log_last_error(void);

#endif // CTH_OS_H_

