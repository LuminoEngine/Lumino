using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{
    class TargetInfo
    {
        public string Name { get; set; }
        public string LibraryExt { get; set; }
        public bool PdbCopy { get; set; } = false;
    }

    class BuildEnvironment
    {
        public const string VSWhereUrl = @"https://github.com/Microsoft/vswhere/releases/download/2.5.2/vswhere.exe";

        public const string EngineInstallDirName = "EngineInstall";

        public static string BuildToolsDir { get; set; }


        // Build settings
        public static string Target { get; set; }
        public static string Configuration { get; set; }
        public static string TargetFullName { get { return (string.IsNullOrEmpty(Configuration)) ? Target : $"{Target}-{BuildEnvironment.Configuration}"; } }

        // Build setting utils.
        public static bool IsMSVCTarget { get { return Target.Contains("MSVC"); } }
        public static bool IsWebTarget { get { return Target == "Emscripten"; } }
        public static bool IsAndroidTarget { get { return Target.Contains("Android"); } }
        public static bool IsMacOSTarget { get { return Target.Contains("macOS"); } }
        public static bool IsIOSTarget { get { return Target.Contains("iOS"); } }

        public static TargetInfo[] Targets = new TargetInfo[]
        {
            new TargetInfo(){ Name = "MSVC2017-x64-MT", LibraryExt = "*.lib", PdbCopy = true },
            new TargetInfo(){ Name = "MSVC2019-x64-MT", LibraryExt = "*.lib", PdbCopy = true },
            new TargetInfo(){ Name = "Emscripten", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "Android-arm64-v8a", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "Android-armeabi-v7a", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "Android-x86", LibraryExt = "*.a"},
            new TargetInfo(){ Name = "Android-x86_64", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "macOS", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "iOS-SIMULATOR64", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "iOS-OS", LibraryExt = "*.a" },
        };

        public static void Initialize(Builder builder)
        {
            BuildToolsDir = Path.Combine(builder.LuminoRootDir, "build", "BuildTools");


            InstallTools(builder);
        }

        private static void InstallTools(Builder builder)
        {
            // BuildExternalProjects のものたちは cmake install などで、各 Target ごとの ExternalInstall に入り、このフォルダは CI によりキャッシュされる。
            // しかし一部の、主に Header-only-library はリポジトリ内のファイルを直接参照するため、キャッシュされる別のフォルダに clone する。
            // (ExternalSource フォルダは全体で 5GB を超えるため、GitHub Actions の cache size に引っかかる。そのため必要なものだけ別フォルダに逃がす)
            string buildCacheDir = Path.Combine(builder.LuminoBuildDir, "BuildCache");
            if (!builder.ExistsCache(buildCacheDir))
            {
                Directory.CreateDirectory(buildCacheDir);
                using (CurrentDir.Enter(buildCacheDir))
                {
                    if (!Directory.Exists("stb"))
                    {
                        Utils.CallProcess("git", "clone https://github.com/nothings/stb.git stb");
                        Utils.CallProcess("git", "-C stb checkout e6afb9cbae4064da8c3e69af3ff5c4629579c1d2");
                    }

                    if (!Directory.Exists("imgui"))
                    {
                        Utils.CallProcess("git", "clone --depth 1 -b v1.72 https://github.com/ocornut/imgui.git imgui");
                    }

                    if (!Directory.Exists("Streams"))
                    {
                        Utils.CallProcess("git", "clone https://github.com/jscheiny/Streams.git Streams");

                        using (CurrentDir.Enter("Streams"))
                        {
                            Utils.CallProcess("git", "checkout 8fc0657b977cfd8f075ad0afb4dca3800630b56c");
                        }
                    }

                    if (!Directory.Exists("tinyobjloader"))
                    {
                        // v1.0.6 より後はタグが降られていないが、頂点カラーなどの対応が入っている。それを持ってくる。
                        Utils.CallProcess("git", "clone https://github.com/syoyo/tinyobjloader.git tinyobjloader");

                        using (CurrentDir.Enter("tinyobjloader"))
                        {
                            Utils.CallProcess("git", "checkout f37fed32f3eb0912cc10a970f78774cd98598ef6");
                        }
                    }

                    if (!Directory.Exists("tinygltf"))
                    {
                        Utils.CallProcess("git", "clone --depth 1 -b v2.2.0 https://github.com/syoyo/tinygltf.git tinygltf");
                    }
                }

                builder.CommitCache(buildCacheDir);
            }


            Directory.CreateDirectory(BuildToolsDir);
        }
    }
}
