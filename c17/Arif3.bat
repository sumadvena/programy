@echo off
if (%1==( %0 Arif3
if (%2==( if (%PROCESSOR_ARCHITECTURE%==(AMD64 %0 %1 64
if not (%2==( set e=0%e%
if (%MHZ%==( set MHZ=0
Arifexp%2 -high -size -time -mhz %MHZ% -rep 10%e% %1 >%1.wrk
if errorlevel 1 pause
if exist %1.res fc /3 %1.res %1.wrk
if not exist %1.res copy %1.wrk %1.res
pause
