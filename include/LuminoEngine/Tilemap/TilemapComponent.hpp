
#pragma once
#include "../Visual/VisualComponent.hpp"

namespace ln {
class Material;
class TilemapModel;
class RigidBody2D;

class TilemapComponent
	: public VisualComponent
{
    LN_OBJECT;
public:
    TilemapModel* tilemapModel() const;
    void setTilemapModel(TilemapModel* tilemapModel);

    const Ref<RigidBody2D>& rigidBody() const { return m_rigidBody; }

	bool intersectTile(const Ray& rayOnWorld, PointI* tilePoint);

protected:
	virtual void onStart() override;
	virtual void onDetachedScene(Scene* oldOwner) override;
    virtual void onRender(RenderingContext* context) override;
    virtual void onRenderGizmo(RenderingContext* context) override;

	LN_SERIALIZE_CLASS_VERSION(1);
	virtual void serialize(Archive& ar) override;

LN_CONSTRUCT_ACCESS:
    TilemapComponent();
	virtual ~TilemapComponent();
	void init();

private:
    Ref<TilemapModel> m_tilemapModel;
	Ref<RigidBody2D> m_rigidBody;
};

} // namespace ln
