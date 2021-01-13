
#include "Internal.hpp"
#include "RLIMaterial.hpp"

namespace ln {
namespace detail {

RILMaterial::RILMaterial()
{
	reset();
}

void RILMaterial::reset()
{
	material = nullptr;
	blendMode = BlendMode::Normal;
	cullingMode = CullMode::Front;
	depthTestEnabled = true;
	depthWriteEnabled = true;
	shadingModel = ShadingModel::Default;
	primitiveTopology = PrimitiveTopology::TriangleList;
}

bool RILMaterial::equals(const RILMaterial* other) const
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

