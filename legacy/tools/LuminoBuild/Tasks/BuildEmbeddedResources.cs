using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEmbeddedResources : BuildTask
    {
        public override string CommandName => "BuildEmbeddedResources";

        public override void Build(Build builder)
        {
            var searchDirs = new string[]
            {
                Path.Combine(builder.RootDir, "lumino", "LuminoEngine", "src"),
                Path.Combine(builder.RootDir, "lumino", "Graphics", "src"),
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

        private void BuildFX(Build builder, string searchDir)
        {
            var compiler = Path.Combine(builder.RootDir, "build/buildtrees/x64-windows-static/lumino/src/Editor/CLI/Debug/lumino.exe");

            foreach (var file in Directory.EnumerateFiles(searchDir, "*.fx", SearchOption.AllDirectories))
            {
                Console.WriteLine(file);

                var output = Path.Combine(Path.GetDirectoryName(file), Path.GetFileNameWithoutExtension(file) + ".lcfx");
                Utils.CallProcess(compiler, $"fxc {file} {output}");
                BinaryToHexCSVHeader(builder, output);

                Console.WriteLine("  -> " + output);
            }
        }

        private void BuildFXH(Build builder, string searchDir)
        {
            foreach (var file in Directory.EnumerateFiles(searchDir, "*.fxh", SearchOption.AllDirectories))
            {
                Console.WriteLine(file);

                BinaryToHexCSVHeader(builder, file);

                Console.WriteLine("  -> " + file + ".inl");
            }
        }

        private void BinaryToHexCSVHeader(Build builder, string file)
        {
            var compiler = Path.Combine(builder.RootDir, "build/buildtrees/x64-windows-static/lumino/src/Editor/CLI/Debug/lumino.exe");
            Utils.CallProcess(compiler, $"bin2inl {file}");
        }
    }
}
