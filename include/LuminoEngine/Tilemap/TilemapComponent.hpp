
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

	bool intersectTile(const Ray& rayOnWorld, PointI* tilePoint);

protected:
	virtual void onAttachedScene(Scene* newOwner) override;
	virtual void onDetachedScene(Scene* oldOwner) override;
    void onRender(RenderingContext* context);

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
