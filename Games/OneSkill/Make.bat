@echo off
REM | Since program changes much more often than the rest,
REM | these scripts will separately process images, sounds
REM | and tilemaps so that we can choose whether to build
REM | everything (MakeAll.bat) or just update the previous
REM | build with any changes done to the code (Make.bat)
REM | For a first time build, MakeAll.bat must be used.

echo.
echo Compile the C code
echo --------------------------
compile Main.c -o obj\OneSkill.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\OneSkill.asm -o obj\OneSkill.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom OneSkill.xml -o "bin\One Skill.v32" || goto :failed
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