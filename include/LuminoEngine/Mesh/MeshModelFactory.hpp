#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class MeshModelFactory
{
public:
	static Ref<MeshModel> createBox(const Vector3& size, Material* material = nullptr);
	static Ref<MeshModel> createCone(float radius, float height, float segments, Material* material = nullptr);
};

} // namespace detail
} // namespace ln

