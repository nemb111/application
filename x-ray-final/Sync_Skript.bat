:: ++++++ Synchronisiere $(TargetDir)files mit $(SolutionDir)files +++++++++++++++++++++++++++++++
setlocal EnableDelayedExpansion

set SolutionDir=%1
set TargetDir=%2
set SolutionDir=%SolutionDir:"=%
set TargetDir=%TargetDir:"=%

:: delete leading and trailing whitespaces from SOURCE and TARGET
:: for /f "tokens=* delims= " %%I in ("%SolutionDir%") do set SOURCE=%%I
:: for /f "tokens=* delims= " %%I in ("%TargetDir%") do set TARGET=%%I
:: for /l %%I in (1,1,20) do if "!SolutionDir:~-1!"==" " set SOURCE=!SOURCE:~0,-1!
:: for /l %%I in (1,1,20) do if "!TargetDir:~-1!"==" " set TARGET=!TARGET:~0,-1!



::  this copys all files from %SolutionDir%files to %TargetDir%files
xcopy /d /c /i /s /y "%SolutionDir%files" "%TargetDir%files"
:: this lists all files in %TargetDir%files and puts them in a file called removefiles.txt
for /r %TargetDir%files %%F in (*.*) do (set string=%%F&&set modified=!string:%TargetDir%=!&&echo !modified!>>%TargetDir%removefiles.txt)
:: takes each entry in removefiles.txt and checks if it exists in source dir, if not delete them in target dir
for /F "tokens=1 delims=" %%I in (%TargetDir%removefiles.txt) do if not exist "%SolutionDir%%%I" del "%TargetDir%%%I"
del %TargetDir%removefiles.txt
:: deletes all empty folders
::for /f "usebackq delims=" %%d in (`"dir /ad/b/s %TargetDir% | sort /R"`) do rd "%%d"
:: --------------------------------------------------------------------------------------------------------------------