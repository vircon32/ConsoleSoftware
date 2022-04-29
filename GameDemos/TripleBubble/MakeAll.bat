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
echo Import the Tiled tile maps
echo --------------------------
tiled2vircon maps\ClassicLevels.tmx -o obj\ || goto :failed
tiled2vircon maps\TripleLevels.tmx  -o obj\ || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon textures\TextureTitle.png      -o obj\TextureTitle.vtex      || goto :failed
png2vircon textures\TextureMenus.png      -o obj\TextureMenus.vtex      || goto :failed
png2vircon textures\TextureGameplay.png   -o obj\TextureGameplay.vtex   || goto :failed
png2vircon textures\TextureCutscenes.png  -o obj\TextureCutscenes.vtex  || goto :failed
png2vircon textures\TextureCharacter1.png -o obj\TextureCharacter1.vtex || goto :failed
png2vircon textures\TextureCharacter2.png -o obj\TextureCharacter2.vtex || goto :failed
png2vircon textures\TextureCharacter3.png -o obj\TextureCharacter3.vtex || goto :failed
png2vircon textures\TextureWorld1.png     -o obj\TextureWorld1.vtex     || goto :failed
png2vircon textures\TextureWorld2.png     -o obj\TextureWorld2.vtex     || goto :failed
png2vircon textures\TextureWorld3.png     -o obj\TextureWorld3.vtex     || goto :failed
png2vircon textures\TextureWorld4.png     -o obj\TextureWorld4.vtex     || goto :failed
png2vircon textures\TextureWorld5.png     -o obj\TextureWorld5.vtex     || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicWorld1.wav -o obj\MusicWorld1.vsnd || goto :failed
wav2vircon music\MusicWorld2.wav -o obj\MusicWorld2.vsnd || goto :failed
wav2vircon music\MusicWorld3.wav -o obj\MusicWorld3.vsnd || goto :failed
wav2vircon music\MusicWorld4.wav -o obj\MusicWorld4.vsnd || goto :failed
wav2vircon music\MusicWorld5.wav -o obj\MusicWorld5.vsnd || goto :failed

wav2vircon sounds\SoundTitleBubble.wav    -o obj\SoundTitleBubble.vsnd    || goto :failed
wav2vircon sounds\SoundTitleFanfare.wav   -o obj\SoundTitleFanfare.vsnd   || goto :failed
wav2vircon sounds\SoundTitleStart.wav     -o obj\SoundTitleStart.vsnd     || goto :failed
wav2vircon sounds\SoundMenuMove.wav       -o obj\SoundMenuMove.vsnd       || goto :failed
wav2vircon sounds\SoundMenuAccept.wav     -o obj\SoundMenuAccept.vsnd     || goto :failed
wav2vircon sounds\SoundMenuCancel.wav     -o obj\SoundMenuCancel.vsnd     || goto :failed
wav2vircon sounds\SoundLauncherMove.wav   -o obj\SoundLauncherMove.vsnd   || goto :failed
wav2vircon sounds\SoundBubbleShoot.wav    -o obj\SoundBubbleShoot.vsnd    || goto :failed
wav2vircon sounds\SoundBubbleBounce.wav   -o obj\SoundBubbleBounce.vsnd   || goto :failed
wav2vircon sounds\SoundBubblePlaced.wav   -o obj\SoundBubblePlaced.vsnd   || goto :failed
wav2vircon sounds\SoundBubblesBurst.wav   -o obj\SoundBubblesBurst.vsnd   || goto :failed
wav2vircon sounds\SoundBurstFanfare.wav   -o obj\SoundBurstFanfare.vsnd   || goto :failed
wav2vircon sounds\SoundChangeLauncher.wav -o obj\SoundChangeLauncher.vsnd || goto :failed
wav2vircon sounds\SoundCountdown.wav      -o obj\SoundCountdown.vsnd      || goto :failed
wav2vircon sounds\SoundCeilingDrop.wav    -o obj\SoundCeilingDrop.vsnd    || goto :failed
wav2vircon sounds\SoundPause.wav          -o obj\SoundPause.vsnd          || goto :failed
wav2vircon sounds\SoundLevelClear.wav     -o obj\SoundLevelClear.vsnd     || goto :failed
wav2vircon sounds\SoundDeath.wav          -o obj\SoundDeath.vsnd          || goto :failed
wav2vircon sounds\SoundReady.wav          -o obj\SoundReady.vsnd          || goto :failed
wav2vircon sounds\SoundClear.wav          -o obj\SoundClear.vsnd          || goto :failed
wav2vircon sounds\SoundFailed.wav         -o obj\SoundFailed.vsnd         || goto :failed
wav2vircon sounds\SoundCombo.wav          -o obj\SoundCombo.vsnd          || goto :failed
wav2vircon sounds\SoundComboNice.wav      -o obj\SoundComboNice.vsnd      || goto :failed
wav2vircon sounds\SoundComboGreat.wav     -o obj\SoundComboGreat.vsnd     || goto :failed
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