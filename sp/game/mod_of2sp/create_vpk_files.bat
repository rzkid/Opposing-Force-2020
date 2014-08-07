call %~dp0setvars

set VPK="%SPBASE2013%\bin\vpk.exe"

python build_vpk.py %VPK% "of2_assets" "models" "materials" "sound" "particles"

pause