# Using only the Core module

1. Configure

```sh
# Without unit tests.
cmake -S <RepositoryRootDir> -B <BuildDir> -G<Generator> \
  -DCMAKE_INSTALL_PREFIX=<InstallDir> \
  -DCMAKE_DEBUG_POSTFIX=d \
  -DLUMINO_BUILD_CORE_ONLY=ON \
  -DLUMINO_BUILD_TESTS=OFF

# With unit tests.
cmake -S <RepositoryRootDir> -B <BuildDir> -G<Generator> \
  -DCMAKE_INSTALL_PREFIX=<InstallDir> \
  -DCMAKE_DEBUG_POSTFIX=d \
  -DLUMINO_BUILD_CORE_ONLY=ON
```

2. Build

```sh
cmake --build <BuildDir> --config Debug --target install
cmake --build <BuildDir> --config Release --target install
```
