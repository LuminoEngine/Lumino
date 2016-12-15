
#pragma once
#include <Lumino/EngineDiag.h>
#include "EngineDiagCore.h"

LN_NAMESPACE_BEGIN

class EngineDiagViewer
	: public Object
{
public:
	EngineDiagViewer();
	~EngineDiagViewer();
	void Initialize(EngineManager* manager, EngineDiagCore* diagCore);

	//void SetVisible(bool visible) { m_isVisible = visible; }
	//bool IsVisible() const { return m_isVisible; }
	void SetDisplayMode(EngineDiagDisplayMode mode) { m_displayMode = mode; }
	void ToggleDisplayMode();
	void UpdateFrame();
	//void Render(DrawingContext* g, const Vector2& viewSize);
	
private:
	EngineDiagCore*			m_diagCore;
	PlatformWindow*			m_mainWindow;
	String					m_originalMainWindowTitle;
	RefPtr<Font>			m_font;
	RectF					m_windowRect;
	EngineDiagDisplayMode	m_displayMode;

	static const int		DisplayModeCycle = 3;
};

LN_NAMESPACE_END
