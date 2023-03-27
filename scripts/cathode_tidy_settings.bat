
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
set build_debug=-DCTH_DEBUG=1
set build_release=-DCTH_RELEASE=1
set build_dist=-DCTH_DIST=1

:: Subsystem settings:
:: ============================================================================
set sub_console=-DCTH_CONSOLE=1 -DCTH_UI=0
set sub_windows=-DCTH_CONSOLE=0 -DCTH_UI=1
set sub_hybrid=-DCTH_CONSOLE=1 -DCTH_UI=1

:: Common compiler settings:
:: ============================================================================
set cth_dir=%~dp0%..
set common_flags=-I%cth_dir%

:: CRT settings:
:: ============================================================================
set crt_flags=-D_VC_NODEFAULTLIB

setlocal EnableDelayedExpansion
set cathode_settings=%cth_dir%\src\cathode.c -header-filter=.* -checks="-clang-diagnostic-format-invalid-specifier" -- !build_%config%! !sub_%subsystem%! %crt_flags% %common_flags%
endlocal & set cathode_settings=%cathode_settings%
exit /b 0

:bad_config
echo Invalid config: '%config%' (Use: debug, release, dist)
exit /b 1

:bad_subsystem
echo Invalid subsystem option: '%subsystem%' (Use: console, windows, hybrid)
exit /b 2

