
mkdir build_msvc120x86_MT
cd build_msvc120x86_MT
cmake -G"Visual Studio 12" -DLN_USE_UNICODE_CHAR_SET=OFF -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ../..
cd ..

mkdir build_msvc120x86u_MT
cd build_msvc120x86u_MT
cmake -G"Visual Studio 12" -DLN_USE_UNICODE_CHAR_SET=ON -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ../..
cd ..

mkdir build_msvc140x86_MT
cd build_msvc140x86_MT
cmake -G"Visual Studio 14 2015" -DLN_USE_UNICODE_CHAR_SET=OFF -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ../..
cd ..

mkdir build_msvc140x86u_MT
cd build_msvc140x86u_MT
cmake -G"Visual Studio 14 2015" -DLN_USE_UNICODE_CHAR_SET=ON -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ../..
cd ..

