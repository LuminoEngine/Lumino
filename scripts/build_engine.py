import utils

# slang が MD しか使えないようなので。
#triplet = "x64-windows-static"
triplet = "x64-windows"
build_dir = f"{utils.root_dir}/build/buildtrees/{triplet}/lumino"
installDir = f"{utils.root_dir}/build/installed/{triplet}"
vcpkgDir = f"{utils.root_dir}/vcpkg"
cmakeHomeDir = utils.root_dir

utils.cd_to_root()

args = [
    f"cmake",
    f"{cmakeHomeDir}",
    f"-B {build_dir}",
    f"-G\"Visual Studio 17 2022\"",
    f"-A x64",
    f"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
    f"-DCMAKE_TOOLCHAIN_FILE={vcpkgDir}/scripts/buildsystems/vcpkg.cmake",
    f"-DVCPKG_TARGET_TRIPLET={triplet}",
    f"-DCMAKE_CXX_FLAGS=\"/MP\"",
]
utils.call(' '.join(args))
