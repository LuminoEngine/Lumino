copy "LuminoC_x86MTd.dll" "Samples\LuminoC_x86MTd.dll"
copy "LuminoC_x86MT.dll" "Samples\LuminoC_x86MT.dll"

set PATH=..\..\..\DevKit-mingw64-32-4.7.2-20130224-1151-sfx\mingw\bin;..\..\..\DevKit-mingw64-32-4.7.2-20130224-1151-sfx\bin;%PATH%

ruby extconf.rb

make
