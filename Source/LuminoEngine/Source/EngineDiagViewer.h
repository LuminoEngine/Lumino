
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
	void initialize(EngineManager* manager, EngineDiagCore* diagCore);

	//void setVisible(bool visible) { m_isVisible = visible; }
	//bool isVisible() const { return m_isVisible; }
	void setDisplayMode(EngineDiagDisplayMode mode) { m_displayMode = mode; }
	void toggleDisplayMode();
	void updateFrame();
	//void render(DrawingContext* g, const Vector2& viewSize);
	
private:
	EngineDiagCore*			m_diagCore;
	PlatformWindow*			m_mainWindow;
	String					m_originalMainWindowTitle;
	Ref<Font>			m_font;
	Rect					m_windowRect;
	EngineDiagDisplayMode	m_displayMode;

	static const int		DisplayModeCycle = 3;
};

LN_NAMESPACE_END
