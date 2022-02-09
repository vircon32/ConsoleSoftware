@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial-MotionBlur.c -o obj\Tutorial-MotionBlur.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-MotionBlur.asm -o obj\Tutorial-MotionBlur.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureMotionBlur.png -o obj\TextureMotionBlur.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-MotionBlur.xml -o bin\Tutorial-MotionBlur.v32 || goto :failed
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