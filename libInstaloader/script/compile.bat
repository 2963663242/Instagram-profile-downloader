rem **************************************************************
rem * Script to compile the solution of libInstaloader *
rem **************************************************************


:: 工程目录
set WORKSPACE_INSTALOADER=%~dp0\..

set SCRIPT_INSTALOADER="%WORKSPACE_INSTALOADER%\script"

:: 打包机的编译程序目录

set COMPILE_TOOL="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.com"
set DEVENV_EXE="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe"
if not exist %COMPILE_TOOL% (
	set COMPILE_TOOL="D:\app\vs2019\Common7\IDE\devenv.com"
	set DEVENV_EXE="D:\app\vs2019\Common7\IDE\devenv.exe"
	echo "Begin to compile on local machine"
)else (
	echo "Begin to compile on remote machine"
)

:: 解决方案路径
set SOLUTION_INSTALOADER="%WORKSPACE_INSTALOADER%\workspace\libInstaloader.sln"

:: 编译输出文件路径
set COMPILE_INSTALOADER=%WORKSPACE_INSTALOADER%\build\

:: 编译日志文件路径
set COMPILE_LOG_INSTALOADER="%COMPILE_INSTALOADER%\compile.log"

if exist %COMPILE_LOG_INSTALOADER% (
	echo "remove file: %COMPILE_LOG_INSTALOADER%"
	del %COMPILE_LOG_INSTALOADER%
)

if exist %COMPILE_INSTALOADER% (
	echo "remove directory: %COMPILE_INSTALOADER%"
	rd /s /Q %COMPILE_INSTALOADER%
)

if not exist %COMPILE_INSTALOADER% (
	echo "make directory: %COMPILE_INSTALOADER%"
	mkdir %COMPILE_INSTALOADER%
)

echo 'Compile Enter ....\r\n'

if not exist %COMPILE_TOOL% (	
	echo 'Compile Tool Error!\r\n'
	goto LEAVE_HANDLE
)

%COMPILE_TOOL% %SOLUTION_INSTALOADER% /build "Release" /Project "libInstaloader" /ProjectConfig "Release|x64"  /Out %COMPILE_LOG_INSTALOADER%


:LEAVE_HANDLE
echo 'Compile Leave ....\r\n'