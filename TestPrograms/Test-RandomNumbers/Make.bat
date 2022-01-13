@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-RandomNumbers.c -o obj\Test-RandomNumbers.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Test-RandomNumbers.asm -o obj\Test-RandomNumbers.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-DiceStats.png -o obj\Texture-DiceStats.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-DiceRoll.wav -o obj\Sound-DiceRoll.vsnd || goto :failed
wav2vircon Sound-PageComplete.wav -o obj\Sound-PageComplete.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-RandomNumbers.xml -o bin\Test-RandomNumbers.v32 || goto :failed
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