#pragma once

class BuildCommand
{
public:
	bool package = false;
	ln::String target;

    int execute(Workspace* workspace, Project* project);

private:
	Result buildWindowsTarget(Workspace* workspace, bool debug);
	Result buildWindowsPackage(Project* project);
	Result buildWebTarget(Workspace* workspace);
	Result buildAndroidTarget();
	Result buildAssets();

	Result buildAsset_Shader(const ln::Path& inputFile, ln::Path* outputFile);

	Project* m_project;
};
