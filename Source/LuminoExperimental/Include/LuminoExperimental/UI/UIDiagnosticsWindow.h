#pragma once
#include <Lumino/UI/UIFrameWindow.h>

LN_NAMESPACE_BEGIN
class ProfilingKey;
class UIButton;

class UIDiagnosticsWindow
	: public UIFrameWindow
{
public:

protected:
	virtual void onRender(DrawingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIDiagnosticsWindow();
	virtual ~UIDiagnosticsWindow() = default;
	void initialize();

private:
	static const int ThreadNameColumnWidth = 80;
	static const int SafeAreaWidth = 160;
	static const int rowHeight = 32;

	void drawStatistics(DrawingContext* context, const Rect& windowRect);
	void drawGroupList(DrawingContext* context, const Rect& listRect);
	void drawSectionGraphBar(DrawingContext* context, ProfilingKey* key, float x, float y, const Rect& listRect, float* totalElapsed);

	Ref<Font>	m_font;
	float			m_limitElapsedTime = 1.0f / 60.0f;	// TODO: FPS

	Ref<UIToggleButton>	m_recordButton;
};

LN_NAMESPACE_END
