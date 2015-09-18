/**
	@file	GUIContext.h	
*/
#ifndef LUMINO_GUI_GUICONTEXT_H
#define LUMINO_GUI_GUICONTEXT_H

#include "Common.h"
#include "RoutedEventArgs.h"
#include "UIInjectedInputReceiver.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
	
/**
	@brief		
*/
class GUIContext
	: public CoreObject
	, public IUIInjectedInputReceiver
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:

	/**
		@brief	メインウィンドウのトップレベルに配置される GUIContext を作成します。
	*/
	static GUIContext* Create();

public:

	/**
		@brief	このコンテキストのルート GUI 要素を設定します。
	*/
	void SetRootElement(UIElement* element);
	
	/**
		@brief	このコンテキストのルート GUI 要素を取得します。
	*/
	UIElement* GetRootElement() const;
	
	// Implements IUIInjectedInputReceiver
	virtual bool InjectMouseMove(float clientX, float clientY);
	virtual bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	virtual bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	virtual bool InjectMouseWheel(int delta, float clientX, float clientY);
	virtual bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	virtual bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	virtual bool InjectTextInput(TCHAR ch);
	virtual void InjectElapsedTime(float elapsedTime);

private:
	friend class GUIManagerImpl;
	friend class GUIHelper;
	GUIContext(GUIManagerImpl* manager);
	virtual ~GUIContext();
	const Size& GetViewPixelSize() const;
	void UpdateLayout(const Size& viewPixelSize);
	void Render();

	GUIManagerImpl*	m_manager;
	UIElement*		m_rootElement;
	Size			m_viewPixelSize;
	bool			m_onMainWindow;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino

#endif // LUMINO_GUI_GUICONTEXT_H
