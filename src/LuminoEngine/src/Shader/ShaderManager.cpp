
#include "Internal.hpp"
#include "ShaderManager.hpp"
#include "ShaderAnalyzer.hpp"

namespace ln {
namespace detail {

//=============================================================================
// ShaderManager

ShaderManager::ShaderManager()
	: m_graphicsManager(nullptr)
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::initialize(const Settings& settings)
{
	m_graphicsManager = settings.graphicsManager;

#ifdef LN_DEBUG
	Path debugIncludePath = Path((Path(String::fromCString(__FILE__)).parent()), u"Resource");
	m_shaderIncludePaths.add(debugIncludePath);
#endif

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
	ShaderCodeTranspiler::initializeGlobals();
#endif
}

void ShaderManager::dispose()
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    ShaderCodeTranspiler::finalizeGlobals();
#endif
}

} // namespace detail
} // namespace ln

