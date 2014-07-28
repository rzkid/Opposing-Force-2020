call %~dp0setvars
set filename=%~dpn1
"%SPBASE2013%\bin\vbsp.exe" -game "%GameDir%" %filename%
"%SPBASE2013%\bin\vvis.exe" -game "%GameDir%" %filename%
"%SPBASE2013%\bin\vrad.exe" -game "%GameDir%" %filename%
XCOPY "%filename%.bsp" "%GameDir%\maps" /D /y

rem pause
