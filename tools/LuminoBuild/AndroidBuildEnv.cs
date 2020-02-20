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
            var toolsDir = BuildEnvironment.BuildToolsDir;


            if (Utils.IsWin32)
            {
                //var cloneParentDir = Path.Combine(builder.LuminoBuildDir, "Emscripten");

                //string localAppDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
                //AndroidSdkRootDir = Path.Combine(localAppDir, @"Android\Sdk");

                AndroidSdkRootDir = Path.Combine(toolsDir, "android-sdk");

                AndroidSdkCMake = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\cmake.exe");
                AndroidSdkNinja = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\ninja.exe");

                AndroidNdkRootDir = Path.Combine(AndroidSdkRootDir, "ndk-bundle");
                AndroidCMakeToolchain = Path.Combine(AndroidNdkRootDir, @"build\cmake\android.toolchain.cmake");
            }

            // Install Android SDK
            if (BuildEnvironment.IsAndroidTarget && Utils.IsWin32)
            {
                if (!Directory.Exists(AndroidSdkRootDir))
                {
                    var zip = Path.Combine(toolsDir, "android-sdk-tools.zip");
                    if (Utils.IsWin32)
                        Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-windows-4333796.zip", zip);
                    else if (Utils.IsMac)
                        Utils.DownloadFile("https://dl.google.com/android/repository/sdk-tools-darwin-4333796.zip", zip);

                    Utils.ExtractZipFile(zip, AndroidSdkRootDir);

                    var javaHome = Path.Combine(toolsDir, "emsdk", "java", "8.152_64bit", "bin");
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
                    var zip = Path.Combine(toolsDir, "android-ndk-r18b.zip");
                    // https://developer.android.com/ndk/downloads/older_releases
                    if (Utils.IsWin32)
                        Utils.DownloadFile("https://dl.google.com/android/repository/android-ndk-r18b-windows-x86_64.zip", zip);
                    else if (Utils.IsMac)
                        Utils.DownloadFile("https://dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip", zip);

                    Console.WriteLine("Extracting Android NDK...");
                    var tmpDir = Path.Combine(toolsDir, "android-ndk-r18b");
                    Utils.ExtractZipFile(zip, tmpDir);
                    Directory.Move(Path.Combine(tmpDir, "android-ndk-r18b"), ndkDir);
                }

            }


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
