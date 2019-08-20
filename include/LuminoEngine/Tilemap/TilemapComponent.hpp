
#pragma once
#include "../Visual/VisualComponent.hpp"

namespace ln {
class Material;
class TilemapModel;

class TilemapComponent
	: public VisualComponent
{
public:
    void setTilemapModel(TilemapModel* tilemapModel);
	bool intersectTile(const Ray& rayOnWorld, PointI* tilePoint);

protected:
    void onRender(RenderingContext* context);

	LN_SERIALIZE_CLASS_VERSION(1);
	virtual void serialize(Archive& ar) override;

LN_CONSTRUCT_ACCESS:
    TilemapComponent();
	virtual ~TilemapComponent();
	void init();

private:
    Ref<TilemapModel> m_tilemapModel;
};

} // namespace ln
