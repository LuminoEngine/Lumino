
#pragma once
#include "EngineDiagCore.h"

LN_NAMESPACE_BEGIN
class GraphicsContext;

class EngineDiagViewer
	: public Object
{
public:
	EngineDiagViewer();
	~EngineDiagViewer();
	void Initialize(EngineManager* manager, EngineDiagCore* diagCore);

	void SetVisible(bool visible) { m_isVisible = visible; }
	bool IsVisible() const { return m_isVisible; }
	void UpdateFrame();
	void Render(GraphicsContext* g, const Vector2& viewSize);
	
private:
	EngineDiagCore*	m_diagCore;
	PlatformWindow*	m_mainWindow;
	String			m_originalMainWindowTitle;
	Font*			m_font;
	RectF			m_windowRect;
	bool			m_isVisible;
};

LN_NAMESPACE_END
