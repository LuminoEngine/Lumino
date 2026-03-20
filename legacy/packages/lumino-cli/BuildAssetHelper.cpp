

#include <LuminoEngine/Graphics//ShaderCompiler/detail/UnifiedShader.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/detail/ShaderManager.hpp>
#include "../../LuminoEngine/src/Graphics/ShaderCompiler/UnifiedShaderCompiler.hpp"
//#include <LuminoCore/Base/CRCHash.hpp>
//#include "../../../lumino/Runtime/src/Asset/AssetArchive.hpp"
//#include <LuminoGraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
////#include "../../../lumino/ShaderCompiler/src/ShaderTranspiler.hpp"
////#include "../../../lumino/ShaderCompiler/src/HLSLMetadataParser.hpp"
////#include "../../../lumino/ShaderCompiler/src/UnifiedShader.hpp"
////#include "../../../lumino/ShaderCompiler/src/UnifiedShaderCompiler.hpp"
//#include "EnvironmentSettings.hpp"
//#include "Workspace.hpp"
//#include "Project.hpp"
#include "BuildAssetHelper.hpp"

namespace ln {

ln::MaybeResult_deprecated
BuildAssetHelper::buildShader(const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir) {
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    ln::detail::ShaderManager::Settings settings;
    ln::detail::ShaderManager::initialize(settings);

    auto diag = makeObject_deprecated<DiagnosticsManager>();
    kokage::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), diag);

	


    List<Path> includeDirs;
    List<String> definitions;
    ByteBuffer code = FileSystem::readAllBytes(inputFile).unwrap();
    compiler.compile(reinterpret_cast<char*>(code.data()), code.size(), includeDirs, definitions);
    compiler.link();

    compiler.unifiedShader()->save(outputFile);


	//auto diag = ln::makeObject_deprecated<ln::DiagnosticsManager>();

	//auto result = ln::kokage::ShaderHelper::generateShader(ln::detail::ShaderManager::instance(), inputFile, outputFile, exportDir, diag);

	//diag->dumpToLog();

	ln::detail::ShaderManager::terminate();

	//if (result && !diag->hasError()) {
	//	CLI::info(_TT(""));
	//	CLI::info(_TT("Compilation succeeded; see ") + outputFile);
	//}

	return LN_MAKE_SUCCESS();
#else
    LN_NOTIMPLEMENTED();
    return LN_MAKE_SUCCESS();
#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
}

} // namespace lna
