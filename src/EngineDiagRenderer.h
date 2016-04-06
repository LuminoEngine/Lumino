
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
	void Initialize(GraphicsManager* manager, EngineDiagCore* diag);

	void Render(GraphicsContext* g, const Vector2& viewSize);
	
private:
	EngineDiagCore*	m_diag;
	Font*			m_font;
	RectF			m_windowRect;
};

LN_NAMESPACE_END
