rem == this is the location of the 2013 single player base bin folder ==
SET SPBASE2013=K:\Source SDK Base 2013 Singleplayer

rem == this is your mod folder ==
rem == edit this only if you don't want this .bat file directly in your mod folder ==
SET GameDir=%~dp0

rem == this removes trailing slash ==
rem == do not edit ==
IF %GameDir:~-1%==\ SET GameDir=%GameDir:~0,-1%