@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Example-BoundingBox.c -o obj\Example-BoundingBox.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Example-BoundingBox.asm -o obj\Example-BoundingBox.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Example-BoundingBox.xml -o bin\Example-BoundingBox.v32 || goto :failed
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