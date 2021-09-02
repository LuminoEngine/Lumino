#pragma once
#include "UIControl.hpp"
#include "../Layout/UILayoutPanel.hpp"

namespace ln {
class UIThumb;


/**
 * UIValueChangedEventHandler
 */
LN_DELEGATE()
using UIValueChangedEventHandler = Delegate<void(float newValue)>;

/**
 * UIValueChangedEventHandler
 */
LN_DELEGATE()
using UIColorChangedEventHandler = Delegate<void(const Color& newValue)>;

class UIPropertyField
	: public UIControl
{
public:
	virtual const String& elementName() const override { static String name = u"UIPropertyField"; return name; }
};

class UISliderField
	: public UIPropertyField
{
public:
	/** スクロール位置に対する値を設定します。*/
	void setValue(float value);

	/** スクロール位置に対する値を取得します。(default: 0.0) */
	float value() const;

	/** 指定可能な最小値を設定します。*/
	void setMinimum(float value);

	/** 指定可能な最小値を取得します。(default: 0.0) */
	float minimum() const;

	/** 指定可能な最大値を設定します。*/
	void setMaximum(float value);

	/** 指定可能な最大値を取得します。(default: 1.0) */
	float maximum() const;

	void setChanged(Ref<UIValueChangedEventHandler> handler);

protected:
	/** ユーザー操作によって値が変更されたときに呼び出されます。*/
	virtual void onChanged(float oldValue, float newValue);

	///** 指定可能な最小値が変更されたときに呼び出されます。*/
	//virtual void onMinimumChanged(float oldMinimum, float newMinimum);

	///** 指定可能な最大値が変更されたときに呼び出されます。*/
	//virtual void onMaximumChanged(float oldMaximum, float newMaximum);
	
	void onRoutedEvent(UIEventArgs* e) override;
	void onRender(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UISliderField();
	void init();

private:
	void updateValue(const Point& pos);

	float m_value;
	float m_minimum;
	float m_maximum;
	bool m_dragging;

	Ref<UIValueChangedEventHandler> m_changedHandler;
};

class UIColorPickerContent
	: public UIControl
{
public:
	void setColor(const Color& value);
	void setChanged(Ref<UIColorChangedEventHandler> handler);
	
protected:
	void onRoutedEvent(UIEventArgs* e) override;
	void onRender(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIColorPickerContent();
	bool init();

private:
	//void notify(const Color& newValue);

	static const float ColorCircleRadius;
	static const int ColorCircleSplits;
	static const Vector2 CenterOffset;

	Ref<MeshPrimitive> m_mesh;
	Ref<Material> m_material;

	Color m_color;
	Ref<UIColorChangedEventHandler> m_changedHandler;
};

class UIColorField
	: public UIPropertyField
{
public:
	void setValue(const Color& value);
	void setChanged(Ref<UIColorChangedEventHandler> handler);

protected:
	/** ユーザー操作によって値が変更されたときに呼び出されます。*/
	virtual void onChanged(const Color& oldValue, const Color& newValue);

	void onRoutedEvent(UIEventArgs* e) override;
	void onRender(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIColorField();
	bool init();

private:
	//void updateValue(const Point& pos);
	void handleColorChanged(const Color& color);

	Color m_value;
	Ref<UIColorPickerContent> m_picker;
	Ref<UIPopup> m_popup;

	Ref<UIColorChangedEventHandler> m_changedHandler;
};


} // namespace ln

