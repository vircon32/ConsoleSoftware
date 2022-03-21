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
tiled2vircon maps\Level01.tmx -o obj\ || goto :failed
tiled2vircon maps\Level02.tmx -o obj\ || goto :failed
tiled2vircon maps\Level03.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\Main.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Main.asm -o obj\Main.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon textures\TextureTitle.png     -o obj\TextureTitle.vtex     || goto :failed
png2vircon textures\TextureStory.png     -o obj\TextureStory.vtex     || goto :failed
png2vircon textures\TextureGameplay.png  -o obj\TextureGameplay.vtex  || goto :failed
png2vircon textures\TextureBGLevel1.png  -o obj\TextureBGLevel1.vtex  || goto :failed
png2vircon textures\TextureBGLevel2.png  -o obj\TextureBGLevel2.vtex  || goto :failed
png2vircon textures\TextureBGLevel3.png  -o obj\TextureBGLevel3.vtex  || goto :failed
png2vircon textures\TextureFont11x16.png -o obj\TextureFont11x16.vtex || goto :failed
png2vircon textures\TextureFont22x32.png -o obj\TextureFont22x32.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicTitle.wav    -o obj\MusicTitle.vsnd    || goto :failed
wav2vircon music\MusicStory.wav    -o obj\MusicStory.vsnd    || goto :failed
wav2vircon music\MusicGameplay.wav -o obj\MusicGameplay.vsnd || goto :failed

wav2vircon sounds\SoundStartGame.wav      -o obj\SoundStartGame.vsnd      || goto :failed
wav2vircon sounds\SoundStoryLetter.wav    -o obj\SoundStoryLetter.vsnd    || goto :failed
wav2vircon sounds\SoundJump.wav           -o obj\SoundJump.vsnd           || goto :failed
wav2vircon sounds\SoundLand.wav           -o obj\SoundLand.vsnd           || goto :failed
wav2vircon sounds\SoundShoot.wav          -o obj\SoundShoot.vsnd          || goto :failed
wav2vircon sounds\SoundDeath.wav          -o obj\SoundDeath.vsnd          || goto :failed
wav2vircon sounds\SoundLevelClear.wav     -o obj\SoundLevelClear.vsnd     || goto :failed
wav2vircon sounds\SoundPause.wav          -o obj\SoundPause.vsnd          || goto :failed
wav2vircon sounds\SoundEnemyShoot.wav     -o obj\SoundEnemyShoot.vsnd     || goto :failed
wav2vircon sounds\SoundEnemyHit.wav       -o obj\SoundEnemyHit.vsnd       || goto :failed
wav2vircon sounds\SoundEnemyDeath.wav     -o obj\SoundEnemyDeath.vsnd     || goto :failed
wav2vircon sounds\SoundExplosionSmall.wav -o obj\SoundExplosionSmall.vsnd || goto :failed
wav2vircon sounds\SoundExplosionLarge.wav -o obj\SoundExplosionLarge.vsnd || goto :failed
wav2vircon sounds\SoundMissile.wav        -o obj\SoundMissile.vsnd        || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom FierceSoldier.xml -o bin\FierceSoldier.v32 || goto :failed
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