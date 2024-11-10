@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial-ScrollParallax.c -o obj\Tutorial-ScrollParallax.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial-ScrollParallax.asm -o obj\Tutorial-ScrollParallax.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureScrollParallax.png -o obj\TextureScrollParallax.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial-ScrollParallax.xml -o bin\Tutorial-ScrollParallax.v32 || goto :failed
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