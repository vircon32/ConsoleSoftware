@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile BadApple.c -o obj\BadApple.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\BadApple.asm -o obj\BadApple.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureBadApple.png -o obj\TextureBadApple.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon MusicBadApple.wav -o obj\MusicBadApple.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom BadApple.xml -o "bin\Bad Apple.v32" || goto :failed
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