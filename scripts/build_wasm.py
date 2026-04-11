#!/usr/bin/env python3
"""Build Lumino for the web (Emscripten / WebAssembly).

Phase 0 responsibilities:
  - Use the Emscripten SDK installed at ``<repo>/emsdk`` (as documented in
    README.md: ``git clone -b 5.0.5 ... ./emsdk``).
  - Run ``emcmake cmake -S . -B build/wasm`` and then ``cmake --build``.
  - Copy the generated artifacts (``LuminoC.mjs``, ``LuminoC.wasm``,
    ``LuminoC.wasm.map``) into ``packages/luminojs/lib/`` so that the
    sandbox HTML can load them directly.

Only the Python standard library is used so that the script runs on any
machine that already has Python 3.8+ and the in-tree emsdk clone.

Usage:
    python scripts/build_wasm.py              # configure + build + copy
    python scripts/build_wasm.py all          # same as above
    python scripts/build_wasm.py configure    # cmake configure only
    python scripts/build_wasm.py build        # cmake --build + copy
    python scripts/build_wasm.py clean        # remove build/wasm and lib output
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
BUILD_DIR = REPO_ROOT / "build" / "wasm"
OUTPUT_DIR = REPO_ROOT / "packages" / "luminojs" / "lib"
# Emscripten SDK is always cloned into <repo>/emsdk per README.md.
EMSDK_ROOT = REPO_ROOT / "emsdk"

# Files produced by the Emscripten link step that should be shipped to the
# TypeScript package.
ARTIFACT_NAMES = ("LuminoC.mjs", "LuminoC.wasm", "LuminoC.wasm.map")


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
            "Clone emsdk into <repo>/emsdk as described in README.md:\n"
            "  git clone -b 5.0.5 https://github.com/emscripten-core/emsdk.git ./emsdk\n"
            "  ./emsdk/emsdk install 5.0.5\n"
            "  ./emsdk/emsdk activate 5.0.5"
        )
    return candidate


def resolve_emsdk() -> Path:
    """Return the in-tree emsdk root (``<repo>/emsdk``)."""
    if not EMSDK_ROOT.is_dir():
        raise SystemExit(
            f"emsdk directory not found at {EMSDK_ROOT}.\n"
            "Clone it into the repository root as described in README.md"
        )
    return EMSDK_ROOT


#----------------------------------------------------------------------------
# Commands
#----------------------------------------------------------------------------

def _run_with_emsdk(emsdk_root: Path, cmd_args: list[str]) -> None:
    """Run ``cmd_args`` in a shell that has ``emsdk_env`` activated.

    We do **not** try to capture emsdk's environment into Python and then
    forward it via ``subprocess.run(..., env=env)``; that approach is fragile
    on Windows because emsdk_env.bat dynamically generates a set_env script
    and any failure in that pipeline is hard to diagnose. Instead, we ask the
    OS shell to chain ``call emsdk_env.bat`` (or ``. emsdk_env.sh``) with the
    real command, so emsdk's environment lives only for the duration of the
    child shell.
    """
    env_script = _find_emsdk_env(emsdk_root)
    print(f"[build_wasm] $ {' '.join(cmd_args)}", flush=True)
    if os.name == "nt":
        # Windows quoting is a minefield. If we pass a Python list to
        # subprocess, ``list2cmdline`` would escape inner quotes with ``\"``
        # (a MSVCRT-style escape), but cmd.exe does NOT understand ``\"`` and
        # interprets it literally, producing errors like
        #   '"C:\...\emsdk_env.bat"' is not recognized ...
        # Instead, build the full cmd.exe command line ourselves and pass it
        # as a single STRING to subprocess.run. Python will forward the string
        # to CreateProcess verbatim (no further escaping), and cmd.exe's /s
        # flag treats the /c argument very simply: if the first and last char
        # are ``"``, strip them and run the rest as-is. That lets us keep
        # embedded quotes around paths-with-spaces without any escaping dance.
        inner = subprocess.list2cmdline(cmd_args)
        full = f'cmd.exe /s /c "call "{env_script}" >NUL && {inner}"'
        subprocess.run(full, check=True, cwd=str(REPO_ROOT))
    else:
        joined = " ".join(shlex.quote(a) for a in cmd_args)
        full = f'. "{env_script}" >/dev/null && {joined}'
        subprocess.run(["bash", "-c", full], check=True, cwd=str(REPO_ROOT))


def cmd_configure(emsdk_root: Path) -> None:
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    _run_with_emsdk(
        emsdk_root,
        [
            "emcmake",
            "cmake",
            "-S",
            str(REPO_ROOT),
            "-B",
            str(BUILD_DIR),
            "-G",
            "Ninja",
            "-DCMAKE_BUILD_TYPE=Debug",
            "-DCMAKE_MAKE_PROGRAM=C:/Proj/Lumino/vcpkg/downloads/tools/ninja-1.13.2-windows/ninja.exe",
        ],
    )


def cmd_build(emsdk_root: Path) -> None:
    if not (BUILD_DIR / "CMakeCache.txt").exists():
        cmd_configure(emsdk_root)
    _run_with_emsdk(emsdk_root, ["cmake", "--build", str(BUILD_DIR)])
    _copy_artifacts()


def cmd_clean() -> None:
    if BUILD_DIR.exists():
        print(f"[build_wasm] removing {BUILD_DIR}")
        shutil.rmtree(BUILD_DIR)
    if OUTPUT_DIR.exists():
        for name in ARTIFACT_NAMES:
            p = OUTPUT_DIR / name
            if p.exists():
                print(f"[build_wasm] removing {p}")
                p.unlink()


def _copy_artifacts() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    # Emscripten places outputs in the same directory as the executable target.
    # With the current CMake layout that is build/wasm/packages/LuminoC/.
    src_dir = BUILD_DIR / "packages" / "LuminoC"
    copied = 0
    for name in ARTIFACT_NAMES:
        src = src_dir / name
        if not src.exists():
            if name.endswith(".map"):
                continue  # source map is optional
            raise FileNotFoundError(f"Expected build artifact not found: {src}")
        dst = OUTPUT_DIR / name
        shutil.copy2(src, dst)
        print(f"[build_wasm] copied {src} -> {dst}")
        copied += 1
    if copied == 0:
        raise RuntimeError("No artifacts were copied. Did the build succeed?")


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
    args = parser.parse_args(argv)

    if args.action == "clean":
        cmd_clean()
        return 0

    emsdk_root = resolve_emsdk()
    print(f"[build_wasm] using EMSDK: {emsdk_root}")

    if args.action in ("configure", "all"):
        cmd_configure(emsdk_root)
    if args.action in ("build", "all"):
        cmd_build(emsdk_root)

    print("[build_wasm] done.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
