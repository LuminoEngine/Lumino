
#pragma once
#include "Common.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
class MeshModel;
class RenderingContext;

class VoxelmapModel
	: public Object
{
    LN_OBJECT;
public:
    void render(RenderingContext* context);

protected:

LN_CONSTRUCT_ACCESS:
    VoxelmapModel();
	virtual ~VoxelmapModel() = default;
	void init();

private:
    void resizeMap(int width, int height, int depth);

    std::vector<int32_t> m_mapData;
    int m_width;
    int m_height;
    int m_depth;

    Ref<MeshModel> m_box;
    Ref<MeshModel> m_mapBlocksMesh;
};

} // namespace ln


//==============================================================================

#include "../Visual/VisualComponent.hpp"

namespace ln {
class RigidBody2D;

class VoxelmapComponent
    : public VisualComponent
{
    LN_OBJECT;
public:

protected:
    virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    VoxelmapComponent();
    virtual ~VoxelmapComponent() = default;
    void init();

private:
    Ref<VoxelmapModel> m_model;
    Ref<RigidBody2D> m_rigidBody;
};

} // namespace ln
