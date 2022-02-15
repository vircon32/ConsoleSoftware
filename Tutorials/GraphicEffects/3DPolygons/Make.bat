@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial-3DPolygons.c -o obj\Tutorial-3DPolygons.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-3DPolygons.asm -o obj\Tutorial-3DPolygons.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture3DPolygons.png -o obj\Texture3DPolygons.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-3DPolygons.xml -o bin\Tutorial-3DPolygons.v32 || goto :failed
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