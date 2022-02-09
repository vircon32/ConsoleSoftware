@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial-WaterReflection.c -o obj\Tutorial-WaterReflection.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-WaterReflection.asm -o obj\Tutorial-WaterReflection.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureLakeMountain.png -o obj\TextureLakeMountain.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon SoundDripping.wav -o obj\SoundDripping.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-WaterReflection.xml -o bin\Tutorial-WaterReflection.v32 || goto :failed
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