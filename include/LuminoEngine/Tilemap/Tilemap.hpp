
#pragma once
#include "../Scene/VisualObject.hpp"

namespace ln {
class TilemapComponent;

class Tilemap
	: public VisualObject
{
public:


protected:

LN_CONSTRUCT_ACCESS:
    Tilemap();
	virtual ~Tilemap();
	void initialize();

private:
    Ref<TilemapComponent> m_component;
};

} // namespace ln
