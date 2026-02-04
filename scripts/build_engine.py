import utils

triplet = "x64-windows" # slang が MD しか使えないようなので、 Static Runtime (x64-windows-static) は使わない
generator = "Visual Studio 18 2026"
build_dir = f"{utils.root_dir}/build/buildtrees/{triplet}/lumino"
installDir = f"{utils.root_dir}/build/installed/{triplet}"
vcpkgDir = f"{utils.root_dir}/vcpkg"
cmakeHomeDir = utils.root_dir

utils.cd_to_root()

args = [
    f"cmake",
    f"{cmakeHomeDir}",
    f"-B {build_dir}",
    f"-G\"{generator}\"",
    f"-A x64",
    f"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
    f"-DCMAKE_TOOLCHAIN_FILE={vcpkgDir}/scripts/buildsystems/vcpkg.cmake",
    f"-DVCPKG_TARGET_TRIPLET={triplet}",
    f"-DCMAKE_CXX_FLAGS=\"/MP\"",
]
utils.call(' '.join(args))
