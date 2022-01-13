@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial06-TextAlignment.c -o obj\Tutorial06-TextAlignment.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial06-TextAlignment.asm -o obj\Tutorial06-TextAlignment.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Font22x32.png -o obj\Texture-Font22x32.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial06-TextAlignment.xml -o bin\Tutorial06-TextAlignment.v32 || goto :failed
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