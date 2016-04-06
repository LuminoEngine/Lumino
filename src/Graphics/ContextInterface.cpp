
#include "Internal.h"
#include <Lumino/Graphics/ContextInterface.h>
#include "GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//=============================================================================
// IContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IContext::IContext()
	: m_manager(nullptr)
	, m_activeRendererPloxy(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IContext::~IContext()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
	m_activeRendererPloxy = nullptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnPrimitiveFlushRequested()
{
	if (m_activeRendererPloxy != nullptr)
	{
		m_activeRendererPloxy->Flush();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnStateChanging()
{
	m_manager->SwitchActiveContext(this);
	OnPrimitiveFlushRequested();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnDrawing(detail::IRendererPloxy* rendererPloxy)
{
	m_manager->SwitchActiveContext(this);
	OnStateFlushRequested();
	SwitchActiveRendererPloxy(rendererPloxy);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnDeactivated()
{
	SwitchActiveRendererPloxy(nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy)
{
	if (rendererPloxy != m_activeRendererPloxy)
	{
		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->OnDeactivated();	// å√Ç¢Ç‡ÇÃÇ Deactivate
		}

		m_activeRendererPloxy = rendererPloxy;

		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->OnActivated();	// êVÇµÇ¢Ç‡ÇÃÇ Activate
		}
	}
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
