
#pragma once
#ifndef CTH_OS_H_
#define CTH_OS_H_

str8				os_get_command_line_args_str8(void);
NORETURN void		os_exit_process(int result);

u32					os_thread_get_ID(void);

#endif // CTH_OS_H_

