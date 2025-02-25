@echo off
REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Assemble the ASM code
echo --------------------------
assemble TutorialASM-MoveAnimated.asm -o obj\TutorialASM-MoveAnimated.vbin || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon TextureMars.png -o obj\TextureMars.vtex || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom TutorialASM-MoveAnimated.xml -o bin\TutorialASM-MoveAnimated.v32 || goto :failed
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