@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Pseudo3DRacer.c -o obj\Pseudo3DRacer.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Pseudo3DRacer.asm -o obj\Pseudo3DRacer.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRoad.png -o obj\TextureRoad.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundRoad.wav   -o obj\SoundRoad.vsnd   || goto :failed
wav2vircon SoundEngine.wav -o obj\SoundEngine.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Pseudo3DRacer.xml -o "bin\Pseudo-3D Racer (Demo).v32" || goto :failed
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