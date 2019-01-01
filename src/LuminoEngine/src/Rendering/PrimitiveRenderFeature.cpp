
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "RenderingManager.hpp"
#include "PrimitiveRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PrimitiveRenderFeature

PrimitiveRenderFeature::PrimitiveRenderFeature()
	: m_manager(nullptr)
{
}

PrimitiveRenderFeature::~PrimitiveRenderFeature()
{
}

void PrimitiveRenderFeature::initialize(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
}

void PrimitiveRenderFeature::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
    LN_NOTIMPLEMENTED();
}

void PrimitiveRenderFeature::flush(GraphicsContext* context)
{
}

} // namespace detail
} // namespace ln

