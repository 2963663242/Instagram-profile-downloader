rem **************************************************************
rem * Script to Package the solution of libInstaloader *
rem **************************************************************

:: 工程目录
set WORKSPACE_PACKAGE_INSTALOADER=%~dp0\..

:: 脚本目录
set SCRIPT_PACKAGE_INSTALOADER="%WORKSPACE_PACKAGE_INSTALOADER%\script"

::打包目录
set PACKAGE_TIME=%hh%%time:~3,2%%time:~6,2%
set PACKAGE_DATE=%date:~,4%%date:~5,2%%date:~8,2%

set PACKAGE_INSTALOADER_EX="%WORKSPACE_PACKAGE_INSTALOADER%\package"
set PACKAGE_INSTALOADER="%PACKAGE_INSTALOADER_EX%\%PACKAGE_TIME%_%PACKAGE_DATE%"
set PACKAGE_BIN_INSTALOADER="%PACKAGE_INSTALOADER%\bin"
set PACKAGE_LIB_INSTALOADER="%PACKAGE_INSTALOADER%\lib"
set PACKAGE_INCLUDE_INSTALOADER="%PACKAGE_INSTALOADER%\include"
set PACKAGE_SYMBOL_INSTALOADER="%PACKAGE_INSTALOADER%\symbol"
set PACKAGE_DOC_INSTALOADER="%PACKAGE_INSTALOADER%\doc"

::编译输出目录
set COMPILE_OUT_INSTALOADER="%WORKSPACE_PACKAGE_INSTALOADER%\build\bin\x64\Release"
set COMPILE_LOG_INSTALOADER="%WORKSPACE_PACKAGE_INSTALOADER%\build\compile.log"


if exist %PACKAGE_INSTALOADER_EX% (
	echo 'remove package \r\n'
	rd /s /Q %PACKAGE_INSTALOADER_EX%
)

::调用编译脚本
cd "%SCRIPT_PACKAGE_INSTALOADER%"
call compile.bat

::判断编译脚本是否成功

if not exist "%COMPILE_OUT_INSTALOADER%" (	
	echo 'Compile Error!\r\n'
	goto LEAVE_PACKAGE_INSTALOADER
)

::创建打包文件夹
if not exist %PACKAGE_INSTALOADER% (
	echo "make directory: %PACKAGE_INSTALOADER%"
	mkdir %PACKAGE_INSTALOADER%
)

if not exist %PACKAGE_BIN_INSTALOADER% (
	echo "make directory: %PACKAGE_BIN_INSTALOADER%"
	mkdir "%PACKAGE_BIN_INSTALOADER%"
	mkdir "%PACKAGE_BIN_INSTALOADER%\winBin"
)
if not exist %PACKAGE_LIB_INSTALOADER% (
	echo "make directory: %PACKAGE_LIB_INSTALOADER%"
	mkdir %PACKAGE_LIB_INSTALOADER%
)
if not exist %PACKAGE_INCLUDE_INSTALOADER% (
	echo "make directory: %PACKAGE_INCLUDE_INSTALOADER%"
	mkdir %PACKAGE_INCLUDE_INSTALOADER%
)
if not exist %PACKAGE_SYMBOL_INSTALOADER% (
	echo "make directory: %PACKAGE_SYMBOL_INSTALOADER%"
	mkdir %PACKAGE_SYMBOL_INSTALOADER%
)

if not exist %PACKAGE_DOC_INSTALOADER% (
	echo "make directory: %PACKAGE_DOC_INSTALOADER%"
	mkdir %PACKAGE_DOC_INSTALOADER%
)

::将文件打包进打包文件夹
echo 'Package bin ....\r\n'
xcopy "%COMPILE_OUT_INSTALOADER%\*.dll" "%PACKAGE_BIN_INSTALOADER%" /F /Y
xcopy "%COMPILE_OUT_INSTALOADER%\winBin" "%PACKAGE_BIN_INSTALOADER%\winBin" /F /Y

echo 'Package inlcude ....\r\n'
xcopy "%COMPILE_OUT_INSTALOADER%\*.h" "%PACKAGE_INCLUDE_INSTALOADER%" /F /Y

echo 'Package lib ....\r\n'
xcopy "%COMPILE_OUT_INSTALOADER%\libInstaloader.lib" "%PACKAGE_LIB_INSTALOADER%" /F /Y

echo 'Package symbol ....\r\n'
xcopy "%COMPILE_OUT_INSTALOADER%\libInstaloader.pdb" "%PACKAGE_SYMBOL_INSTALOADER%" /E /F /Y

echo 'Package doc ....\r\n'
xcopy "%WORKSPACE_PACKAGE_INSTALOADER%\doc" "%PACKAGE_DOC_INSTALOADER%" /E /F /Y

echo 'Package log ....\r\n'
xcopy "%COMPILE_LOG_INSTALOADER%" "%PACKAGE_INSTALOADER%"  /F /Y


:LEAVE_PACKAGE_INSTALOADER
echo 'Compile Leave ....\r\n'