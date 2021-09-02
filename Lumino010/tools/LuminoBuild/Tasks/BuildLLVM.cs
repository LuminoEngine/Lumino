using System;
using System.IO;
using System.Linq;
using LuminoBuild;


namespace LuminoBuild.Tasks
{
    class BuildLLVM : BuildTask
    {
        public override string CommandName => "BuildLLVM";

        public override void Build(Builder builder)
        {
            var reposDir = Path.Combine(builder.LuminoBuildDir, "BuildTools");
            Directory.SetCurrentDirectory(reposDir);

            if (!Directory.Exists("llvm-project"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b llvmorg-7.0.0 https://github.com/llvm/llvm-project.git llvm-project");
                //Utils.CallProcess("git", "clone --progress --depth 1 -b llvmorg-7.0.1 https://github.com/llvm/llvm-project.git llvm-project");
                //Utils.CallProcess("git", "clone --progress --depth 1 -b llvmorg-8.0.0-rc5 https://github.com/llvm/llvm-project.git llvm-project");
            }

            /* 
            Directory.SetCurrentDirectory(Path.Combine(reposDir, "llvm", "tools"));
            if (!Directory.Exists("clang"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b release_80 https://github.com/llvm-mirror/clang.git clang");
            }
            Directory.SetCurrentDirectory(reposDir);
            */
            var repoRoot = Path.Combine(reposDir, "llvm-project", "llvm");
            var buildDir = Path.Combine(builder.LuminoBuildDir, "MSVC2019-x64-MT", "llvm-build");
            var installDir = Path.Combine(builder.LuminoBuildDir, "MSVC2019-x64-MT", "BuildToolsInstall", "llvm");
            BuildProject(repoRoot, buildDir, installDir);
        }

        public void BuildProject(string cmakeHomeDir, string buildDir, string installDir)
        {
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            
            var args = new string[]
            {
                $"{cmakeHomeDir}",
                $"-G\"Visual Studio 16 2019\"",
                $"-A x64",
                //$"-A Win32",
                //$"-A Win64",
                $"-Thost=x64",  // required http://clang.llvm.org/get_started.html
                $"-DLLVM_ENABLE_PROJECTS=clang",
                $"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
                $"-DCMAKE_DEBUG_POSTFIX=d",
                $"-DCMAKE_CXX_FLAGS=\"/MP\"",
                // 以下、デフォルトONだけどいらなそうなのでOFF
                // https://www.llvm.org/docs/CMake.html
                //$"-DCLANG_BUILD_TOOLS=OFF",
                $"-DCLANG_INCLUDE_DOCS=OFF",
                $"-DCLANG_INCLUDE_TESTS=OFF",
                //$"-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY",
                //$"-DLLVM_USE_SANITIZER=Memory",
/*
                $"-DLLVM_ADD_NATIVE_VISUALIZERS_TO_SOLUTION=OFF",
                $"-DLLVM_BUILD_RUNTIME=OFF",
                $"-DLLVM_BUILD_RUNTIMES=OFF",
                $"-DLLVM_BUILD_TOOLS=OFF",
                $"-DLLVM_BUILD_UTILS=OFF",
                $"-DLLVM_ENABLE_BACKTRACES=OFF",
                $"-DLLVM_ENABLE_BINDINGS=OFF",
                $"-DLLVM_ENABLE_OCAMLDOC=OFF",
                $"-DLLVM_ENABLE_PEDANTIC=OFF",
                $"-DLLVM_ENABLE_PIC=OFF",
                $"-DLLVM_INCLUDE_BENCHMARKS=OFF",
                $"-DLLVM_INCLUDE_DOCS=OFF",
                $"-DLLVM_INCLUDE_EXAMPLES=OFF",
                $"-DLLVM_INCLUDE_GO_TESTS=OFF",
                $"-DLLVM_INCLUDE_RUNTIMES=OFF",
                 */
                //$"-DLLVM_INCLUDE_TESTS=OFF",
                //$"-DLLVM_INCLUDE_TOOLS=OFF",
                //$"-DLLVM_INCLUDE_UTILS=OFF",
                //$"-DLLVM_POLLY_BUILD=OFF",
                //$"-DLLVM_POLLY_LINK_INTO_TOOLS=OFF",
                $"-DLLVM_USE_CRT_DEBUG=MTd",
                $"-DLLVM_USE_CRT_RELEASE=MT",
                $"-DLLVM_USE_CRT_MINSIZEREL=MT",
                $"-DLLVM_USE_CRT_RELWITHDEBINFO=MT",
            };

            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config Debug --target install");
            Utils.CallProcess("cmake", $"--build . --config Release --target install");
        }
    }
}
