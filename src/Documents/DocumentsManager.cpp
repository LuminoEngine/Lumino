
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
	m_TCharToUTF32Converter.SetDestinationEncoding(Text::Encoding::GetUTF32Encoding());
	m_TCharToUTF32Converter.SetSourceEncoding(Text::Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetDestinationEncoding(Text::Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetSourceEncoding(Text::Encoding::GetUTF32Encoding());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DocumentsManager::~DocumentsManager()
{
}

} // namespace Documents
} // namespace Lumino

