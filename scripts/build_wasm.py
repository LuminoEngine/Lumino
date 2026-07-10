#!/usr/bin/env python3
"""Build Lumino for the web (Emscripten / WebAssembly).

Usage:
    python scripts/build_wasm.py              # configure + build + copy (Release)
    python scripts/build_wasm.py all          # same as above
    python scripts/build_wasm.py configure    # cmake configure only
    python scripts/build_wasm.py build        # cmake --build + copy
    python scripts/build_wasm.py clean        # remove output
    python scripts/build_wasm.py --debug      # Debug 構成でビルド (build/lumino-wasm32-emscripten-debug)
"""

from __future__ import annotations

import argparse
import os
import shlex
import shutil
import subprocess
import sys
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parent.parent
# フラグなし(Release)は従来と同じディレクトリ名を使う。--debug は専用ディレクトリに分離する。
BUILD_DIR_RELEASE = REPO_ROOT / "build" / "lumino-wasm32-emscripten"
BUILD_DIR_DEBUG = REPO_ROOT / "build" / "lumino-wasm32-emscripten-debug"
OUTPUT_DIR = REPO_ROOT / "packages" / "luminojs" / "lib"

# Emscripten SDK is always cloned into <repo>/build/emsdk per README.md.
EMSDK_ROOT = REPO_ROOT / "build" / "emsdk"

# WebGPU-distribution is pinned to this commit per README.md. Update both
# when bumping the Dawn version.
WEBGPU_ROOT = REPO_ROOT / "build" / "webgpu"
WEBGPU_EXPECTED_COMMIT = "17dcd42a7683355e7a40ac4e97e77f36dff5b5ab"
NINJA = REPO_ROOT / "build" / "vcpkg" / "downloads" / "tools" / "ninja-1.13.2-windows" / "ninja.exe"
VCPKG_TOOLCHAIN = REPO_ROOT / "build" / "vcpkg" / "scripts" / "buildsystems" / "vcpkg.cmake"

# Files produced by the Emscripten link step that should be shipped to the TypeScript package.
ARTIFACT_NAMES = ("LuminoC.mjs", "LuminoC.wasm", "LuminoC.wasm.map")

# Native binaries (luminosc) built for x64-Windows that are bundled into the
# TypeScript package for use as a local CLI tool.
NATIVE_BIN_SRC = REPO_ROOT / "build" / "lumino-x64-windows" / "packages" / "luminosc" / "Release"
NATIVE_BIN_DEST = REPO_ROOT / "packages" / "luminojs" / "bin" / "x64-windows"
NATIVE_BIN_GLOBS = ("*.exe", "*.dll")

# C++ example assets (shaders, textures) that should be available to the
# TypeScript examples via the Vite dev server / production build.
EXAMPLE_ASSETS_SRC = REPO_ROOT / "packages" / "LuminoC" / "examples" / "assets"
EXAMPLE_ASSETS_DEST = REPO_ROOT / "packages" / "luminojs-examples" / "public"
EXAMPLE_ASSET_GLOBS = ("*.lcsh", "*.png")


#----------------------------------------------------------------------------
# Emscripten SDK resolution
#----------------------------------------------------------------------------

def _find_emsdk_env(emsdk_root: Path) -> Path:
    """Return the path to emsdk_env.bat / emsdk_env.sh for the given root."""
    if os.name == "nt":
        candidate = emsdk_root / "emsdk_env.bat"
    else:
        candidate = emsdk_root / "emsdk_env.sh"
    if not candidate.is_file():
        raise FileNotFoundError(
            f"emsdk env script not found at {candidate}. "
            "Clone emsdk into <repo>/build/emsdk as described in README.md:\n"
            "  git clone -b 5.0.5 https://github.com/emscripten-core/emsdk.git ./build/emsdk\n"
            "  ./build/emsdk/emsdk install 5.0.5\n"
            "  ./build/emsdk/emsdk activate 5.0.5"
        )
    return candidate


def resolve_emsdk() -> Path:
    """Return the in-tree emsdk root (``<repo>/emsdk``)."""
    if not EMSDK_ROOT.is_dir():
        raise SystemExit(
            f"emsdk directory not found at {EMSDK_ROOT}.\n"
            "Clone it into <repo>/build/emsdk as described in README.md"
        )
    return EMSDK_ROOT


def check_webgpu_pin() -> None:
    """build/webgpu が README.md に記載のコミットで固定されているか確認します。"""
    if not WEBGPU_ROOT.is_dir():
        raise SystemExit(
            f"webgpu directory not found at {WEBGPU_ROOT}.\n"
            "Clone it into the repository root as described in README.md:\n"
            "  git clone https://github.com/eliemichel/WebGPU-distribution.git ./build/webgpu\n"
            f"  git -C ./build/webgpu checkout {WEBGPU_EXPECTED_COMMIT}"
        )
    try:
        result = subprocess.run(
            ["git", "-C", str(WEBGPU_ROOT), "rev-parse", "HEAD"],
            check=True,
            capture_output=True,
            text=True,
        )
    except (subprocess.CalledProcessError, FileNotFoundError):
        return
    actual_commit = result.stdout.strip()
    if actual_commit != WEBGPU_EXPECTED_COMMIT:
        print(
            f"[build_wasm] warning: build/webgpu is at {actual_commit}, "
            f"expected {WEBGPU_EXPECTED_COMMIT} (see README.md). "
            "Dawn version may differ from the pinned one.",
            flush=True,
        )


#----------------------------------------------------------------------------
# Commands
#----------------------------------------------------------------------------

def _run_with_emsdk(emsdk_root: Path, cmd_args: list[str]) -> None:
    env_script = _find_emsdk_env(emsdk_root)
    print(f"[build_wasm] $ {' '.join(cmd_args)}", flush=True)
    if os.name == "nt":
        inner = subprocess.list2cmdline(cmd_args)
        full = f'cmd.exe /s /c "call "{env_script}" >NUL && {inner}"'
        subprocess.run(full, check=True, cwd=str(REPO_ROOT))
    else:
        joined = " ".join(shlex.quote(a) for a in cmd_args)
        full = f'. "{env_script}" >/dev/null && {joined}'
        subprocess.run(["bash", "-c", full], check=True, cwd=str(REPO_ROOT))


def _build_dir(debug: bool) -> Path:
    return BUILD_DIR_DEBUG if debug else BUILD_DIR_RELEASE


def _build_type(debug: bool) -> str:
    return "Debug" if debug else "Release"


def cmd_configure(emsdk_root: Path, debug: bool) -> None:
    check_webgpu_pin()
    build_dir = _build_dir(debug)
    build_type = _build_type(debug)
    build_dir.mkdir(parents=True, exist_ok=True)
    emscripten_toolchain = (
        emsdk_root / "upstream" / "emscripten" / "cmake" / "Modules" / "Platform" / "Emscripten.cmake"
    )
    if not emscripten_toolchain.is_file():
        raise FileNotFoundError(
            f"Emscripten CMake toolchain not found at {emscripten_toolchain}. "
            "Make sure emsdk is installed and activated."
        )
    print(f"[build_wasm] configuring ({build_type}) -> {build_dir}", flush=True)
    _run_with_emsdk(
        emsdk_root,
        [
            "cmake",
            "-S",
            str(REPO_ROOT),
            "-B",
            str(build_dir),
            "-G",
            "Ninja",
            f"-DCMAKE_BUILD_TYPE={build_type}",
            f"-DCMAKE_MAKE_PROGRAM={NINJA}",
            f"-DCMAKE_TOOLCHAIN_FILE={VCPKG_TOOLCHAIN}",
            f"-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE={emscripten_toolchain}",
            "-DVCPKG_TARGET_TRIPLET=wasm32-emscripten",
            "-DEMSCRIPTEN=ON",
            "-DLUMINO_BUILD_EXAMPLES=OFF",
            "-DLUMINO_BUILD_TESTS=OFF",
            "-DLUMINO_BUILD_TOOLS=OFF",
            "-DLUMINO_USE_WEBGPU=ON",
            "-DWEBGPU_BACKEND=EMDAWNWEBGPU",
        ],
    )


def cmd_build(emsdk_root: Path, debug: bool) -> None:
    build_dir = _build_dir(debug)
    build_type = _build_type(debug)
    if not (build_dir / "CMakeCache.txt").exists():
        cmd_configure(emsdk_root, debug)
    print(f"[build_wasm] building ({build_type}) -> {build_dir}", flush=True)
    _run_with_emsdk(emsdk_root, ["cmake", "--build", str(build_dir)])
    _copy_artifacts(debug)
    _print_size_report()


def cmd_clean(debug: bool) -> None:
    build_dir = _build_dir(debug)
    if build_dir.exists():
        print(f"[build_wasm] removing {build_dir}")
        shutil.rmtree(build_dir)
    if OUTPUT_DIR.exists():
        for name in ARTIFACT_NAMES:
            p = OUTPUT_DIR / name
            if p.exists():
                print(f"[build_wasm] removing {p}")
                p.unlink()


def _copy_artifacts(debug: bool) -> None:
    build_dir = _build_dir(debug)
    build_type = _build_type(debug)
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    src_dir = build_dir / "packages" / "LuminoC"
    copied = 0
    for name in ARTIFACT_NAMES:
        src = src_dir / name
        if not src.exists():
            if name.endswith(".map"):
                continue  # source map is optional
            raise FileNotFoundError(f"Expected build artifact not found: {src}")
        dst = OUTPUT_DIR / name
        shutil.copy2(src, dst)
        print(f"[build_wasm] copied ({build_type}) {src} -> {dst}")
        copied += 1
    if copied == 0:
        raise RuntimeError("No artifacts were copied. Did the build succeed?")

    _copy_native_binaries()
    _copy_example_assets()


def _human_size(num_bytes: int) -> str:
    return f"{num_bytes / 1024:.1f} KiB"


def _print_size_report() -> None:
    """Debug/Release 両方のビルド成果物のサイズを、存在するものだけ比較表示します。"""
    sources = {
        "Debug": BUILD_DIR_DEBUG / "packages" / "LuminoC",
        "Release": BUILD_DIR_RELEASE / "packages" / "LuminoC",
    }
    print("[build_wasm] artifact size report:", flush=True)
    for name in ("LuminoC.wasm", "LuminoC.mjs"):
        sizes: dict[str, int] = {}
        for label, src_dir in sources.items():
            p = src_dir / name
            if p.exists():
                sizes[label] = p.stat().st_size
        if not sizes:
            continue
        parts = [f"{label}={_human_size(size)}" for label, size in sizes.items()]
        line = f"[build_wasm]   {name}: " + ", ".join(parts)
        if "Debug" in sizes and "Release" in sizes and sizes["Debug"] > 0:
            ratio = sizes["Release"] / sizes["Debug"] * 100
            line += f" (Release = {ratio:.1f}% of Debug)"
        print(line, flush=True)


def _copy_native_binaries() -> None:
    if not NATIVE_BIN_SRC.is_dir():
        print(f"[build_wasm] warning: native bin directory not found: {NATIVE_BIN_SRC}")
        return
    NATIVE_BIN_DEST.mkdir(parents=True, exist_ok=True)
    for pattern in NATIVE_BIN_GLOBS:
        for src_file in sorted(NATIVE_BIN_SRC.glob(pattern)):
            dst_file = NATIVE_BIN_DEST / src_file.name
            shutil.copy2(src_file, dst_file)
            print(f"[build_wasm] copied {src_file.name} -> {dst_file}")


# C++のサンプルアセット（シェーダー、テクスチャ）を luminojs-examples 内にコピーします。
# public/に配置されたファイルは、ViteによってルートURLパスで処理されずに配信されるため、
# TypeScriptのサンプルでは「/foo.lcsh」のように参照できます。
def _copy_example_assets() -> None:
    if not EXAMPLE_ASSETS_SRC.is_dir():
        print(f"[build_wasm] warning: example assets directory not found: {EXAMPLE_ASSETS_SRC}")
        return
    EXAMPLE_ASSETS_DEST.mkdir(parents=True, exist_ok=True)
    for pattern in EXAMPLE_ASSET_GLOBS:
        for src_file in sorted(EXAMPLE_ASSETS_SRC.glob(pattern)):
            dst_file = EXAMPLE_ASSETS_DEST / src_file.name
            shutil.copy2(src_file, dst_file)
            print(f"[build_wasm] copied {src_file.name} -> {dst_file}")

#----------------------------------------------------------------------------
# main
#----------------------------------------------------------------------------
def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Build Lumino (Phase 0) for WebAssembly via Emscripten."
    )
    parser.add_argument(
        "action",
        nargs="?",
        default="all",
        choices=("all", "configure", "build", "clean"),
        help="Step to run (default: all)",
    )
    parser.add_argument(
        "--debug",
        action="store_true",
        help=(
            "Debug 構成でビルドする (最適化なし、ソースマップ付き)。"
            "未指定時は Release 構成 (-O2) でビルドする。"
        ),
    )
    args = parser.parse_args(argv)

    if args.action == "clean":
        cmd_clean(args.debug)
        return 0

    emsdk_root = resolve_emsdk()
    print(f"[build_wasm] using EMSDK: {emsdk_root}")
    print(f"[build_wasm] build type: {_build_type(args.debug)}")

    if args.action in ("configure", "all"):
        cmd_configure(emsdk_root, args.debug)
    if args.action in ("build", "all"):
        cmd_build(emsdk_root, args.debug)

    print("[build_wasm] done.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
