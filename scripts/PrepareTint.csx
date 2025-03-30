// https://commondatastorage.googleapis.com/chrome-infra-docs/flat/depot_tools/docs/html/depot_tools_tutorial.html#_setting_up


var Ninja = "vcpkg/downloads/tools/ninja/1.10.2-windows/ninja.exe";

Console.WriteLine("Building Dawn... (Ninja: " + Ninja + ")");

Directory.SetCurrentDirectory("build/tools");
//Process.Start("git", "clone -b chromium/7091 https://dawn.googlesource.com/dawn").WaitForExit();
Process.Start("git", "clone https://chromium.googlesource.com/chromium/tools/depot_tools.git").WaitForExit();

// PATH に depot_tools を追加
var deptToolsDir = Path.Combine(Directory.GetCurrentDirectory(), "depot_tools");
Environment.SetEnvironmentVariable("PATH", Environment.GetEnvironmentVariable("PATH") + ";" + deptToolsDir);

/*
cd build/tools
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
$ENV:Path+=";C:/Proj/LN/Luminovcpkg/downloads/tools/ninja/1.10.2-windows/ninja.exe"
*/


/*
mkdir -p out/Debug
cd out/Debug
cmake -GNinja ../..
ninja # or autoninja
*/
