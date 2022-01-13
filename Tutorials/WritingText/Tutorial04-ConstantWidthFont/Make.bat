@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial04-ConstantWidthFont.c -o obj\Tutorial04-ConstantWidthFont.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial04-ConstantWidthFont.asm -o obj\Tutorial04-ConstantWidthFont.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Font11x16.png -o obj\Texture-Font11x16.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial04-ConstantWidthFont.xml -o bin\Tutorial04-ConstantWidthFont.v32 || goto :failed
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