@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial03-CustomNumbers.c -o obj\Tutorial03-CustomNumbers.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial03-CustomNumbers.asm -o obj\Tutorial03-CustomNumbers.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Numbers.png -o obj\Texture-Numbers.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial03-CustomNumbers.xml -o bin\Tutorial03-CustomNumbers.v32 || goto :failed
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