
#pragma once
#include "EngineDiagCore.h"

LN_NAMESPACE_BEGIN
class GraphicsContext;

class EngineDiagRenderer
	: public Object
{
public:
	EngineDiagRenderer();
	~EngineDiagRenderer();
	void Initialize(GraphicsManager* manager, EngineDiagCore* diagCore);

	void SetVisible(bool visible) { m_isVisible = visible; }
	bool IsVisible() const { return m_isVisible; }
	void Render(GraphicsContext* g, const Vector2& viewSize);
	
private:
	EngineDiagCore*	m_diagCore;
	Font*			m_font;
	RectF			m_windowRect;
	bool			m_isVisible;
};

LN_NAMESPACE_END
