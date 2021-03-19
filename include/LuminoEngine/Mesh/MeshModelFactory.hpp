#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class MeshGenerater;

class MeshModelFactory
{
public:
	static Ref<MeshModel> createBox(const Vector3& size, Material* material = nullptr);
	static Ref<MeshModel> createCone(float radius, float height, float segments, Material* material = nullptr);
	static Ref<MeshModel> createCylinder(float radiusTop, float radiusBottom, float height, float segments, Material* material = nullptr);

private:
	static Ref<MeshModel> createMesh(MeshGenerater* factory, Material* material = nullptr);
};

} // namespace detail
} // namespace ln

