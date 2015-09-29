
set NativeLibDir=..\lib\

set NativeReleaseDll=%NativeLibDir%LuminoC_x86MT.dll
set NativeReleaseDllU=%NativeLibDir%LuminoC_x86uMT.dll

copy %NativeReleaseDllU% DotNet\
copy %NativeReleaseDll% Ruby\

xcopy Common\Media DotNet\Samples\Media /i /d /y

pause
