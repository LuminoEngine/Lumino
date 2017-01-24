
#include "Internal.h"
#include "DocumentsManager.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// DocumentsManager
//==============================================================================

//------------------------------------------------------------------------------
DocumentsManager::DocumentsManager()
	: m_graphicsManager(nullptr)
	, m_TCharToUTF32Converter()
	, m_UTF32ToTCharConverter()
{
}

//------------------------------------------------------------------------------
DocumentsManager::~DocumentsManager()
{
}

//------------------------------------------------------------------------------
void DocumentsManager::Initialize(const ConfigData& configData)
{
	m_graphicsManager = configData.graphicsManager;
	m_TCharToUTF32Converter.SetDestinationEncoding(Encoding::GetUTF32Encoding());
	m_TCharToUTF32Converter.SetSourceEncoding(Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetDestinationEncoding(Encoding::GetTCharEncoding());
	m_UTF32ToTCharConverter.SetSourceEncoding(Encoding::GetUTF32Encoding());
}

} // namespace detail
LN_NAMESPACE_END
