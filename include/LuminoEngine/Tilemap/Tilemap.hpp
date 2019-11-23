
#pragma once
#include "../Scene/VisualObject.hpp"

namespace ln {
class TilemapModel;
class TilemapComponent;

class Tilemap
	: public VisualObject
{
    LN_OBJECT;
public:
    static Ref<Tilemap> create();

    void setTilemapModel(TilemapModel* tilemapModel);

	TilemapComponent* tilemapComponent() const;

protected:

    LN_SERIALIZE_CLASS_VERSION(1);
    virtual void serialize(Archive& ar) override;

LN_CONSTRUCT_ACCESS:
    Tilemap();
	virtual ~Tilemap();
	void init();

private:
    Ref<TilemapComponent> m_component;
};

} // namespace ln
