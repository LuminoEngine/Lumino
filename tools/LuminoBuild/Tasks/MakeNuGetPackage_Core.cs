using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class MakeNuGetPackage_Core : BuildTask
    {
        public override string CommandName => "MakeNuGetPackage_Core";

        public override string Description => "MakeNuGetPackage_Core";

        public override void Build(Builder builder)
        {
            var nugetroot = Path.Combine(builder.LuminoBuildDir, "NuGetPackage-LuminoCore");
            var nativeDir = Path.Combine(builder.LuminoBuildDir, "NuGetPackage-LuminoCore", "build", "native");
            Directory.CreateDirectory(nativeDir);

            var files = new List<(string src, string dst)>()
            {
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x86-MD", "lib", "Debug", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x86-MD", "Debug", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x86-MD", "lib", "Release", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x86-MD", "Release", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x86-MT", "lib", "Debug", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x86-MT", "Debug", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x86-MT", "lib", "Release", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x86-MT", "Release", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x64-MD", "lib", "Debug", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x64-MD", "Debug", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x64-MD", "lib", "Release", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x64-MD", "Release", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x64-MT", "lib", "Debug", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x64-MT", "Debug", "LuminoCore.lib")
                ),
                (
                    Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "MSVC2017-x64-MT", "lib", "Release", "LuminoCore.lib"),
                    Path.Combine(nativeDir, "lib", "MSVC2017-x64-MT", "Release", "LuminoCore.lib")
                ),
                // nuget --------
                (
                    Path.Combine(builder.LuminoRootDir, "src", "LuminoCore", "NuGet", "Lumino.Core.props"),
                    Path.Combine(nativeDir, "Lumino.Core.props")
                ),
                (
                    Path.Combine(builder.LuminoRootDir, "src", "LuminoCore", "NuGet", "Lumino.Core.targets"),
                    Path.Combine(nativeDir, "Lumino.Core.targets")
                ),
                (
                    Path.Combine(builder.LuminoRootDir, "src", "LuminoCore", "NuGet", "Lumino.Core.nuspec"),
                    Path.Combine(nugetroot, "Lumino.Core.nuspec")
                ),
            };

            Utils.CopyDirectory(
                Path.Combine(builder.LuminoRootDir, "src", "LuminoCore", "include"),
                Path.Combine(nativeDir, "include"));

            foreach (var pair in files)
            {
                Directory.CreateDirectory(Path.GetDirectoryName(pair.dst));
                File.Copy(pair.src, pair.dst, true);
            }

            Directory.SetCurrentDirectory(nugetroot);
            Utils.CallProcess("nuget", "pack Lumino.Core.nuspec");
        }
    }
}
