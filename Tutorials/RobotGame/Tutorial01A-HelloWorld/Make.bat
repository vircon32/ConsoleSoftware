@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial01A-HelloWorld.c -o obj\Tutorial01A-HelloWorld.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial01A-HelloWorld.asm -o obj\Tutorial01A-HelloWorld.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial01A-HelloWorld.xml -o bin\Tutorial01A-HelloWorld.v32 || goto :failed
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