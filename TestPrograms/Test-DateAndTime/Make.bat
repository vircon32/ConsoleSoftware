@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-DateAndTime.c -o obj\Test-DateAndTime.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-DateAndTime.asm -o obj\Test-DateAndTime.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Clock.png -o obj\Texture-Clock.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-TicTac.wav -o obj\Sound-TicTac.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-DateAndTime.xml -o bin\Test-DateAndTime.v32 || goto :failed
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