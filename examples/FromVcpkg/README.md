FromVcpkg
==========

Here is an example using Lumino installed by vcpkg.

```sh
git clone https://github.com/Microsoft/vcpkg.git
git clone -b new-port-lumino https://github.com/lriki/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install lumino[core]
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```


