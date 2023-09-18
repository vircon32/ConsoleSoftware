@echo off

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import embedded data files
echo --------------------------
tiled2vircon maps\TileMapBricks.tmx -o obj\ || goto :failed
tiled2vircon maps\TileMapScenery.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile Demo-Bricks.c -o obj\Demo-Bricks.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Demo-Bricks.asm -o obj\Demo-Bricks.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-Bricks.png -o obj\Texture-Bricks.vtex || goto :failed

echo.
echo Convert the WAV sounds
echo --------------------------
wav2vircon Music-Gameplay.wav -o obj\Music-Gameplay.vsnd || goto :failed

wav2vircon Sound-PaddleBounce.wav      -o obj\Sound-PaddleBounce.vsnd      || goto :failed
wav2vircon Sound-WallBounce.wav        -o obj\Sound-WallBounce.vsnd        || goto :failed
wav2vircon Sound-RegularBlock.wav      -o obj\Sound-RegularBlock.vsnd      || goto :failed
wav2vircon Sound-NonBreakableBlock.wav -o obj\Sound-NonBreakableBlock.vsnd || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Demo-Bricks.xml -o "bin\Bricks (Demo).v32" || goto :failed
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