using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{
    class AndoridBuildEnv
    {
        public const string AndroidTargetPlatform = "android-26";
        public static bool AndroidStudioFound { get; set; }
        public static string AndroidSdkRootDir { get; set; }
        public static string AndroidSdkCMake { get; set; }
        public static string AndroidSdkNinja { get; set; }
        public static string AndroidNdkRootDir { get; set; }
        public static string AndroidCMakeToolchain { get; set; }


        public static void Initialize(Builder builder)
        {
            string buildCacheDir = Path.Combine(builder.LuminoBuildDir, "BuildCache");


            if (Utils.IsWin32)
            {
                AndroidSdkRootDir = Path.GetFullPath(Path.Combine(buildCacheDir, "android-sdk"));

                AndroidSdkCMake = Path.Combine(AndroidSdkRootDir, @"cmake\3.10.2.4988404\bin\cmake.exe");
                AndroidSdkNinja = Path.Combine(AndroidSdkRootDir, @"cmake\3.10.2.4988404\bin\ninja.exe");

                AndroidNdkRootDir = Path.Combine(AndroidSdkRootDir, "ndk", "22.0.7026061");
                AndroidCMakeToolchain = Path.Combine(AndroidNdkRootDir, @"build\cmake\android.toolchain.cmake");
            }


            // Install Android SDK
            if (BuildEnvironment.IsAndroidTarget && Utils.IsWin32)
            {
                var javaHome = Path.Combine(builder.LuminoBuildDir, "Emscripten", "emsdk", "java", "8.152_64bit");
                var javaPath = Path.Combine(javaHome, "bin");
                var skdmanager = Path.Combine(AndroidSdkRootDir, "tools", "bin", (Utils.IsWin32) ? "sdkmanager.bat" : "sdkmanager");
                var env = new Dictionary<string, string>()
                {
                    { "PATH", javaPath + ";"+ Environment.GetEnvironmentVariable("PATH") },
                    { "JAVA_HOME", javaHome },
                };

                if (!Directory.Exists(AndroidSdkRootDir))
                {
                    var zip = Path.Combine(buildCacheDir, "android-commandlinetools.zip");
                    Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-windows-4333796.zip", zip);
                    Utils.ExtractZipFile(zip, AndroidSdkRootDir);


                    if (!Utils.IsWin32)
                        Utils.chmod(skdmanager, Utils.S_0755);


                    Utils.CallProcess("java", $"-version", env);
                    Utils.CallProcess(skdmanager, $"ndk;22.0.7026061 --sdk_root={AndroidSdkRootDir}", env, (stdin) => stdin.WriteLine("y"));
                    Utils.CallProcess(skdmanager, $"cmake;3.10.2.4988404 --sdk_root={AndroidSdkRootDir}", env, (stdin) => stdin.WriteLine("y"));
                }
            }

#if false

            // Install Android SDK
            if (BuildEnvironment.IsAndroidTarget && Utils.IsWin32)
            {
                if (!Directory.Exists(AndroidSdkRootDir))
                {
                    var zip = Path.Combine(buildCacheDir, "android-sdk-tools.zip");
                    if (Utils.IsWin32)
                        Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-windows-4333796.zip", zip);
                    else if (Utils.IsMac)
                        Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-darwin-4333796.zip", zip);

                    Utils.ExtractZipFile(zip, AndroidSdkRootDir);

                    var javaHome = Path.Combine(buildCacheDir, "emsdk", "java", "8.152_64bit", "bin");
                    var skdmanager = Path.Combine(AndroidSdkRootDir, "tools", "bin", (Utils.IsWin32) ? "sdkmanager.bat" : "sdkmanager");


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
                var ndkDir = Path.Combine(AndroidSdkRootDir, "ndk-bundle");
                if (!Directory.Exists(ndkDir))
                {
                    Console.WriteLine("Downloading Android NDK...");
                    var zip = Path.Combine(buildCacheDir, "android-ndk-r18b.zip");
                    // https://developer.android.com/ndk/downloads/older_releases
                    if (Utils.IsWin32)
                       Utils.DownloadFile("https://dl.google.com/android/repository/android-ndk-r18b-windows-x86_64.zip", zip);
                    else if (Utils.IsMac)
                       Utils.DownloadFile("https://dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip", zip);

                    Console.WriteLine("Extracting Android NDK...");
                    var tmpDir = Path.Combine(buildCacheDir, "android-ndk-r18b");
                    Utils.ExtractZipFile(zip, tmpDir);
                    Directory.Move(Path.Combine(tmpDir, "android-ndk-r18b"), ndkDir);
                }

            }

#endif
            if (Directory.Exists(AndroidSdkRootDir))
            {
                AndroidStudioFound = true;
            }
        }

        public static string GetABIFromTargetName(string targetName)
        {
            return targetName.Replace("Android-", "");
        }
    }
}
