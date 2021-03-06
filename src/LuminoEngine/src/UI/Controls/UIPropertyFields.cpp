
#include "Internal.hpp"
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIPopup.hpp>
#include <LuminoEngine/UI/Controls/UIPropertyFields.hpp>

namespace ln {
	
//==============================================================================
// UISliderField

UISliderField::UISliderField()
	: m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_dragging(false)
{
}

void UISliderField::init()
{
	UIControl::init();

	//setBackgroundColor(Color::White.withAlpha(0.5));
	//setBorderColor(Color::Gray);
	//setBorderThickness(1);
}

void UISliderField::setValue(float value)
{
	float oldValue = m_value;
	m_value = value;
}

float UISliderField::value() const
{
	return m_value;
}

void UISliderField::setMinimum(float value)
{
	float oldValue = m_minimum;
	m_minimum = value;
	//if (m_minimum != oldValue) {
	//	onMinimumChanged(oldValue, m_minimum);
	//}
}

float UISliderField::minimum() const
{
	return m_minimum;
}

void UISliderField::setMaximum(float value)
{
	float oldValue = m_maximum;
	m_maximum = value;
	//if (m_maximum != oldValue) {
	//	onMaximumChanged(oldValue, m_maximum);
	//}
}

float UISliderField::maximum() const
{
	return m_maximum;
}

void UISliderField::setChanged(Ref<UIValueChangedEventHandler> handler)
{
	m_changedHandler = handler;
}

void UISliderField::onChanged(float oldValue, float newValue)
{
	if (m_changedHandler) {
		m_changedHandler->call(newValue);
	}
}

//void UISliderField::onMinimumChanged(float oldMinimum, float newMinimum)
//{
//}
//
//void UISliderField::onMaximumChanged(float oldMaximum, float newMaximum)
//{
//}

void UISliderField::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent)
	{
		if (!m_dragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			updateValue(mouseEvent->getPosition(this));

			m_dragging = true;
			retainCapture();
			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseUpEvent)
	{
		if (m_dragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);

			m_dragging = false;
			releaseCapture();
			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseMoveEvent)
	{
		if (m_dragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			updateValue(mouseEvent->getPosition(this));

			e->handled = true;
			return;
		}
	}

	UIElement::onRoutedEvent(e);
}

void UISliderField::onRender(UIRenderingContext* context)
{
	float r = (m_value - m_minimum) / (m_maximum - m_minimum);
	auto cr = clientRect_Obsolete();
	auto bar = cr.makeDeflate(2);
	bar.width = bar.width * r;
	context->drawSolidRectangle(bar, context->theme()->color(UIThemeConstantPalette::PrimaryMainColor));

	
	context->drawText(String::fromNumber(m_value, 'F', 3), cr, TextAlignment::Center);
}

void UISliderField::updateValue(const Point& pos)
{
	float oldValue = m_value;
	auto cr = clientRect_Obsolete();
	//if (cr.contains(pos)) {
		float r = (pos.x - cr.x) / cr.width;
		m_value = Math::clamp(m_minimum + ((m_maximum - m_minimum) * r), m_minimum, m_maximum);
	//}
	if (m_value != oldValue) {
		onChanged(oldValue, m_value);
	}
}


//==============================================================================
// UIColorPickerContent

const float UIColorPickerContent::ColorCircleRadius = 100;
const int UIColorPickerContent::ColorCircleSplits = 32;
const Vector2 UIColorPickerContent::CenterOffset(100, 100);

UIColorPickerContent::UIColorPickerContent()
{
}

bool UIColorPickerContent::init()
{
	if (!UIControl::init()) return false;
	setSize(200, 400);
	setBackgroundColor(Color::Gray);
	setFocusable(true);

	m_mesh = makeObject<MeshPrimitive>(1 + (ColorCircleSplits + 2) * 2, 0, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);
	auto* vb = static_cast<Vertex*>(m_mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));

	vb[0].setPosition(Vector3::Zero);
	vb[0].color = Color::White;


	// TODO: 1週目は AA、2週目で本体を描いているが、ちょっとAAが汚い。
	// やっぱり ShapesRenderer と同じ仕組みで AA 作らないとダメかも。
	// その場合は TriagleFan じゃなくて TraiangleList にする必要がある。
	// あとは画像にしてしまうか。
	for (int j = 0; j < 2; j++) {
		int offset = 1 + (ColorCircleSplits + 2) * j;
		const float ext = (j == 0) ? 1.0f : 0.0f;
		const float alpha = (j == 0) ? 0.5f : 1.0f;
		for (int i = 0; i < ColorCircleSplits + 1; i++) {
			const float r = Math::PI2 / ColorCircleSplits * i;
			float s, c;
			Math::sinCos(r, &s, &c);
			vb[offset + i].position.x = c * (ColorCircleRadius + ext);
			vb[offset + i].position.y = s * (ColorCircleRadius + ext);
			vb[offset + i].color = HSVColor(Math::radiansToDegrees(r), 1, 1, alpha).toColor();
		}
	}

	m_mesh->addSection(0, ColorCircleSplits * 2 + 2, 0, PrimitiveTopology::TriangleFan);

	m_material = makeObject<Material>();

	return true;
}

void UIColorPickerContent::setColor(const Color& value)
{
	m_color = value;
}

void UIColorPickerContent::setChanged(Ref<UIColorChangedEventHandler> handler)
{
	m_changedHandler = handler;
}

void UIColorPickerContent::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent)
	{
		const auto me = static_cast<UIMouseEventArgs*>(e);
		const auto localPos = me->getPosition(this);

		const float d = Vector2::distance(localPos, CenterOffset);
		if (d <= ColorCircleRadius) {

			HSVColor hsv;
			const auto dir = Vector2::safeNormalize(localPos - CenterOffset, Vector2::Zero);
			if (dir.isZero()) {
				hsv = HSVColor(0, 1.0, 1.0, m_color.a);
			}
			else {
				float r = std::atan2(dir.y, dir.x);
				hsv = HSVColor(Math::radiansToDegrees(r), 1.0, 1.0, m_color.a);
				hsv.s = d / ColorCircleRadius;
			}

			m_color = hsv.toColor();

			if (m_changedHandler) {
				m_changedHandler->call(m_color);
			}
		}
		e->handled = true;
		return;

	}

	UIControl::onRoutedEvent(e);
}

void UIColorPickerContent::onRender(UIRenderingContext* context)
{
	UIControl::onRender(context);

	auto t = Matrix::makeTranslation(CenterOffset.x, CenterOffset.y, 0);
	context->setTransfrom(t);

	context->setMaterial(m_material);
	context->drawMesh(m_mesh, 0);
}

//void UIColorPickerContent::notify(const Color& newValue)
//{
//	if (m_changedHandler) {
//
//	}
//}

//==============================================================================
// UIColorField

UIColorField::UIColorField()
{

}

bool UIColorField::init()
{
	if (!UIPropertyField::init()) return false;

	m_value = Color::White;
	m_picker = makeObject<UIColorPickerContent>();
	m_picker->setChanged(makeDelegate(this, &UIColorField::handleColorChanged));

	// TODO: PopUp なカラーピッカーは共通の単一インスタンスでいいかも
	m_popup = makeObject<UIPopup>();
	m_popup->setBackgroundColor(Color::Red);
	m_popup->setPlacementTarget(this);
	m_popup->addChild(m_picker);
	//addChild(m_popup);


	//auto e = makeObject<UIElement>();
	//e->setSize(100, 100);
	//e->setBackgroundColor(Color::Green);
	//m_popup->setPlacementTarget(e);

	return true;
}

void UIColorField::setValue(const Color& value)
{
	m_value = value;
}

void UIColorField::setChanged(Ref<UIColorChangedEventHandler> handler)
{
	m_changedHandler = handler;
}

void UIColorField::onChanged(const Color& oldValue, const Color& newValue)
{

}

void UIColorField::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent)
	{
		m_picker->setColor(m_value);
		m_popup->open();
		e->handled = true;
		return;
	}

	UIPropertyField::onRoutedEvent(e);
}

void UIColorField::onRender(UIRenderingContext* context)
{
	UIPropertyField::onRender(context);

	auto cr = clientRect_Obsolete();
	auto bar = cr.makeDeflate(2);
	context->drawSolidRectangle(bar, m_value);

}

//void UIColorField::updateValue(const Point& pos)
//{
//
//}

void UIColorField::handleColorChanged(const Color& color)
{
	m_value = color;
}

} // namespace ln

