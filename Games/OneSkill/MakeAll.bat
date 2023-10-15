@echo off
REM | Since program changes much more often than the rest,
REM | these scripts will separately process images, sounds
REM | and tilemaps so that we can choose whether to build
REM | everything (MakeAll.bat) or just update the previous
REM | build with any changes done to the code (Make.bat).
REM | For a first time build, MakeAll.bat must be used.

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import embedded data files
echo --------------------------

REM levels have 2 parts each
for /l %%x in (1,1,4) do (
    tiled2vircon maps\Level%%x-1.tmx -o obj\ || goto :failed
    tiled2vircon maps\Level%%x-2.tmx -o obj\ || goto :failed
)

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon textures\TextureTitle.png      -o obj\TextureTitle.vtex      || goto :failed
png2vircon textures\TextureGameplay.png   -o obj\TextureGameplay.vtex   || goto :failed
png2vircon textures\TextureCutscenes.png  -o obj\TextureCutscenes.vtex  || goto :failed
png2vircon textures\TextureFont11x16.png  -o obj\TextureFont11x16.vtex  || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicTitle.wav      -o obj\MusicTitle.vsnd      || goto :failed
wav2vircon music\MusicCutscenes.wav  -o obj\MusicCutscenes.vsnd  || goto :failed
wav2vircon music\MusicTutorial.wav   -o obj\MusicTutorial.vsnd   || goto :failed
wav2vircon music\MusicLevelDay.wav   -o obj\MusicLevelDay.vsnd   || goto :failed
wav2vircon music\MusicLevelNight.wav -o obj\MusicLevelNight.vsnd || goto :failed

wav2vircon sounds\SoundTitleSlide.wav    -o obj\SoundTitleSlide.vsnd    || goto :failed
wav2vircon sounds\SoundTitleLogo.wav     -o obj\SoundTitleLogo.vsnd     || goto :failed
wav2vircon sounds\SoundPressStart.wav    -o obj\SoundPressStart.vsnd    || goto :failed
wav2vircon sounds\SoundMenuMove.wav      -o obj\SoundMenuMove.vsnd      || goto :failed
wav2vircon sounds\SoundMenuAccept.wav    -o obj\SoundMenuAccept.vsnd    || goto :failed
wav2vircon sounds\SoundMenuCancel.wav    -o obj\SoundMenuCancel.vsnd    || goto :failed
wav2vircon sounds\SoundStoryLetter.wav   -o obj\SoundStoryLetter.vsnd   || goto :failed
wav2vircon sounds\SoundPause.wav         -o obj\SoundPause.vsnd         || goto :failed
wav2vircon sounds\SoundDeath.wav         -o obj\SoundDeath.vsnd         || goto :failed
wav2vircon sounds\SoundEnterDoor.wav     -o obj\SoundEnterDoor.vsnd     || goto :failed
wav2vircon sounds\SoundLevelClear.wav    -o obj\SoundLevelClear.vsnd    || goto :failed
wav2vircon sounds\SoundJump.wav          -o obj\SoundJump.vsnd          || goto :failed
wav2vircon sounds\SoundLand.wav          -o obj\SoundLand.vsnd          || goto :failed
wav2vircon sounds\SoundGolemWalk.wav     -o obj\SoundGolemWalk.vsnd     || goto :failed
wav2vircon sounds\SoundGolemDeath.wav    -o obj\SoundGolemDeath.vsnd    || goto :failed
wav2vircon sounds\SoundBirdFlap.wav      -o obj\SoundBirdFlap.vsnd      || goto :failed
wav2vircon sounds\SoundBirdDeath.wav     -o obj\SoundBirdDeath.vsnd     || goto :failed
wav2vircon sounds\SoundEnemyHit.wav      -o obj\SoundEnemyHit.vsnd      || goto :failed
wav2vircon sounds\SoundShoot.wav         -o obj\SoundShoot.vsnd         || goto :failed
wav2vircon sounds\SoundShotDestroyed.wav -o obj\SoundShotDestroyed.vsnd || goto :failed
wav2vircon sounds\SoundBoxDestroyed.wav  -o obj\SoundBoxDestroyed.vsnd  || goto :failed
wav2vircon sounds\SoundSpikesSwitch.wav  -o obj\SoundSpikesSwitch.vsnd  || goto :failed
wav2vircon sounds\SoundWandMagic.wav     -o obj\SoundWandMagic.vsnd     || goto :failed
wav2vircon sounds\SoundDoubleJump.wav    -o obj\SoundDoubleJump.vsnd    || goto :failed
wav2vircon sounds\SoundBootsLand.wav     -o obj\SoundBootsLand.vsnd     || goto :failed
goto :succeeded

:failed
echo.
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
REM Now call the make script to build the program and pack the ROM
call Make.bat
exit /b

@echo on