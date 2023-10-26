@echo off

REM prepare the script to be run from another directory
pushd %~dp0

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import embedded data files
echo --------------------------
tiled2vircon maps\Level.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\VersusShooter.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\VersusShooter.asm -o obj\VersusShooter.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon textures\TextureGameplay.png  -o obj\TextureGameplay.vtex  || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicLevel.wav -o obj\MusicLevel.vsnd || goto :failed

wav2vircon sounds\SoundCountdown.wav      -o obj\SoundCountdown.vsnd      || goto :failed
wav2vircon sounds\SoundShoot.wav          -o obj\SoundShoot.vsnd          || goto :failed
wav2vircon sounds\SoundTankHit.wav        -o obj\SoundTankHit.vsnd        || goto :failed
wav2vircon sounds\SoundPressStart.wav     -o obj\SoundPressStart.vsnd     || goto :failed
wav2vircon sounds\SoundExplosionSmall.wav -o obj\SoundExplosionSmall.vsnd || goto :failed
wav2vircon sounds\SoundExplosionLarge.wav -o obj\SoundExplosionLarge.vsnd || goto :failed
wav2vircon sounds\SoundRespawn.wav        -o obj\SoundRespawn.vsnd        || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom VersusShooter.xml -o "bin\Versus Shooter.v32" || goto :failed
goto :succeeded

:failed
popd
echo.
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
popd
echo.
echo BUILD SUCCESSFUL
exit /b

@echo on