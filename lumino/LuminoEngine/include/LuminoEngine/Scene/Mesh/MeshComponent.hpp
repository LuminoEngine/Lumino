
#pragma once
#include "../../Visual/VisualComponent.hpp"

namespace ln {
class MeshModel;
class RigidBody;

/** UIElement */
LN_CLASS()
class MeshComponent
	: public VisualComponent
{
    LN_OBJECT;
public:
    /** setModel */
    LN_METHOD()
    void setModel(MeshModel* model);

    MeshModel* model() const;

    const Ref<detail::MeshModelInstance>& modelInstance() const { return m_modelInstance; }

	/** 指定した名前の MeshContainer から、衝突判定用の Body を作成します。 */
	LN_METHOD()
	void makeCollisionBody(StringView meshContainerName);

protected:
    void onDispose(bool explicitDisposing) override;
    void serialize_deprecated(Serializer2_deprecated& ar) override;
    void onUpdate(float elapsedSeconds) override;
    void onRender(RenderingContext* context) override;
    void onRenderGizmo(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    MeshComponent();
	virtual ~MeshComponent();

    /** init */
    LN_METHOD()
    Result<> init();

private:
    void deleteCollisionBody();
    void forEachVisibleMeshPrimitives(CommandList* context, std::function<void(MeshNode* node, MeshPrimitive* meshPrimitive)> callback);

    Ref<MeshModel> m_model;
    Ref<RigidBody> m_body;
    Ref<detail::MeshModelInstance> m_modelInstance;
};

} // namespace ln
