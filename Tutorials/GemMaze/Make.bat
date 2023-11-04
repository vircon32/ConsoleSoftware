@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\Tutorial-GemMaze.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-GemMaze.asm -o obj\Tutorial-GemMaze.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon textures\TextureGemMaze.png -o obj\TextureGemMaze.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon sounds\SoundStart.wav          -o obj\SoundStart.vsnd          || goto :failed
wav2vircon sounds\SoundCollectRedGem.wav  -o obj\SoundCollectRedGem.vsnd  || goto :failed
wav2vircon sounds\SoundCollectBlueGem.wav -o obj\SoundCollectBlueGem.vsnd || goto :failed
wav2vircon sounds\SoundCollectTime.wav    -o obj\SoundCollectTime.vsnd    || goto :failed
wav2vircon sounds\SoundDeath.wav          -o obj\SoundDeath.vsnd          || goto :failed
wav2vircon sounds\SoundTimeOut.wav        -o obj\SoundTimeOut.vsnd        || goto :failed
wav2vircon sounds\SoundGoal.wav           -o obj\SoundGoal.vsnd           || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-GemMaze.xml -o bin\Tutorial-GemMaze.v32 || goto :failed
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