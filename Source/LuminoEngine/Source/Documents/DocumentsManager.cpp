
#include "Internal.h"
#include "DocumentsManager.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// DocumentsManager
//==============================================================================

static DocumentsManager* g_manager = nullptr;

//------------------------------------------------------------------------------
DocumentsManager* DocumentsManager::getInstance(DocumentsManager* priority)
{
	return (priority != nullptr) ? priority : g_manager;
}

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
	g_manager = nullptr;
}

//------------------------------------------------------------------------------
void DocumentsManager::initialize(const ConfigData& configData)
{
	m_graphicsManager = configData.graphicsManager;
	m_TCharToUTF32Converter.setDestinationEncoding(Encoding::getUTF32Encoding());
	m_TCharToUTF32Converter.getSourceEncoding(Encoding::getTCharEncoding());
	m_UTF32ToTCharConverter.setDestinationEncoding(Encoding::getTCharEncoding());
	m_UTF32ToTCharConverter.getSourceEncoding(Encoding::getUTF32Encoding());

	g_manager = this;
}

} // namespace detail
LN_NAMESPACE_END
