
set SrcDir=..\src\C_API\
set LibDir=..\..\lib\

mkdir Include
copy %SrcDir%LuminoC.h Include\
copy %SrcDir%LNApplication.h Include\
copy %SrcDir%LNAudio.h Include\
copy %SrcDir%LNBase.h Include\
copy %SrcDir%LNCommon.h Include\
copy %SrcDir%LNMath.h Include\
copy %SrcDir%LNTypedef.h Include\

mkdir Lib
copy %LibDir%LuminoC_x86MTd.lib Lib\
copy %LibDir%LuminoC_x86MTd.dll Lib\
copy %LibDir%LuminoC_x86MT.lib Lib\
copy %LibDir%LuminoC_x86MT.dll Lib\
copy %LibDir%LuminoC_x86uMTd.lib Lib\
copy %LibDir%LuminoC_x86uMTd.dll Lib\
copy %LibDir%LuminoC_x86uMT.lib Lib\
copy %LibDir%LuminoC_x86uMT.dll Lib\

mkdir Samples\Debug
copy Lib\*.dll Samples\Debug\

mkdir Samples\Release
copy Lib\*.dll Samples\Release\

xcopy ..\Common\Media Samples\Media /i /d /y

