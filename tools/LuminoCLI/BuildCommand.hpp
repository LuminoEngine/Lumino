#pragma once

class BuildCommand
{
public:
	bool package = false;
	ln::String target;

    int execute(Workspace* workspace, Project* project);

private:
    ln::Result buildWindowsTarget(Workspace* workspace, bool debug);
    ln::Result buildWindowsPackage(Project* project);
    ln::Result buildWebTarget(Workspace* workspace);
    ln::Result buildAndroidTarget();
    ln::Result buildAssets();

    ln::Result buildAsset_Shader(const ln::Path& inputFile, ln::Path* outputFile);

	Project* m_project;
};
