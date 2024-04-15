#pragma once
#include "../Kanata/KBatchProxy.hpp"
#include "../../Mesh/Common.hpp"
#include "../Common.hpp"

namespace ln {

//class MeshRenderer : public URefObject {
//public:
//    void drawMesh(Material* material, MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton = nullptr, detail::MorphInstance* morph = nullptr);
//
//private:
//    MeshRenderer();
//
//    friend class detail::RenderingManager;
//};

class MeshBatchProxy : public kanata::BatchProxy {
public:
    Ref<detail::MeshModelInstance> modelInstance;

    MeshBatchProxy();
    virtual ~MeshBatchProxy();

protected:
    void getBatch(kanata::BatchCollector* collector) override;
};

} // namespace ln

