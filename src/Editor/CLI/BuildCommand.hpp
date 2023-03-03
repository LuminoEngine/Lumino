#pragma once

class BuildCommand
{
public:
	bool package = false;
	ln::String target;

    int execute(lna::Workspace* workspace, lna::Project* project);

private:
	ln::String selectDefaultTarget() const;

    ln::Result<> buildWindowsTarget(lna::Workspace* workspace, bool debug);
    ln::Result<> buildWindowsPackage(lna::Project* project);
    ln::Result<> buildWebTarget(lna::Workspace* workspace);
    ln::Result<> buildAndroidTarget();
    ln::Result<> buildAssets();

    ln::Result<> buildAsset_Shader(const ln::Path& inputFile, ln::Path* outputFile);

    lna::Project* m_project;
};
