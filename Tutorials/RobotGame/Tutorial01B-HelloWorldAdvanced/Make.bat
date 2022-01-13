@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial01B-HelloWorldAdvanced.c -o obj\Tutorial01B-HelloWorldAdvanced.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial01B-HelloWorldAdvanced.asm -o obj\Tutorial01B-HelloWorldAdvanced.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-HelloWorld.png -o obj\Texture-HelloWorld.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Sound-HelloWorld.wav -o obj\Sound-HelloWorld.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial01B-HelloWorldAdvanced.xml -o bin\Tutorial01B-HelloWorldAdvanced.v32 || goto :failed
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