
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

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
	ShaderCode::initializeGlobals();
#endif
}

void ShaderManager::dispose()
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
	ShaderCode::finalizeGlobals();
#endif
}

} // namespace detail
} // namespace ln

