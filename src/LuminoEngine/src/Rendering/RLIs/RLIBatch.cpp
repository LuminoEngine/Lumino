
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RLIBatchList.hpp"
#include "RLIBatch.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// RenderFeatureBatch

RenderFeatureBatch::RenderFeatureBatch()
	: m_owner(nullptr)
	, m_next(nullptr)
	, m_stage(nullptr)
	//, m_type(detail::RenderDrawElementTypeFlags::Geometry)
	, m_worldTransform(nullptr)
	, m_renderPass(nullptr)
{
}

void RenderFeatureBatch::render(GraphicsContext* context)
{
	m_owner->renderBatch(context, this);
}


} // namespace detail
} // namespace ln

