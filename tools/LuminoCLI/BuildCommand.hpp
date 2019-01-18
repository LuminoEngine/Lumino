#pragma once

class BuildCommand
{
public:
	ln::String target;

    int execute(Workspace* workspace, Project* project);

private:
	Result buildWindowsTarget(Workspace* workspace);
	Result buildWebTarget(Workspace* workspace);
	Result buildAndroidTarget();
	Result buildAssets();

	Result buildAsset_Shader(const ln::Path& inputFile, ln::Path* outputFile);

	Project* m_project;
};
