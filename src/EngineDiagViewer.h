
#pragma once
#include "EngineDiagCore.h"

LN_NAMESPACE_BEGIN
class DrawingContext;

class EngineDiagViewer
	: public Object
{
public:
	enum DisplayMode
	{
		DisplayMode_Hide = 0,
		DisplayMode_FPSSummary,
		DisplayMode_Details,

		DisplayMode__Count,
	};

public:
	EngineDiagViewer();
	~EngineDiagViewer();
	void Initialize(EngineManager* manager, EngineDiagCore* diagCore);

	//void SetVisible(bool visible) { m_isVisible = visible; }
	//bool IsVisible() const { return m_isVisible; }
	void ToggleDisplayMode();
	void UpdateFrame();
	void Render(DrawingContext* g, const Vector2& viewSize);
	
private:
	EngineDiagCore*	m_diagCore;
	PlatformWindow*	m_mainWindow;
	String			m_originalMainWindowTitle;
	Font*			m_font;
	RectF			m_windowRect;
	DisplayMode		m_displayMode;
};

LN_NAMESPACE_END
