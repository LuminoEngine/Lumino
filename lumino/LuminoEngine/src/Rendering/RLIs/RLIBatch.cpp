
#include "Internal.hpp"
#include <LuminoGraphics/GraphicsContext.hpp>
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
	//, m_worldTransform(nullptr)
	//, m_renderPass(nullptr)
{
}

void RenderFeatureBatch::setup(const Matrix* worldTransformPtr, Material* finalMaterial, const SubsetInfo& subsetInfo, RenderPass* renderPass)
{
	assert(m_stage);
	m_material.m_worldTransform = worldTransformPtr;
	m_material.m_subsetInfo = subsetInfo;
	m_material.m_renderPass = renderPass;
	m_material.mergeFrom(m_stage->geometryStageParameters, finalMaterial);
}

void RenderFeatureBatch::render(GraphicsContext* context)
{
	m_owner->renderBatch(context, this);
}

//void RenderFeatureBatch::setFinalMaterial(Material* value)
//{
//}

} // namespace detail
} // namespace ln

