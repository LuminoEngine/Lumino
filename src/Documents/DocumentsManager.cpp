
#include "../Internal.h"
#include <Lumino/Documents/DocumentsManager.h>

namespace Lumino
{
namespace Documents
{

//=============================================================================
// DocumentsManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DocumentsManager::DocumentsManager(const ConfigData& configData)
	: m_graphicsManager(configData.GraphicsManager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DocumentsManager::~DocumentsManager()
{
}

} // namespace Documents
} // namespace Lumino

