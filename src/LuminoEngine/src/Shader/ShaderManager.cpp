
#include "Internal.hpp"
#include "ShaderManager.hpp"

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
	glslang::InitializeProcess();
#endif
}

void ShaderManager::dispose()
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
	glslang::FinalizeProcess();
#endif
}

} // namespace detail
} // namespace ln

