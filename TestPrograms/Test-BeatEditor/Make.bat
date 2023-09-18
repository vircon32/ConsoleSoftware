@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Test-BeatEditor.c -o obj/Test-BeatEditor.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj/Test-BeatEditor.asm -o obj/Test-BeatEditor.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureEditor.png -o obj/TextureEditor.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundDrum.wav -o obj/SoundDrum.vsnd || goto :failed
wav2vircon SoundSticks.wav -o obj/SoundSticks.vsnd || goto :failed
wav2vircon SoundClap.wav -o obj/SoundClap.vsnd || goto :failed
wav2vircon SoundBell.wav -o obj/SoundBell.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Test-BeatEditor.xml -o "bin/Test - Beat editor.v32" || goto :failed
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