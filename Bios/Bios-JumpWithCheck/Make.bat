@echo off

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Assemble the ASM code
echo --------------------------
assemble Bios-JumpWithCheck.asm -o obj/Bios-JumpWithCheck.vbin -b || goto :failed

echo.
echo Convert the PNG texture
echo --------------------------
png2vircon DummyBiosTexture.png -o obj/DummyBiosTexture.vtex || goto :failed

echo.
echo Convert the WAV sound
echo --------------------------
wav2vircon DummyBiosSound.wav -o obj/DummyBiosSound.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Bios-JumpWithCheck.xml -o bin/Bios-JumpWithCheck.v32 || goto :failed

echo.
echo Copy ROM to emulator BIOS folder
echo ----------------------------------
copy /Y bin/Bios-JumpWithCheck.v32 "../../../Emulator/Binaries/Bios" || goto :failed
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