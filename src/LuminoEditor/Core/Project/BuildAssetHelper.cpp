
#define LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "../../LuminoEngine/src/Asset/AssetArchive.hpp"
#include "../../LuminoEngine/src/Shader/ShaderManager.hpp"
#include "../../LuminoEngine/src/Shader/ShaderTranspiler.hpp"
#include "../../LuminoEngine/src/Shader/HLSLMetadataParser.hpp"
#include "../../LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "../../LuminoEngine/src/Shader/UnifiedShaderCompiler.hpp"
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildAssetHelper.hpp"

namespace lna {

//==============================================================================
// LanguageContext

ln::Result BuildAssetHelper::buildShaderFromAutoBuild(const Project* project, const ln::Path& inputFile, ln::Path* outputFile)
{
	if (LN_REQUIRE(project)) return false;

	auto rel = project->assetsDir().makeRelative(inputFile);
	auto workFile = ln::Path::combine(project->intermediateAssetsDir(), rel.parent(), inputFile.fileName().replaceExtension(ln::detail::UnifiedShader::FileExt));
	ln::FileSystem::createDirectory(workFile.parent());

	if (buildShader(inputFile, workFile, ln::Path::Empty) != 0) {
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

	auto result = generateShader(manager, inputFile, outputFile, exportDir, diag);

	diag->dumpToLog();

	manager->dispose();

	return ((!result) || diag->hasError());
}

ln::Result BuildAssetHelper::generateShader(ln::detail::ShaderManager* manager, const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir, ln::DiagnosticsManager* diag)
{
	ln::Path inputFilePath = inputFile.canonicalize();
	ln::Path outputFilePath = outputFile;
	if (outputFilePath.isEmpty()) {
		outputFilePath = inputFilePath.replaceExtension(ln::detail::UnifiedShader::FileExt);
	}

	ln::List<ln::Path> includeDirectories;
	ln::List<ln::String> definitions;

	auto inputCodeBuffer = ln::FileSystem::readAllBytes(inputFilePath);
	char* inputCode = (char*)inputCodeBuffer.data();
	size_t inputCodeLength = inputCodeBuffer.size();


	ln::detail::UnifiedShaderCompiler compiler(manager, diag);
	if (!compiler.compile(inputCode, inputCodeLength, includeDirectories, definitions)) {
		return false;
	}
	if (!compiler.link()) {
		return false;
	}


	if (!compiler.unifiedShader()->save(outputFilePath)) {
		return false;
	}

	// dump intermediate codes.
	if (!exportDir.isEmpty()) {
		ln::FileSystem::createDirectory(exportDir);
		compiler.unifiedShader()->saveCodes(exportDir.str() + u"/");
	}

	CLI::info(u"");
	CLI::info(u"Compilation succeeded; see " + outputFilePath);

	return true;

}



} // namespace lna
