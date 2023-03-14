
@echo off

set llvm_dir="C:\Program Files\LLVM\bin"

set proj_dir=%~dp0%
set proj_dir=%proj_dir:~0,-1%

set cathode_dir=%proj_dir%\..\..

:: Default build configuration [debug, release, dist]
set config=debug

:: Console app [yes, no]
set console=yes

:: Set the source files to compile
set src_files=main.c

:: Executable name and output directory:
:: ============================================================================
set output_file=hello_world.exe
set bin_dir=%proj_dir%\bin\%config%

:: Common compiler settings:
:: ============================================================================
set everything_test=-Weverything -Wno-missing-prototypes
set common_flags=-Werror -Wall -Wextra -Wpedantic -lOneCore.lib -I%cathode_dir%
set enabled_warnings=-Wshadow -Wvla -Wstrict-prototypes
set disabled_warnings=-Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments -Wno-format-invalid-specifier -Wno-language-extension-token

:: Buld configuration settings:
:: ============================================================================
set options_debug=-g -O0 -DCTH_DEBUG=1
set options_release=-g -flto -O2 -DCTH_RELEASE=1
set options_dist=-flto -O2 -DCTH_DIST=1

:: CRT settings:
:: ============================================================================
set stack_size=1048576
set stack_options=-mstack-probe-size=%stack_size% --for-linker=-stack:%stack_size%,%stack_size%
set crt_flags=-nostdlib -lkernel32 -D_VC_NODEFAULTLIB %stack_options% --for-linker=-entry:crt_entry

:: Subsystem settings:
:: ============================================================================
set console_yes=--for-linker=-subsystem:console -DCTH_CONSOLE=1
set console_no=--for-linker=-subsystem:windows -DCTH_CONSOLE=0

:: Compile:
:: ============================================================================
echo [Build: %config%, Console: %console%]
setlocal EnableDelayedExpansion
set all_options=%common_flags% %enabled_warnings% %disabled_warnings% %crt_flags% !options_%config%! !console_%console%!
setlocal DisableDelayedExpansion
mkdir %bin_dir% 2>NUL
%llvm_dir%\clang.exe -fuse-ld=lld-link %all_options% %src_files% -o %bin_dir%\%output_file% -save-temps=obj
set compile_error_level=%errorlevel%
if %compile_error_level% neq 0 (goto:compile_error)
echo Output: %bin_dir%\%output_file%
echo Build successful!
goto:eof

:compile_error
echo Build failed! Error code: %compile_error_level%
goto:eof
