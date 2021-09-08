
#define LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "../../Engine/src/Asset/AssetArchive.hpp"
#include "../../Engine/src/Shader/ShaderManager.hpp"
#include "../../Engine/src/Shader/ShaderTranspiler.hpp"
#include "../../Engine/src/Shader/HLSLMetadataParser.hpp"
#include "../../Engine/src/Shader/UnifiedShader.hpp"
#include "../../Engine/src/Shader/UnifiedShaderCompiler.hpp"
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildAssetHelper.hpp"

namespace lna {

//==============================================================================
// BuildAssetHelper

ln::Result BuildAssetHelper::buildShaderFromAutoBuild(const Project* project, const ln::Path& inputFile, ln::Path* outputFile)
{
	if (LN_REQUIRE(project)) return false;

	auto rel = project->assetsDir().makeRelative(inputFile);
	auto workFile = ln::Path::combine(project->intermediateAssetsDir(), rel.parent(), inputFile.fileName().replaceExtension(ln::detail::UnifiedShader::FileExt));
	ln::FileSystem::createDirectory(workFile.parent());

	if (!buildShader(inputFile, workFile, ln::Path::Empty)) {
		return false;
	}

	*outputFile = workFile;
	return true;
}

ln::Result BuildAssetHelper::buildShader(const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir)
{
	ln::detail::ShaderManager::Settings settings;
	auto manager = ln::makeRef<ln::detail::ShaderManager>();
	manager->init(settings);

	auto diag = ln::makeObject<ln::DiagnosticsManager>();

	auto result = ln::detail::ShaderHelper::generateShader(manager, inputFile, outputFile, exportDir, diag);

	diag->dumpToLog();

	manager->dispose();

	if (result && !diag->hasError()) {
		CLI::info(_TT(""));
		CLI::info(_TT("Compilation succeeded; see ") + outputFile);
	}

	return result;
}

} // namespace lna
