
echo.
echo [Build: %config%, Subsystem: %subsystem%]
echo.

:: The following variables must be set in the calling batch file.
:: set config=[debug, release, dist]
:: set subsystem=[console, windows, hybrid]

if not defined config (echo Error: 'config' variable not defined! & goto:bad_config)
if not defined subsystem (echo Error: 'subsystem' variable not defined! & goto:bad_subsystem)

if [%config%] neq [debug] (if [%config%] neq [release] (if [%config%] neq [dist] (goto:bad_config)))
if [%subsystem%] neq [console] (if [%subsystem%] neq [windows] (if [%subsystem%] neq [hybrid] (goto:bad_subsystem)))

:: Buld configuration settings:
:: ============================================================================
set build_debug=-g -O0 -DCTH_DEBUG=1
set build_release=-g -flto -O2 -DCTH_RELEASE=1
set build_dist=-flto -O2 -DCTH_DIST=1

:: Subsystem settings:
:: ============================================================================
set sub_console=--for-linker=-subsystem:console -DCTH_CONSOLE=1 -DCTH_UI=0
set sub_windows=--for-linker=-subsystem:windows -DCTH_CONSOLE=0 -DCTH_UI=1
set sub_hybrid=--for-linker=-subsystem:console -DCTH_CONSOLE=1 -DCTH_UI=1

:: Common compiler settings:
:: ============================================================================
set cth_dir=%~dp0%..
set everything_test=-Weverything -Wno-missing-prototypes
set common_flags=-fuse-ld=lld-link -Werror -Wall -Wextra -Wpedantic -lOneCore.lib %cth_dir%\src\cathode.c -I%cth_dir%
set enabled_warnings=-Wshadow -Wvla -Wstrict-prototypes
set disabled_warnings=-Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments -Wno-format-invalid-specifier -Wno-language-extension-token

:: CRT settings:
:: ============================================================================
set stack_size=1048576
set stack_options=-mstack-probe-size=%stack_size% --for-linker=-stack:%stack_size%,%stack_size%
set crt_flags=-nostdlib -lkernel32 -D_VC_NODEFAULTLIB %stack_options% --for-linker=-entry:crt_entry

setlocal EnableDelayedExpansion
set cathode_settings=!build_%config%! !sub_%subsystem%! %common_flags% %enabled_warnings% %disabled_warnings% %crt_flags%
endlocal & set cathode_settings=%cathode_settings%
exit /b 0

:bad_config
echo Invalid config: '%config%' (Use: debug, release, dist)
exit /b 2

:bad_subsystem
echo Invalid subsystem option: '%subsystem%' (Use: console, windows, hybrid)
exit /b 3

