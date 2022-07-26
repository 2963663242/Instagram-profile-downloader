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

echo "packaging ..."
pyinstaller %1 -i %LIBPROFILE_WORKSPACE%\resource\HitPawVideoConverter.ico -y insExtractor.py --distpath  %LIBPROFILE_BUILD%\bin --workpath %LIBPROFILE_BUILD%\obj 
rem pyinstaller %1  -y insExtractor.py --distpath  %LIBPROFILE_BUILD%\bin --workpath %LIBPROFILE_BUILD%\obj --clean
cd %LIBPROFILE_SCRIPT%
