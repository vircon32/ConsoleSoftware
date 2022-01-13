@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial05-VariableWidthFont.c -o obj\Tutorial05-VariableWidthFont.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial05-VariableWidthFont.asm -o obj\Tutorial05-VariableWidthFont.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Font22x32.png -o obj\Texture-Font22x32.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial05-VariableWidthFont.xml -o bin\Tutorial05-VariableWidthFont.v32 || goto :failed
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