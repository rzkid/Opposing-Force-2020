rem == drag and drop your qc files here ==

call %~dp0setvars

"%SPBASE2013BIN%\studiomdl.exe" -game "%GameDir%" -nop4 %1

rem pause