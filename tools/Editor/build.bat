@echo off
mkdir build
cd build

cmake -G "Visual Studio 12 Win64" -DCMAKE_PREFIX_PATH=C:\Qt\5.6\msvc2013_64  ../
cd ..