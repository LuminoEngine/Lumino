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

	Project* m_project;
};
