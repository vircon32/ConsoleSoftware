@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import tile map as an embedded data file
echo -------------------------------------------
tiled2vircon map\TileMapTest.tmx -o obj\ || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile RPGConcept.c -o obj\RPGConcept.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\RPGConcept.asm -o obj\RPGConcept.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRPG.png        -o obj\TextureRPG.vtex        || goto :failed
png2vircon TextureDialogFont.png -o obj\TextureDialogFont.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom RPGConcept.xml -o bin\RPGConcept.v32 || goto :failed
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