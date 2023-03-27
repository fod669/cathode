
@echo off
setlocal

set proj_dir=%~dp0%
set proj_dir=%proj_dir:~0,-1%

:: Build configuration [debug, release, dist]
set config=debug

:: Subsystem [console, windows, hybrid]
set subsystem=hybrid

set cathode_dir=%proj_dir%\..\..
call %cathode_dir%\scripts\global_settings.bat
call %cathode_dir%\scripts\cathode_tidy_settings.bat
if %errorlevel% neq 0 (goto:eof)

set src_files=main.c

:: Compile:
:: ============================================================================
%llvm_dir%\clang-tidy.exe %src_files% %cathode_settings%

