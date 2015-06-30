
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
	@note		Thumb はマウスドラッグによる移動量を通知するためのコントロール。
				Thumb 自体の位置を移動するものではない点に注意。
				ScrollBar はもちろん、GridSpritter も Thumb のサブクラス。
*/
class Thumb
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Thumb);
public:
	static PropertyID	IsDraggingProperty;		///< ドラッグ中であるかを示す値

	static EventID		DragStartedEvent;		///< ドラッグ開始イベント
	static EventID		DragCompletedEvent;		///< ドラッグ終了イベント
	static EventID		DragDeltaEvent;			///< ドラッグ中の移動イベント

public:
	Thumb(GUIManager* manager);
	virtual ~Thumb();

	/// ドラッグ中であるかを確認する
	bool IsDragging() const { return m_isDragging; }

protected:
	virtual void OnMouseMove(MouseEventArgs* e);
	virtual void OnMouseLeftButtonDown(MouseEventArgs* e);
	virtual void OnMouseLeftButtonUp(MouseEventArgs* e);

private:
	bool	m_isDragging;
};

/**
	@brief		Thumb の外観
*/
class ThumbChrome
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ThumbChrome);

public:
	static PropertyID	RenderMouseOverProperty;
	static PropertyID	RenderPressedProperty;

public:
	ThumbChrome(GUIManager* manager);
	virtual ~ThumbChrome();

	void SetRenderMouseOver(bool enabled) { m_renderMouseOver = enabled; }
	bool GetRenderMouseOver() const { return m_renderMouseOver; }
	void SetRenderPressed(bool enabled) { m_renderPressed = enabled; }
	bool GetRenderPressed() const { return m_renderPressed; }

protected:
	virtual Size MeasureOverride(Size availableSize);	// 必要ないかも
	virtual Size ArrangeOverride(Size finalSize);		// 必要ないかも
	virtual void OnRender();

private:
	bool	m_renderMouseOver;
	bool	m_renderPressed;
};

} // namespace GUI
} // namespace Lumino
