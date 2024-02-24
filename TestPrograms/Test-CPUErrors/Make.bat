@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-CPUErrors.c -o obj\Test-CPUErrors.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-CPUErrors.asm -o obj\Test-CPUErrors.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-CPUErrors.xml -o "bin\Test - CPU Errors.v32" || goto :failed
goto :succeeded

:failed
echo.
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
echo.
echo BUILD SUCCESSFUL
exit /b

@echo on