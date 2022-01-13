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
tiled2vircon maps\LevelTitle.tmx -o obj\ || goto :failed

REM levels with a leading 0 (1 to 9)
for /l %%x in (1,1,9) do (
    tiled2vircon maps\Level0%%x.tmx -o obj\ || goto :failed
)

REM rest of levels (10 to 30)
for /l %%x in (10,1,30) do (
    tiled2vircon maps\Level%%x.tmx -o obj\ || goto :failed
)

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon textures\TextureGame.png     -o obj\TextureGame.vtex     || goto :failed
png2vircon textures\TextureTextFont.png -o obj\TextureTextFont.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicTitle.wav      -o obj\MusicTitle.vsnd      || goto :failed
wav2vircon music\MusicWorld1.wav     -o obj\MusicWorld1.vsnd     || goto :failed
wav2vircon music\MusicWorld2.wav     -o obj\MusicWorld2.vsnd     || goto :failed
wav2vircon music\MusicWorld3.wav     -o obj\MusicWorld3.vsnd     || goto :failed
wav2vircon music\MusicWorldClear.wav -o obj\MusicWorldClear.vsnd || goto :failed
wav2vircon music\MusicCredits.wav    -o obj\MusicCredits.vsnd    || goto :failed
wav2vircon music\MusicEnding.wav     -o obj\MusicEnding.vsnd     || goto :failed
wav2vircon music\MusicGameOver.wav   -o obj\MusicGameOver.vsnd   || goto :failed

wav2vircon sounds\SoundJump.wav     -o obj\SoundJump.vsnd     || goto :failed
wav2vircon sounds\SoundLand.wav     -o obj\SoundLand.vsnd     || goto :failed
wav2vircon sounds\SoundCoin.wav     -o obj\SoundCoin.vsnd     || goto :failed
wav2vircon sounds\SoundOnOff.wav    -o obj\SoundOnOff.vsnd    || goto :failed
wav2vircon sounds\SoundSpring.wav   -o obj\SoundSpring.vsnd   || goto :failed
wav2vircon sounds\SoundGetKey.wav   -o obj\SoundGetKey.vsnd   || goto :failed
wav2vircon sounds\SoundOpenDoor.wav -o obj\SoundOpenDoor.vsnd || goto :failed
wav2vircon sounds\SoundGoal.wav     -o obj\SoundGoal.vsnd     || goto :failed
wav2vircon sounds\SoundDeath.wav    -o obj\SoundDeath.vsnd    || goto :failed
wav2vircon sounds\SoundPause.wav    -o obj\SoundPause.vsnd    || goto :failed
wav2vircon sounds\SoundStart.wav    -o obj\SoundStart.vsnd    || goto :failed
wav2vircon sounds\SoundHurry.wav    -o obj\SoundHurry.vsnd    || goto :failed
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