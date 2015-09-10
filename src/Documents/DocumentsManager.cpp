
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
	m_TCharToUTF32Converter.SetDestinationEncoding(Encoding::GetUTF32Encoding());
	m_TCharToUTF32Converter.SetSourceEncoding(Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetDestinationEncoding(Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetSourceEncoding(Encoding::GetUTF32Encoding());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DocumentsManager::~DocumentsManager()
{
}

} // namespace Documents
} // namespace Lumino

