using System;
using System.IO;
using System.Linq;
using LuminoBuild;


namespace LuminoBuild.Tasks
{
    class BuildDocuments : BuildTask
    {
        public override string CommandName => "BuildDocuments";

        public override void Build(Builder builder)
        {
            Directory.SetCurrentDirectory(Path.Combine(builder.LuminoDocDir, "Doxygen"));
            Utils.TryCallProcess("doxygen", "Doxyfile");
            Utils.CopyDirectory("theme", "html");
            File.Copy("../Assets/icon64.png", "html/icon64.png", true);

            // doxygen の出力を docfx の出力フォルダにコピーする
            Directory.SetCurrentDirectory(builder.LuminoDocDir);
            Utils.CopyDirectory(Path.Combine("Doxygen", "html"), Path.Combine(builder.LuminoDocDir, "WebDocument", "_site", "articles", "reference-cpp"));

            Directory.SetCurrentDirectory(Path.Combine(builder.LuminoDocDir, "WebDocument"));
            Utils.CallProcess("docfx", "build");
        }
    }
}
