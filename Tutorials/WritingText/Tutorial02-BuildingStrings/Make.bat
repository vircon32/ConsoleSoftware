@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial02-BuildingStrings.c -o obj\Tutorial02-BuildingStrings.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial02-BuildingStrings.asm -o obj\Tutorial02-BuildingStrings.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial02-BuildingStrings.xml -o bin\Tutorial02-BuildingStrings.v32 || goto :failed
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