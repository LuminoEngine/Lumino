
var Ninja = "vcpkg/downloads/tools/ninja/1.10.2-windows/ninja.exe";

Console.WriteLine("Building Dawn... (Ninja: " + Ninja + ")");

Directory.SetCurrentDirectory("build/tools");
//Process.Start("git", "clone -b chromium/7091 https://dawn.googlesource.com/dawn").WaitForExit();
Process.Start("git", "clone -b dawn-7069 https://github.com/eliemichel/WebGPU-distribution.git").WaitForExit();


/*
mkdir -p out/Debug
cd out/Debug
cmake -GNinja ../..
ninja # or autoninja
*/
