@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Particles.c -o obj\Particles.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Particles.asm -o obj\Particles.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureParticles.png -o obj\TextureParticles.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Particles.xml -o bin\Particles.v32 || goto :failed
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