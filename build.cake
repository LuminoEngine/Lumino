#tool nuget:?package=NUnit.ConsoleRunner&version=3.4.0
//////////////////////////////////////////////////////////////////////
// ARGUMENTS
//////////////////////////////////////////////////////////////////////

var target = Argument("target", "Default");
var configuration = Argument("configuration", "Release");

//////////////////////////////////////////////////////////////////////
// PREPARATION
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// TASKS
//////////////////////////////////////////////////////////////////////

Task("Clean")
    .Does(() =>
{
});

Task("Restore-NuGet-Packages")
    .IsDependentOn("Clean")
    .Does(() =>
{
});

Task("Build-LuminoBuild")
    .IsDependentOn("Clean")
    .Does(() =>
{
    if(IsRunningOnWindows())
    {
      // Use MSBuild
      MSBuild("./Build/LuminoBuild.sln", settings =>
        settings.SetConfiguration(configuration)
            .SetPlatformTarget(PlatformTarget.x86));
    }
    else
    {
      // Use XBuild
      XBuild("./Build/LuminoBuild.sln", settings =>
        settings.SetConfiguration(configuration));
    }
});

Task("Make-Projects")
    .IsDependentOn("Build-LuminoBuild")
    .Does(() =>
{
    StartProcess("./Build/LuminoBuild/bin/Release/LuminoBuild.exe", "MakeVSProjects");
});

Task("Build")
    .IsDependentOn("Build-LuminoBuild")
    .Does(() =>
{
    StartProcess("./Build/LuminoBuild/bin/Release/LuminoBuild.exe");
});

Task("Run-Unit-Tests")
    .IsDependentOn("Build")
    .Does(() =>
{
});

Task("Publish")
    .IsDependentOn("Run-Unit-Tests")
    .Does(() =>
{
    StartProcess("./Build/LuminoBuild/bin/Release/LuminoBuild.exe MakeInstaller");
});

//////////////////////////////////////////////////////////////////////
// TASK TARGETS
//////////////////////////////////////////////////////////////////////

Task("Default")
    .IsDependentOn("Run-Unit-Tests");


//////////////////////////////////////////////////////////////////////
// EXECUTION
//////////////////////////////////////////////////////////////////////

RunTarget(target);
