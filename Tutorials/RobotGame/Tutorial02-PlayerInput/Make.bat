@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Compile the C code
echo --------------------------
compile Tutorial02-PlayerInput.c -o obj\Tutorial02-PlayerInput.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\Tutorial02-PlayerInput.asm -o obj\Tutorial02-PlayerInput.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon Texture-RobotLab.png -o obj\Texture-RobotLab.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Tutorial02-PlayerInput.xml -o bin\Tutorial02-PlayerInput.v32 || goto :failed
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