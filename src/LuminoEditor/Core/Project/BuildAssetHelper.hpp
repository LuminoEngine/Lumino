#pragma once

namespace lna {
class Workspace;
class Project;

class BuildAssetHelper
{
public:
	static ln::Result buildShaderFromAutoBuild(const Project* project, const ln::Path& inputFile, ln::Path* outputFile);
	static ln::Result buildShader(const ln::Path& inputFile, const ln::Path& outputFile);
	static ln::Result generateShader(ln::detail::ShaderManager* manager, const ln::Path& inputFile, const ln::Path& outputFile, ln::DiagnosticsManager* diag);

};

} // namespace lna



