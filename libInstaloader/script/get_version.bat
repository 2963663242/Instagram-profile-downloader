
setlocal enabledelayedexpansion
set SCRIPT_DIR=%cd%\
set VERSION_HPP_FILE=%SCRIPT_DIR%\version.h
set VERSION_FILE=%SCRIPT_DIR%\version

if exist "%VERSION_HPP_FILE%" (
	echo "del %VERSION_HPP_FILE%"
	del /Q "%VERSION_HPP_FILE%"
)

if exist "%VERSION_FILE%" (
	echo "del %VERSION_FILE%"
	del /Q "%VERSION_FILE%"
)



::git rev-parse --abbrev-ref HEAD获取当前分支名称
for /f %%i in ('git rev-parse --abbrev-ref HEAD') do (set BRANCH=%%i)

::git rev-list HEAD -n 1 获取当前分支最近一次提交的hash值
for /f %%i in ('git rev-list HEAD -n 1') do (set COMMITID=%%i)

::git rev-list HEAD --count 获取当前分支的提交次数
for /f %%i in ('git rev-list HEAD --count') do (set LOCALVER=%%i)

::将获取到的版本信息按照提交次数-分支首字母-hash值的前7个字符拼接起来
set VER=%LOCALVER%-%BRANCH:~0,1%-%COMMITID:~0,7%
set LIB_VER=1.0.0
set COMPANYNAME=Tenorshare

set temp=#ifndef _PROJECT_VERSION_H_
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define _PROJECT_VERSION_H_
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%

set temp=#ifndef PROJECT_VERSION_SYSTEM_FILE
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_VERSION_SYSTEM_FILE         "%LIB_VER%,%LOCALVER%"
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%

set temp=#ifndef PROJECT_VERSION_SYSTEM_PRODUCT
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_VERSION_SYSTEM_PRODUCT				 "%LIB_VER%,%LOCALVER%"
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%


set temp=#ifndef PROJECT_VERSION_DISPLAY_FILE
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_VERSION_DISPLAY_FILE				 "%LIB_VER%,%VER%"
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%

set temp=#ifndef PROJECT_VERSION_DISPLAY_PRODUCT
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_VERSION_DISPLAY_PRODUCT			"%LIB_VER%,%VER%"
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%


set temp=#ifndef PROJECT_OWNER_COMPANY_NAME
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_OWNER_COMPANY_NAME					"%COMPANYNAME%  CO.,Ltd."
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%
set temp=#ifndef PROJECT_OWNER_PRODUCT_NAME
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_OWNER_PRODUCT_NAME					"%COMPANYNAME% Product Library"
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%


set temp=#ifndef PROJECT_OWNER_LEGAL_COPYRIGHT
echo !temp!>>%VERSION_HPP_FILE%
set temp=#define PROJECT_OWNER_LEGAL_COPYRIGHT		"Copyright (C) 2007-2021 %COMPANYNAME% CO.,Ltd."
echo !temp!>>%VERSION_HPP_FILE%
set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
echo.>>%VERSION_HPP_FILE%


set temp=#endif
echo !temp!>>%VERSION_HPP_FILE%
