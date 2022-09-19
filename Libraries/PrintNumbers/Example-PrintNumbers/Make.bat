@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Example-PrintNumbers.c -o obj\Example-PrintNumbers.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Example-PrintNumbers.asm -o obj\Example-PrintNumbers.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Example-PrintNumbers.xml -o bin\Example-PrintNumbers.v32 || goto :failed
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