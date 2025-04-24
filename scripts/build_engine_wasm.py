# 
# ./build/tools/emsdk/emsdk activate latest
# ./build/tools/emsdk/emsdk_env
# python scripts/build_engine_wasm.py
import utils
import os

triplet = "wasm32-emscripten"
generator = "Ninja"
build_dir = f"{utils.root_dir}/build/buildtrees/{triplet}/lumino"
installDir = f"{utils.root_dir}/build/installed/{triplet}"
vcpkgDir = f"{utils.root_dir}/vcpkg"
cmakeHomeDir = utils.root_dir
ninja = f"{utils.root_dir}/build/tools/ninja-win"

os.environ["PATH"] = f"{ninja};{os.environ['PATH']}"

utils.cd_to_root()

args = [
    f"emcmake",
    f"cmake",
    f"{cmakeHomeDir}",
    f"-B {build_dir}",
    f"-G\"{generator}\"",
    f"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
    f"-DVCPKG_TARGET_TRIPLET={triplet}",
]
utils.call(' '.join(args))

args = [
    f"cmake",
    f"--build {build_dir}",
    f"-j8",
]
utils.call(' '.join(args))
