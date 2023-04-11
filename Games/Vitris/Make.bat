@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import the Tiled tile maps
echo --------------------------
tiled2vircon maps\TitleMap.tmx    -o obj\ || goto :failed
tiled2vircon maps\PieceModels.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\Vitris.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Vitris.asm -o obj\Vitris.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureTitle.png    -o obj\TextureTitle.vtex    || goto :failed
png2vircon TextureGameplay.png -o obj\TextureGameplay.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicTitle.wav         -o obj\MusicTitle.vsnd        || goto :failed
wav2vircon music\MusicGameplay.wav      -o obj\MusicGameplay.vsnd     || goto :failed

wav2vircon sounds\SoundPressStart.wav   -o obj\SoundPressStart.vsnd   || goto :failed
wav2vircon sounds\SoundMenuMove.wav     -o obj\SoundMenuMove.vsnd     || goto :failed
wav2vircon sounds\SoundMenuAccept.wav   -o obj\SoundMenuAccept.vsnd   || goto :failed
wav2vircon sounds\SoundMenuCancel.wav   -o obj\SoundMenuCancel.vsnd   || goto :failed
wav2vircon sounds\SoundGameOver.wav     -o obj\SoundGameOver.vsnd     || goto :failed
wav2vircon sounds\SoundQuitGame.wav     -o obj\SoundQuitGame.vsnd     || goto :failed
wav2vircon sounds\SoundPieceMove.wav    -o obj\SoundPieceMove.vsnd    || goto :failed
wav2vircon sounds\SoundPieceRotate.wav  -o obj\SoundPieceRotate.vsnd  || goto :failed
wav2vircon sounds\SoundPieceDrop.wav    -o obj\SoundPieceDrop.vsnd    || goto :failed
wav2vircon sounds\SoundPieceLock.wav    -o obj\SoundPieceLock.vsnd    || goto :failed
wav2vircon sounds\SoundClear1Line.wav   -o obj\SoundClear1Line.vsnd   || goto :failed
wav2vircon sounds\SoundClear2Lines.wav  -o obj\SoundClear2Lines.vsnd  || goto :failed
wav2vircon sounds\SoundClear3Lines.wav  -o obj\SoundClear3Lines.vsnd  || goto :failed
wav2vircon sounds\SoundClear4Lines.wav  -o obj\SoundClear4Lines.vsnd  || goto :failed
wav2vircon sounds\SoundBoardFall.wav    -o obj\SoundBoardFall.vsnd    || goto :failed
wav2vircon sounds\SoundLevelAdvance.wav -o obj\SoundLevelAdvance.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Vitris.xml -o bin\Vitris.v32 || goto :failed
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