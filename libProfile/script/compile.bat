rem **************************************************************
rem * Script to Package the solution of libProfile *
rem **************************************************************

echo "set global path"
set LIBPROFILE_WORKSPACE=%~dp0\..
set LIBPROFILE_SRC=%LIBPROFILE_WORKSPACE%\src
set LIBPROFILE_SCRIPT=%LIBPROFILE_WORKSPACE%\script
set LIBPROFILE_BUILD=%LIBPROFILE_WORKSPACE%\build

echo "choice package way"
cd %LIBPROFILE_SRC% 
if "%1"=="" (
	echo "Packaging to a folder"
) else (
	echo "Packaging to a file"
) 



echo "pip install package..."
pip install -r %LIBPROFILE_WORKSPACE%\resource\requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

echo "packaging ..."
pyinstaller %1 -i %LIBPROFILE_WORKSPACE%\resource\HitPawVideoConverter.ico -y main.py --distpath  %LIBPROFILE_BUILD%\bin --workpath %LIBPROFILE_BUILD%\obj --name tupepaw
rem pyinstaller %1 -i %LIBPROFILE_WORKSPACE%\resource\HitPawVideoConverter.ico -y insExtractor.py --distpath  %LIBPROFILE_BUILD%\bin --workpath %LIBPROFILE_BUILD%\obj --clean

if %errorlevel%==0 (
	echo pyinstaller successfully
) else (
	rd /Q /S %LIBPROFILE_SRC%\pyinstaller
	git clone https://gitee.com/miao123456miao/pyinstaller.git
	if %errorlevel%==0 (
		python pyinstaller\PyInstaller\__main__.py %1 -i %LIBPROFILE_WORKSPACE%\resource\HitPawVideoConverter.ico -y main.py --distpath  %LIBPROFILE_BUILD%\bin --workpath %LIBPROFILE_BUILD%\obj --name tupepaw
	) else (
		package error !!!
	)
)

cd %LIBPROFILE_SCRIPT%
