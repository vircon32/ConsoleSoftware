@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial-Shooter.c -o obj\Tutorial-Shooter.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-Shooter.asm -o obj\Tutorial-Shooter.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Textures\TextureBackground.png  -o obj\TextureBackground.vtex  || goto :failed
png2vircon Textures\TextureGameObjects.png -o obj\TextureGameObjects.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sounds\MusicGameplay.wav  -o obj\MusicGameplay.vsnd  || goto :failed
wav2vircon Sounds\SoundShoot.wav     -o obj\SoundShoot.vsnd     || goto :failed
wav2vircon Sounds\SoundExplosion.wav -o obj\SoundExplosion.vsnd || goto :failed
wav2vircon Sounds\SoundDeath.wav     -o obj\SoundDeath.vsnd     || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-Shooter.xml -o bin\Tutorial-Shooter.v32 || goto :failed
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