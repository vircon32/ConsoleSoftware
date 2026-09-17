@echo off
setlocal EnableExtensions

rem ---------------------------------------------------------------------------
rem VIRCOUT - native Windows build script
rem Requires the Vircon32 command-line tools to be available in PATH:
rem   v32lua, assemble, packrom, png2vircon, wav2vircon
rem Run this file from Explorer or from cmd.exe.
rem ---------------------------------------------------------------------------

cd /d "%~dp0"

set "GAME=vircout"
set "ROM_NAME=Vircout"
set "V32LUA=v32lua"
set "ASSEMBLER=assemble"
set "PACKER=packrom"
set "TEXTURE_TOOL=png2vircon"
set "SOUND_TOOL=wav2vircon"

if not exist "obj" mkdir "obj"
if errorlevel 1 goto :error
if not exist "bin" mkdir "bin"
if errorlevel 1 goto :error

echo [1/4] Converting textures...
call :texture "assets\sprites.png" "assets\sprites.vtex" || goto :error
call :texture "assets\title.png" "assets\title.vtex" || goto :error
call :texture "assets\gameplay.png" "assets\gameplay.vtex" || goto :error
call :texture "assets\ending.png" "assets\ending.vtex" || goto :error

echo [2/4] Converting audio...
call :sound "audio\music_title.wav" "audio\music_title.vsnd" || goto :error
call :sound "audio\music_gameplay.wav" "audio\music_gameplay.vsnd" || goto :error
call :sound "audio\music_ending.wav" "audio\music_ending.vsnd" || goto :error
call :sound "audio\sfx_paddle.wav" "audio\sfx_paddle.vsnd" || goto :error
call :sound "audio\sfx_wall.wav" "audio\sfx_wall.vsnd" || goto :error
call :sound "audio\sfx_normal.wav" "audio\sfx_normal.vsnd" || goto :error
call :sound "audio\sfx_hard_hit.wav" "audio\sfx_hard_hit.vsnd" || goto :error
call :sound "audio\sfx_hard_break.wav" "audio\sfx_hard_break.vsnd" || goto :error
call :sound "audio\sfx_metal.wav" "audio\sfx_metal.vsnd" || goto :error
call :sound "audio\sfx_item.wav" "audio\sfx_item.vsnd" || goto :error
call :sound "audio\sfx_laser.wav" "audio\sfx_laser.vsnd" || goto :error
call :sound "audio\sfx_ball_lost.wav" "audio\sfx_ball_lost.vsnd" || goto :error
call :sound "audio\sfx_level_clear.wav" "audio\sfx_level_clear.vsnd" || goto :error
call :sound "audio\sfx_game_over.wav" "audio\sfx_game_over.vsnd" || goto :error
call :sound "audio\sfx_cheat.wav" "audio\sfx_cheat.vsnd" || goto :error

echo [3/4] Compiling Lua and assembling...
"%V32LUA%" -o "obj\%GAME%.asm" "%GAME%.lua"
if errorlevel 1 goto :error

rem v32lua writes the cartridge XML beside the ASM output.
if not exist "obj\%GAME%.xml" (
    echo ERROR: v32lua did not generate obj\%GAME%.xml
    goto :error
)
move /Y "obj\%GAME%.xml" "%GAME%.xml" >nul
if errorlevel 1 goto :error

"%ASSEMBLER%" -o "obj\%GAME%.vbin" "obj\%GAME%.asm"
if errorlevel 1 goto :error

echo [4/4] Packing ROM...
"%PACKER%" "%GAME%.xml" -o "bin\%ROM_NAME%.v32"
if errorlevel 1 goto :error

echo.
echo Build completed successfully:
echo   bin\%ROM_NAME%.v32
echo.
exit /b 0

:texture
"%TEXTURE_TOOL%" %1 -o %2
exit /b %errorlevel%

:sound
"%SOUND_TOOL%" -o %2 %1
exit /b %errorlevel%

:error
echo.
echo BUILD FAILED.
echo Check that all Vircon32 command-line tools are installed and available in PATH.
echo.
exit /b 1
