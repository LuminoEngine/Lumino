using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{
    public class TargetInfo
    {
        public string Name { get; set; }
        public string LibraryExt { get; set; }
        public bool PdbCopy { get; set; } = false;
    }

    public class BuildEnvironment
    {
        public const string VSWhereUrl = @"https://github.com/Microsoft/vswhere/releases/download/2.5.2/vswhere.exe";

        public const string EngineInstallDirName = "EngineInstall";

        public static string BuildToolsDir { get; set; }

        public const string AndroidTargetPlatform = "android-26";
        public static bool AndroidStudioFound { get; set; }
        public static string AndroidSdkRootDir { get; set; }
        public static string AndroidSdkCMake { get; set; }
        public static string AndroidSdkNinja { get; set; }
        public static string AndroidNdkRootDir { get; set; }
        public static string AndroidCMakeToolchain { get; set; }

        // Build settings
        public static string Target { get; set; }
        public static string Configuration { get; set; }
        public static string TargetFullName { get { return (string.IsNullOrEmpty(Configuration)) ? Target : $"{Target}-{BuildEnvironment.Configuration}"; } }

        // Build setting utils.
        public static bool IsMSVCTarget { get { return Target.Contains("MSVC2017"); } }
        public static bool IsWebTarget { get { return Target == "Emscripten"; } }
        public static bool IsAndroidTarget { get { return Target.Contains("Android"); } }
        public static bool IsMacOSTarget { get { return Target.Contains("macOS"); } }
        public static bool IsIOSTarget { get { return Target.Contains("iOS"); } }

        public static TargetInfo[] Targets = new TargetInfo[]
        {
            new TargetInfo(){ Name = "MSVC2017-x86-MD", LibraryExt = "*.lib", PdbCopy = true },
            new TargetInfo(){ Name = "MSVC2017-x86-MT", LibraryExt = "*.lib", PdbCopy = true },
            new TargetInfo(){ Name = "MSVC2017-x64-MD", LibraryExt = "*.lib", PdbCopy = true },
            new TargetInfo(){ Name = "MSVC2017-x64-MT", LibraryExt = "*.lib", PdbCopy = true },
            new TargetInfo(){ Name = "Emscripten", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "Android-arm64-v8a", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "Android-armeabi-v7a", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "Android-x86", LibraryExt = "*.a"},
            new TargetInfo(){ Name = "Android-x86_64", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "macOS", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "iOS-SIMULATOR64", LibraryExt = "*.a" },
            new TargetInfo(){ Name = "iOS-OS", LibraryExt = "*.a" },
        };

        public static void Initialize(string repoRootDir)
        {
            BuildToolsDir = Path.Combine(repoRootDir, "build", "BuildTools");

            if (Utils.IsWin32)
            {
                //string localAppDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
                //AndroidSdkRootDir = Path.Combine(localAppDir, @"Android\Sdk");
                AndroidSdkRootDir = Path.Combine(BuildToolsDir, "android-sdk");

                AndroidSdkCMake = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\cmake.exe");
                AndroidSdkNinja = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\ninja.exe");

                AndroidNdkRootDir = Path.Combine(AndroidSdkRootDir, "ndk-bundle");
                AndroidCMakeToolchain = Path.Combine(AndroidNdkRootDir, @"build\cmake\android.toolchain.cmake");
            }

            InstallTools(repoRootDir);
        }

        private static void InstallTools(string repoRootDir)
        {
            Directory.CreateDirectory(BuildToolsDir);

            // Install Android SDK
            if (IsAndroidTarget && Utils.IsWin32)
            {
                var androidSdk = Path.Combine(BuildToolsDir, "android-sdk");
                if (!Directory.Exists(androidSdk))
                {
                    var zip = Path.Combine(BuildToolsDir, "android-sdk-tools.zip");
                    if (Utils.IsWin32)
                        Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-windows-4333796.zip", zip);
                    else if (Utils.IsMac)
                        Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-darwin-4333796.zip", zip);

                    Utils.ExtractZipFile(zip, androidSdk);

                    var javaHome = Path.Combine(BuildToolsDir, "emsdk", "java", "8.152_64bit", "bin");
                    var skdmanager = Path.Combine(androidSdk, "tools", "bin", (Utils.IsWin32) ? "sdkmanager.bat" : "sdkmanager");


                    if (!Utils.IsWin32)
                        Utils.chmod(skdmanager, Utils.S_0755);

                    var env = new Dictionary<string, string>()
                    {
                        { "PATH", javaHome + ";"+ Environment.GetEnvironmentVariable("PATH") },
                    };
                    
                    Utils.CallProcess(skdmanager, "cmake;3.6.4111459", env, (stdin) => stdin.WriteLine("y"));
                    //Utils.CallProcess(skdmanager, "ndk-bundle", env, (stdin) => stdin.WriteLine("y"));
                }

                // sdkmanager でインストールできるのは最新版のみ。
                // r19 では find_package(zlib) が失敗していて、その時点 (2019/1/18) では不具合情報も無いので対策しようがなかった。
                // そのため r18 を直接ダウンロードして配置する。
                var ndkDir = Path.Combine(androidSdk, "ndk-bundle");
                if (!Directory.Exists(ndkDir))
                {
                    Console.WriteLine("Downloading Android NDK...");
                    var zip = Path.Combine(BuildToolsDir, "android-ndk-r18b.zip");
                    // https://developer.android.com/ndk/downloads/older_releases
                    if (Utils.IsWin32)
                        Utils.DownloadFile("https://dl.google.com/android/repository/android-ndk-r18b-windows-x86_64.zip", zip);
                    else if (Utils.IsMac)
                        Utils.DownloadFile("https://dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip", zip);

                    Console.WriteLine("Extracting Android NDK...");
                    var tmpDir = Path.Combine(BuildToolsDir, "android-ndk-r18b");
                    Utils.ExtractZipFile(zip, tmpDir);
                    Directory.Move(Path.Combine(tmpDir, "android-ndk-r18b"), ndkDir);
                }

                AndroidStudioFound = true;
            }
        }
    }
}
