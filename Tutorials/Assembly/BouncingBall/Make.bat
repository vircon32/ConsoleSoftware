@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Assemble the ASM code
echo --------------------------
assemble TutorialASM-BouncingBall.asm -o obj\TutorialASM-BouncingBall.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureBall.png -o obj\TextureBall.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundBounce.wav -o obj\SoundBounce.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom TutorialASM-BouncingBall.xml -o bin\TutorialASM-BouncingBall.v32 || goto :failed
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