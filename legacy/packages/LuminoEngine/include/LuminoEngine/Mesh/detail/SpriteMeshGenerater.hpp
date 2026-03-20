#pragma once
#include "MeshGenerater.hpp"

namespace ln {
namespace detail {

class SpriteMeshGenerater : public MeshGenerater {
public:
    SpriteData sprite;

    int vertexCount() const override { return 4; }
    int indexCount() const override { return 6; }
    PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
    void onGenerate(MeshGeneraterBuffer* buf) override;
    void copyFrom(const SpriteMeshGenerater* other);
    LN_MESHGENERATOR_CLONE_IMPLEMENT(SpriteMeshGenerater);
};

} // namespace detail
} // namespace ln

