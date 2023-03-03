#pragma once

namespace lna {
class Workspace;
class Project;

class BuildAssetHelper
{
public:
    static ln::Result<> buildShaderFromAutoBuild(const ln::Path& intermediateDir, const ln::Path& inputFile, ln::Path* outputFile);
	static ln::Result<> buildShader(const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir);

};

} // namespace lna



