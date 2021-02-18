using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEmbeddedResources : BuildTask
    {
        public override string CommandName => "BuildEmbeddedResources";

        public override void Build(Builder builder)
        {
            var searchDirs = new string[]
            {
                Path.Combine(builder.LuminoSourceDir, "LuminoEngine", "src"),
            };

            foreach (var searchDir in searchDirs)
            {
                BuildFXH(builder, searchDir);
            }
            foreach (var searchDir in searchDirs)
            {
                BuildFX(builder, searchDir);
            }
        }

        private void BuildFX(Builder builder, string searchDir)
        {
            var compiler = Path.Combine(builder.LuminoRootDir, "build/MSVC2019-x64-MT/EngineBuild/src/LuminoEditor/CLI/Debug/lumino.exe");

            foreach (var file in Directory.EnumerateFiles(searchDir, "*.fx", SearchOption.AllDirectories))
            {
                Console.WriteLine(file);

                var output = Path.Combine(Path.GetDirectoryName(file), Path.GetFileNameWithoutExtension(file) + ".lcfx");
                Utils.CallProcess(compiler, $"fxc {file} {output}");
                BinaryToHexCSVHeader(builder, output);

                Console.WriteLine("  -> " + output);
            }
        }

        private void BuildFXH(Builder builder, string searchDir)
        {
            foreach (var file in Directory.EnumerateFiles(searchDir, "*.fxh", SearchOption.AllDirectories))
            {
                Console.WriteLine(file);

                BinaryToHexCSVHeader(builder, file);

                Console.WriteLine("  -> " + file + ".inl");
            }
        }

        private void BinaryToHexCSVHeader(Builder builder, string file)
        {
            var csv = Path.Combine(builder.LuminoRootDir, "tools/BinaryToIntArray/BinaryToIntArray.rb");
            Utils.CallProcess("ruby", $"{csv} {file}");
        }
    }
}
