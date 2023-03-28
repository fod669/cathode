
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
call %cathode_dir%\scripts\cathode_build_settings.bat
if %errorlevel% neq 0 (goto:eof)

set src_files=main.c
set output_file=crt_tests.exe
set bin_dir=%proj_dir%\bin\%config%

:: Compile:
:: ============================================================================
set obj_dir=%bin_dir%\obj
mkdir %obj_dir% 2>NUL
%llvm_dir%\clang.exe %cathode_settings% %src_files% -o %obj_dir%\%output_file% -save-temps=obj
set compile_error_level=%errorlevel%
if %compile_error_level% neq 0 (goto:compile_error)
move /y %obj_dir%\*.exe %bin_dir% >NUL
move /y %obj_dir%\*.pdb %bin_dir% >NUL
echo Output: %bin_dir%\%output_file%
echo Build successful!
goto:eof

:compile_error
echo Build failed! Error code: %compile_error_level%
goto:eof

