@echo off
if "%*"=="" (
	set /a a=10000
) else (
	set /a a=%1
)
set /a result=%RANDOM% %% %a%
echo %result%