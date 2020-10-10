
#pragma once
#include "../../Visual/VisualComponent.hpp"

namespace ln {
class StaticMeshModel;
class RigidBody;

/** UIElement */
LN_CLASS()
class StaticMeshComponent
	: public VisualComponent
{
    LN_OBJECT;
public:
    /** setModel */
    LN_METHOD()
    void setModel(StaticMeshModel* model);

    StaticMeshModel* model() const;
    
	/** 指定した名前の MeshContainer から、衝突判定用の Body を作成します。 */
	LN_METHOD()
	void makeCollisionBody(StringRef meshContainerName);

protected:
    void onDispose(bool explicitDisposing) override;
    void serialize(Serializer2& ar) override;
    void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    StaticMeshComponent();
	virtual ~StaticMeshComponent();

    /** init */
    LN_METHOD()
	bool init();

private:
    void deleteCollisionBody();

    Ref<StaticMeshModel> m_model;
    Ref<RigidBody> m_body;
};

} // namespace ln
