
#include "Internal.h"
#include "DocumentElements.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// Document
//==============================================================================

//------------------------------------------------------------------------------
Document::Document()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
Document::~Document()
{
}

//------------------------------------------------------------------------------
void Document::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
}

} // namespace detail
LN_NAMESPACE_END
