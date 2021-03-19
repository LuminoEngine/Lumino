#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class MeshModelFactory
{
public:
	static Ref<MeshModel> createBox(const Vector3& size, Material* material = nullptr);
};

} // namespace detail
} // namespace ln

