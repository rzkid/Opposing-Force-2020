call %~dp0setvars.bat
set buildmap = call %~dp0compile_map.bat

%buildmap% "%GameDir%\..\..\content\mod_of2sp\of2_canals_02.vmf"

pause