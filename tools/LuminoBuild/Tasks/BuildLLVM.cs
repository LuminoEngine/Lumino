using System;
using System.IO;
using System.Linq;
using LuminoBuild;


namespace LuminoBuild.Tasks
{
    class BuildLLVM : BuildTask
    {
        public override string CommandName { get { return "BuildLLVM"; } }

        public override string Description { get { return "BuildLLVM"; } }

        public override void Build(Builder builder)
        {
            var reposDir = Path.Combine(builder.LuminoBuildDir, "BuildTools");
            Directory.SetCurrentDirectory(reposDir);

            if (!Directory.Exists("llvm"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b release_60 https://github.com/llvm-mirror/llvm.git llvm");
            }

            Directory.SetCurrentDirectory(Path.Combine(reposDir, "llvm", "tools"));
            if (!Directory.Exists("clang"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b release_60 https://github.com/llvm-mirror/clang.git clang");
            }
            Directory.SetCurrentDirectory(reposDir);

            var repoRoot = Path.Combine(reposDir, "llvm");
            var packageDir = Path.Combine(reposDir, "LLVMPackage");
            BuildProject(repoRoot, Path.Combine(repoRoot, "_Build"), Path.Combine(packageDir));
            //BuildProject(repoRoot, Path.Combine(repoRoot, "_Build-Debug"), Path.Combine(packageDir, "Debug"), "Debug");
            //BuildProject(repoRoot, Path.Combine(repoRoot, "_Build-Release"), Path.Combine(packageDir, "Release"), "Release");
        }

        public void BuildProject(string cmakeHomeDir, string buildDir, string installDir)
        {
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            
            var args = new string[]
            {
                $"{cmakeHomeDir}",
                $"-G\"Visual Studio 15 2017\"",
                $"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
                $"-DCMAKE_DEBUG_POSTFIX=d",
            };

            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config Debug --target install");
            Utils.CallProcess("cmake", $"--build . --config Release --target install");
        }
    }
}
