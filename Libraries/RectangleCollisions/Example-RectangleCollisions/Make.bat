@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Example-RectangleCollisions.c -o obj\Example-RectangleCollisions.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Example-RectangleCollisions.asm -o obj\Example-RectangleCollisions.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureRectangles.png -o obj\TextureRectangles.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Example-RectangleCollisions.xml -o bin\Example-RectangleCollisions.v32 || goto :failed
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