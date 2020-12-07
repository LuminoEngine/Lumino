
#pragma once
#include "StaticMeshComponent.hpp"

namespace ln {
class SkinnedMeshModel;
namespace detail {
class MeshModelInstance;
}

/**
 * SkinnedMeshComponent
 */
LN_CLASS()
class SkinnedMeshComponent
	: public StaticMeshComponent
{
public:
	//const Ref<detail::MeshModelInstance>& modelInstance() const { return m_modelInstance; }

protected:
	//void onUpdate(float elapsedSeconds) override;
	//void onRender(RenderingContext* context) override;
	//void onRenderGizmo(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    SkinnedMeshComponent();
	virtual ~SkinnedMeshComponent();

	/** init */
	LN_METHOD()
	bool init();

private:
 //   Ref<SkinnedMeshModel> m_model;
	//Ref<detail::MeshModelInstance> m_modelInstance;
};

} // namespace ln
