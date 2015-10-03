
set CMakeExe="cmake"

:------------------------------------------------------------------------------
mkdir ".\build_vs2015_u"
cd "build_vs2015_u"
cmake ".." -G "Visual Studio 14 2015" -DLN_USE_UNICODE_CHAR_SET=ON

:MSBuild ".\Lumino.sln" /t:Rebuild /p:Configuration="Debug" /p:Platform="Win32" /m
:if "%ERRORLEVEL%" NEQ "0" goto BUILD_FAILED

MSBuild ".\Lumino.sln" /t:Rebuild /p:Configuration="Release" /p:Platform="Win32" /m
if "%ERRORLEVEL%" NEQ "0" goto BUILD_FAILED

cd ".."
:------------------------------------------------------------------------------
mkdir ".\build_vs2015_mb"
cd "build_vs2015_mb"
cmake ".." -G "Visual Studio 14 2015" -DLN_USE_UNICODE_CHAR_SET=OFF

:MSBuild ".\Lumino.sln" /t:Rebuild /p:Configuration="Debug" /p:Platform="Win32" /m
:if "%ERRORLEVEL%" NEQ "0" goto BUILD_FAILED

MSBuild ".\Lumino.sln" /t:Rebuild /p:Configuration="Release" /p:Platform="Win32" /m
if "%ERRORLEVEL%" NEQ "0" goto BUILD_FAILED

cd ".."
:------------------------------------------------------------------------------


goto END

:BUILD_FAILED
cd ".."
echo "Build failed."
goto END

:UNITTEST_FAILED
cd ".."
echo "UnitTest failed."
goto END

:END
