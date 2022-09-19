@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Example-Vector2D.c -o obj\Example-Vector2D.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Example-Vector2D.asm -o obj\Example-Vector2D.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Example-Vector2D.xml -o bin\Example-Vector2D.v32 || goto :failed
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