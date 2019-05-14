@echo off
if (%1==( %0 10
for %%f in (1 2 3) do matrix%%f %1 %1.%%f
for %%f in (1 2 3) do fc %1.res %1.%%f
