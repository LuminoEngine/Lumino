#pragma once
#include "../Common.hpp"

namespace ln {
namespace kanata {

class MeshRenderFeature : public URefObject {
public:
    MeshRenderFeature(detail::RenderingManager* manager);

    void drawMesh(
        BatchCollector* collector,
        Material* material,
        MeshPrimitive* mesh,
        int sectionIndex,
        const Matrix* transform = nullptr,
        detail::SkeletonInstance* skeleton = nullptr,
        detail::MorphInstance* morph = nullptr);
    void drawMeshInstanced(BatchCollector* collector, Material* material, InstancedMeshList* list) const;

private:
};

} // namespace kanata
} // namespace ln

