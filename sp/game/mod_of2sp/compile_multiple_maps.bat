call %~dp0setvars.bat
set buildmap=call %~dp0compile_map.bat
set contentdir=..\..\content\mod_of2sp
set mapsrcdir=%GameDir%\%contentdir%\mapsrc

rem %buildmap% %mapsrcdir%\Miscellaneous\of2_canals_02.vmf
rem %buildmap% %mapsrcdir%\Miscellaneous\of2_canals_07.vmf
rem %buildmap% %mapsrcdir%\Miscellaneous\of2_devmap.vmf
%buildmap% %mapsrcdir%\Miscellaneous\dev_catapult_test.vmf

pause