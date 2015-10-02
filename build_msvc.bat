
set CMakeExe="cmake"

:------------------------------------------------------------------------------
mkdir ".\build_vs2013_u"
cd "build_vs2013_u"
cmake ".." -G "Visual Studio 12 2013" -DLN_USE_UNICODE_CHAR_SET=ON

:MSBuild ".\Lumino.sln" /t:Rebuild /p:Configuration="Debug" /p:Platform="Win32" /m
:if "%ERRORLEVEL%" NEQ "0" goto BUILD_FAILED

MSBuild ".\Lumino.sln" /t:Rebuild /p:Configuration="Release" /p:Platform="Win32" /m
if "%ERRORLEVEL%" NEQ "0" goto BUILD_FAILED

cd ".."
:------------------------------------------------------------------------------
mkdir ".\build_vs2013_mb"
cd "build_vs2013_mb"
cmake ".." -G "Visual Studio 12 2013" -DLN_USE_UNICODE_CHAR_SET=OFF

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
