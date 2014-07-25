call %~dp0setvars
set filename=%~dpn1
"%SPBASE2013BIN%\vbsp.exe" -game "%GameDir%" %filename%
"%SPBASE2013BIN%\vvis.exe" -game "%GameDir%" %filename%
"%SPBASE2013BIN%\vrad.exe" -game "%GameDir%" %filename%
XCOPY "%filename%.bsp" "%GameDir%\maps" /D /y
pause