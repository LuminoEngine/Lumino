
#include "Internal.hpp"
#include "../Rendering/RenderStage.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>

namespace ln {

//==============================================================================
// UIRenderingContext

UIRenderingContext::UIRenderingContext()
	: m_elementList(makeRef<detail::DrawElementList>(detail::EngineDomain::renderingManager()))
{
	setDrawElementList(m_elementList);
}

void UIRenderingContext::reset()
{
	RenderingContext::reset();
	m_elementList->clear();
}

} // namespace ln

