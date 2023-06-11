@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import the Tiled tile maps
echo --------------------------
tiled2vircon maps\MapTurtle2.tmx   -o obj\ || goto :failed
tiled2vircon maps\MapPyramids2.tmx -o obj\ || goto :failed
tiled2vircon maps\MapCastle2.tmx   -o obj\ || goto :failed
tiled2vircon maps\MapBridge2.tmx   -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\Mahjong.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Mahjong.asm -o obj\Mahjong.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureMahjong.png -o obj\TextureMahjong.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicTitle.wav         -o obj\MusicTitle.vsnd        || goto :failed
wav2vircon music\MusicGameplay.wav      -o obj\MusicGameplay.vsnd     || goto :failed

wav2vircon sounds\SoundTitleLogo.wav    -o obj\SoundTitleLogo.vsnd    || goto :failed
wav2vircon sounds\SoundPressStart.wav   -o obj\SoundPressStart.vsnd   || goto :failed
wav2vircon sounds\SoundMenuMove.wav     -o obj\SoundMenuMove.vsnd     || goto :failed
wav2vircon sounds\SoundMenuAccept.wav   -o obj\SoundMenuAccept.vsnd   || goto :failed
wav2vircon sounds\SoundMenuCancel.wav   -o obj\SoundMenuCancel.vsnd   || goto :failed
wav2vircon sounds\SoundWin.wav          -o obj\SoundWin.vsnd          || goto :failed
wav2vircon sounds\SoundQuitGame.wav     -o obj\SoundQuitGame.vsnd     || goto :failed
wav2vircon sounds\SoundSelectTile.wav   -o obj\SoundSelectTile.vsnd   || goto :failed
wav2vircon sounds\SoundUndoSelect.wav   -o obj\SoundUndoSelect.vsnd   || goto :failed
wav2vircon sounds\SoundMatchTiles.wav   -o obj\SoundMatchTiles.vsnd   || goto :failed
wav2vircon sounds\SoundUnmatchTiles.wav -o obj\SoundUnmatchTiles.vsnd || goto :failed
wav2vircon sounds\SoundHighlight.wav    -o obj\SoundHighlight.vsnd    || goto :failed
wav2vircon sounds\SoundTileCollide.wav  -o obj\SoundTileCollide.vsnd  || goto :failed
wav2vircon sounds\SoundWrongMove.wav    -o obj\SoundWrongMove.vsnd    || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Mahjong.xml -o bin\Mahjong.v32 || goto :failed
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