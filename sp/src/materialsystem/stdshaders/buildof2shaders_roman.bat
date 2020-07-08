@echo off
setlocal

rem ================================
rem ==== MOD PATH CONFIGURATIONS ===

rem == Set the absolute path to your mod's game directory here ==
set GAMEDIR="J:\Steam\steamapps\sourcemods\of2"

rem == Set the relative or absolute path to Source SDK Base 2013 Singleplayer\bin ==
set SDKBINDIR="J:\Steam\steamapps\common\Source SDK Base 2013 Singleplayer\bin"

rem ==  Set the Path to your mod's root source code ==
rem This should already be correct, accepts relative paths only!
set SOURCEDIR=..\..

rem ==== MOD PATH CONFIGURATIONS END ===
rem ====================================


rem Use dynamic shaders to build .inc files only
rem set dynamic_shaders=1
rem == Setup path to nmake.exe, from vc 2013 common tools directory ==
call "%VS120COMNTOOLS%vsvars32.bat"

set BUILD_SHADER=call buildshaders.bat
set ARG_EXTRA=

%BUILD_SHADER% of2shader_dx9_20	-game %GAMEDIR% -source %SOURCEDIR%

rem echo.
if not "%dynamic_shaders%" == "1" (
  echo Finished full buildallshaders %*
) else (
  echo Finished dynamic buildallshaders %*
)

PAUSE