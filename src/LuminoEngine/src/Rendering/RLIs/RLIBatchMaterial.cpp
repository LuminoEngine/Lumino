
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "RLIBatchMaterial.hpp"

namespace ln {
namespace detail {

RLIBatchMaterial::RLIBatchMaterial()
{
	reset();
}

void RLIBatchMaterial::reset()
{
	material = nullptr;
	blendMode = BlendMode::Normal;
	cullingMode = CullMode::Front;
	depthTestEnabled = true;
	depthWriteEnabled = true;
	shadingModel = ShadingModel::Default;
	primitiveTopology = PrimitiveTopology::TriangleList;
}

bool RLIBatchMaterial::equals(const RLIBatchMaterial* other) const
{
	return
		material == other->material &&  // TODO: Material が一致することはまずない。ちゃんと中身かhashを見ること。
		blendMode == other->blendMode &&
		cullingMode == other->cullingMode &&
		depthTestEnabled == other->depthTestEnabled &&
		depthWriteEnabled == other->depthWriteEnabled &&
		shadingModel == other->shadingModel &&
		primitiveTopology == other->primitiveTopology;
}

} // namespace detail
} // namespace ln

