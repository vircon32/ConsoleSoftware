@echo off

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\SimpleTennis.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\SimpleTennis.asm -o obj\SimpleTennis.vprg || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-SimpleTennis.png -o obj\Texture-SimpleTennis.vtex || goto :failed
png2vircon TextureFont11x16.png -o obj\TextureFont11x16.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundHitPaddle.wav -o obj\SoundHitPaddle.vsnd || goto :failed
wav2vircon SoundHitWall.wav -o obj\SoundHitWall.vsnd || goto :failed
wav2vircon SoundScore.wav -o obj\SoundScore.vsnd || goto :failed
wav2vircon SoundGameEnd.wav -o obj\SoundGameEnd.vsnd || goto :failed
wav2vircon SoundMenu.wav -o obj\SoundMenu.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom SimpleTennis.xml -o bin\SimpleTennis.v32 || goto :failed
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