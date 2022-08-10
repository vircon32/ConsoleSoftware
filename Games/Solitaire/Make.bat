@echo off

REM prepare the script to be run from another directory
pushd %~dp0

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

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
png2vircon TextureSolitaire.png -o obj\TextureSolitaire.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon music\MusicTitle.wav    -o obj\MusicTitle.vsnd    || goto :failed
wav2vircon music\MusicGameplay.wav -o obj\MusicGameplay.vsnd || goto :failed

wav2vircon sounds\SoundTitleLogo.wav    -o obj\SoundTitleLogo.vsnd    || goto :failed
wav2vircon sounds\SoundTitleStart.wav   -o obj\SoundTitleStart.vsnd   || goto :failed
wav2vircon sounds\SoundMenuMove.wav     -o obj\SoundMenuMove.vsnd     || goto :failed
wav2vircon sounds\SoundMenuAccept.wav   -o obj\SoundMenuAccept.vsnd   || goto :failed
wav2vircon sounds\SoundMenuCancel.wav   -o obj\SoundMenuCancel.vsnd   || goto :failed
wav2vircon sounds\SoundDealLadders.wav  -o obj\SoundDealLadders.vsnd  || goto :failed
wav2vircon sounds\SoundDraw.wav         -o obj\SoundDraw.vsnd         || goto :failed
wav2vircon sounds\SoundCardSlide.wav    -o obj\SoundCardSlide.vsnd    || goto :failed
wav2vircon sounds\SoundCardDrop.wav     -o obj\SoundCardDrop.vsnd     || goto :failed
wav2vircon sounds\SoundCardFlip.wav     -o obj\SoundCardFlip.vsnd     || goto :failed
wav2vircon sounds\SoundWrongMove.wav    -o obj\SoundWrongMove.vsnd    || goto :failed
wav2vircon sounds\SoundQuitGame.wav     -o obj\SoundQuitGame.vsnd     || goto :failed
wav2vircon sounds\SoundWin.wav          -o obj\SoundWin.vsnd          || goto :failed
wav2vircon sounds\SoundWinHighlight.wav -o obj\SoundWinHighlight.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Solitaire.xml -o bin\Solitaire.v32 || goto :failed
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