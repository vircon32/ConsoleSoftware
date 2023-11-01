@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Example-BoxCollisions.c -o obj\Example-BoxCollisions.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Example-BoxCollisions.asm -o obj\Example-BoxCollisions.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureCollisions.png -o obj\TextureCollisions.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Example-BoxCollisions.xml -o bin\Example-BoxCollisions.v32 || goto :failed
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