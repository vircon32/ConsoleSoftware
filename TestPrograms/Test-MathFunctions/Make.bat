@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-MathFunctions.c -o obj\Test-MathFunctions.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-MathFunctions.asm -o obj\Test-MathFunctions.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-MathFunctions.png -o obj\Texture-MathFunctions.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-MathFunctions.xml -o "bin\Test - Math functions.v32" || goto :failed
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