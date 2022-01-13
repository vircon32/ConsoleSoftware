@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile CodeBlocksTest.c -o obj\CodeBlocksTest.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\CodeBlocksTest.asm -o obj\CodeBlocksTest.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-ColorsWithAlpha.png -o obj\Texture-ColorsWithAlpha.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-SoftSynth.wav -o obj\Sound-SoftSynth.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom CodeBlocksTest.xml -o bin\CodeBlocksTest.v32 || goto :failed
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