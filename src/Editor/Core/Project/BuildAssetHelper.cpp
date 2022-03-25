
#define LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include <LuminoCore/Base/CRCHash.hpp>
#include "../../../lumino/Runtime/src/Asset/AssetArchive.hpp"
#include <LuminoGraphics/ShaderCompiler/detail/ShaderManager.hpp>
//#include "../../../lumino/ShaderCompiler/src/ShaderTranspiler.hpp"
//#include "../../../lumino/ShaderCompiler/src/HLSLMetadataParser.hpp"
//#include "../../../lumino/ShaderCompiler/src/UnifiedShader.hpp"
//#include "../../../lumino/ShaderCompiler/src/UnifiedShaderCompiler.hpp"
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildAssetHelper.hpp"

namespace lna {

//==============================================================================
// BuildAssetHelper

ln::Result BuildAssetHelper::buildShaderFromAutoBuild(const ln::Path& intermediateDir, const ln::Path& inputFile, ln::Path* outputFile)
{
    const auto fileName = inputFile.fileName();
    const auto crc = ln::String::fromNumber(ln::CRCHash::compute(fileName.c_str()));
    const auto workFile = ln::Path::combine(intermediateDir, ln::format(U"{}-{}", crc, fileName));

	//auto rel = project->assetsDir().makeRelative(inputFile);
	//auto workFile = ln::Path::combine(project->intermediateAssetsDir(), rel.parent(), inputFile.fileName().replaceExtension(ln::detail::UnifiedShader::FileExt));
	//ln::FileSystem::createDirectory(workFile.parent());

	if (!buildShader(inputFile, workFile, ln::Path::Empty)) {
		return ln::err();
	}

	*outputFile = workFile;
	return ln::ok();
}

ln::Result BuildAssetHelper::buildShader(const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir)
{
	ln::detail::ShaderManager::Settings settings;
	ln::detail::ShaderManager::initialize(settings);

	auto diag = ln::makeObject<ln::DiagnosticsManager>();

	auto result = ln::detail::ShaderHelper::generateShader(ln::detail::ShaderManager::instance(), inputFile, outputFile, exportDir, diag);

	diag->dumpToLog();

	ln::detail::ShaderManager::terminate();

	if (result && !diag->hasError()) {
		CLI::info(_TT(""));
		CLI::info(_TT("Compilation succeeded; see ") + outputFile);
	}

	return ln::Result(result);
}

} // namespace lna
