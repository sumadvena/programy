@echo off
if (%1==( %0 Millrand
set r=
if (%PROCESSOR_ARCHITECTURE%==(AMD64 set r=64
if not exist %1.bad goto do
echo FAIL occurs during the previous test run
echo Look for NO MATCH in %1.bad or for error code if %1.res
choice /m "Continue the test?"
if not errorlevel 2 if errorlevel 1 goto do
goto end
:do
:loop
echo:
echo 1000000001+100* >%1.1
Arifexp%r% -idle -rand %1 >>%1.1
if errorlevel 1 goto e1
echo:
Arifexp%r% -idle %1.1 >%1.2
if errorlevel 1 goto e2
echo:
Miller%r% -idle -scan 50 %1.2 >%1.wrk
if errorlevel 255 goto e3
if errorlevel 10 goto e
if not errorlevel 0 goto e3
echo>>%1.res %ERRORLEVEL%
goto loop
:e
echo Incorrect result(s) detected
echo Look for NO MATCH in %1.bad
:e3
echo:
echo Program returns error code %ERRORLEVEL%
echo>>%1.res %ERRORLEVEL% (failed)
copy /b %1.wrk %1.bad >nul
:e2
copy /b %1.2 %1.b2 >nul
:e1
copy /b %1.1 %1.b1 >nul
echo FAIL!
:end
pause
