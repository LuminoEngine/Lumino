
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include "../Rendering/RenderStage.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIElement

UIElement::UIElement()
    : m_manager(nullptr)
    , m_context(nullptr)
    , m_visualParent(nullptr)
    , m_localStyle(newObject<UIStyle>()) // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    , m_finalStyle()
    , m_renderPriority(0)
    , m_isHitTestVisible(true)
{
}

UIElement::~UIElement()
{
}

void UIElement::init()
{
	UILayoutElement::init(&m_finalStyle);
    m_manager = detail::EngineDomain::uiManager();

	// TODO: Material も、実際に描画が必要な Element に限って作成した方がいいだろう
	m_finalStyle.backgroundMaterial = newObject<Material>();

    UIContainerElement* primaryElement = m_manager->primaryElement();
    if (primaryElement) {
        primaryElement->addElement(this);
    }

    if (m_manager->mainContext()) {
        m_manager->mainContext()->addElement(this);
    }
}

void UIElement::setMargin(const Thickness& margin)
{
    m_localStyle->margin = margin;
}

const Thickness& UIElement::margin() const
{
    return m_localStyle->margin;
}

void UIElement::setPadding(const Thickness& padding)
{
    m_localStyle->padding = padding;
}

const Thickness& UIElement::padding() const
{
    return m_localStyle->padding;
}

void UIElement::setHorizontalAlignment(HAlignment value)
{
	m_localStyle->horizontalAlignment = value;
}

HAlignment UIElement::horizontalAlignment() const
{
	return m_localStyle->horizontalAlignment;
}

void UIElement::setVerticalAlignment(VAlignment value)
{
	m_localStyle->verticalAlignment = value;
}

VAlignment UIElement::verticalAlignment() const
{
	return m_localStyle->verticalAlignment;
}

void UIElement::setPosition(const Vector3 & pos)
{
    m_localStyle->position = pos;
}

const Vector3 & UIElement::position() const
{
    return m_localStyle->position.getOrDefault(Vector3::Zero);
}

void UIElement::setRotation(const Quaternion & rot)
{
    m_localStyle->rotation = rot;
}

const Quaternion & UIElement::rotation() const
{
    return m_localStyle->rotation.getOrDefault(Quaternion::Identity);
}

void UIElement::setScale(const Vector3 & scale)
{
    m_localStyle->scale = scale;
}

const Vector3 & UIElement::scale() const
{
    return m_localStyle->scale.getOrDefault(Vector3::Ones);
}

void UIElement::setCenterPoint(const Vector3 & value)
{
    m_localStyle->centerPoint = value;
}

const Vector3& UIElement::centerPoint() const
{
    return m_localStyle->centerPoint.getOrDefault(Vector3::Zero);
}

void UIElement::setBackgroundColor(const Color& value)
{
	m_localStyle->backgroundColor = value;
}

const Color& UIElement::backgroundColor() const
{
	return m_localStyle->backgroundColor;
}

void UIElement::setBackgroundImage(Texture* value)
{
	m_localStyle->backgroundImage = value;
}

Texture* UIElement::backgroundImage() const
{
	return m_localStyle->backgroundImage.getOrDefault(nullptr);
}

void UIElement::setBackgroundShader(Shader* value)
{
	m_localStyle->backgroundShader = value;
}

Shader* UIElement::backgroundShader() const
{
	return m_localStyle->backgroundShader.getOrDefault(nullptr);
}

void UIElement::setTextColor(const Color& value)
{
	m_localStyle->textColor = value;
}

const Color& UIElement::textColor() const
{
	return m_localStyle->textColor;
}

void UIElement::setFontFamily(const String& value)
{
	m_localStyle->fontFamily = value;
}

const String& UIElement::fontFamily() const
{
	return m_localStyle->fontFamily.getOrDefault(String::Empty);
}

void UIElement::setFontSize(float value)
{
	m_localStyle->fontSize = value;
}

float UIElement::fontSize() const
{
	return m_localStyle->fontSize.getOrDefault(0);
}

void UIElement::setFontWeight(UIFontWeight value)
{
	m_localStyle->fontWeight = value;
}

UIFontWeight UIElement::fontWeight() const
{
	return m_localStyle->fontWeight.getOrDefault(UIFontWeight::Normal);
}

void UIElement::setFontStyle(UIFontStyle value)
{
	m_localStyle->fontStyle = value;
}

UIFontStyle UIElement::fontStyle() const
{
	return m_localStyle->fontStyle.getOrDefault(UIFontStyle::Normal);
}

void UIElement::setVisible(bool value)
{
    m_localStyle->visible = value;
}

bool UIElement::isVisible() const
{
    return m_localStyle->visible.getOrDefault(true);
}

void UIElement::setBlendMode(const Optional<BlendMode>& value)
{
    if (value.hasValue()) {
        m_localStyle->blendMode = value.value();
    }
    else {
        m_localStyle->blendMode.reset();
    }
}

const BlendMode & UIElement::blendMode() const
{
    return m_localStyle->blendMode.getOrDefault(BlendMode::Alpha);
}

void UIElement::setOpacity(float value)
{
    m_localStyle->opacity = value;
}

float UIElement::opacity() const
{
    return m_localStyle->opacity.getOrDefault(detail::BuiltinEffectData::DefaultValue.opacity);
}

void UIElement::setColorScale(const Color & value)
{
    m_localStyle->colorScale = value;
}

const Color & UIElement::colorScale() const
{
    return m_localStyle->colorScale.getOrDefault(detail::BuiltinEffectData::DefaultValue.colorScale);
}

void UIElement::setBlendColor(const Color & value)
{
    m_localStyle->blendColor = value;
}

const Color & UIElement::blendColor() const
{
    return m_localStyle->blendColor.getOrDefault(detail::BuiltinEffectData::DefaultValue.blendColor);
}

void UIElement::setTone(const ToneF & value)
{
    m_localStyle->tone = value;
}

const ToneF & UIElement::tone() const
{
    return m_localStyle->tone.getOrDefault(detail::BuiltinEffectData::DefaultValue.tone);
}

void UIElement::setRenderPriority(int value)
{
    m_renderPriority = value;
}

void UIElement::updateFrame(float elapsedSeconds)
{
    onUpdateFrame(elapsedSeconds);

    // child elements
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->updateFrame(elapsedSeconds);
    }
}

void UIElement::raiseEvent(UIEventArgs* e)
{
    raiseEventInternal(e);
}

UIElement* UIElement::lookupMouseHoverElement(const Point& globalPt)
{
    // 後ろからループする。後のモノが上に描画されるので、この方が自然。
    // TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
    int count = getVisualChildrenCount();
    for (int i = count - 1; i >= 0; i--)
    {
        UIElement* e = static_cast<UIElement*>(getVisualChild(i))->lookupMouseHoverElement(globalPt);
        if (e != nullptr) return e;
    }

    if (m_isHitTestVisible)
    {
        Point localPoint = globalPt;
        if (m_visualParent != nullptr)
        {
            localPoint.x -= m_visualParent->finalGlobalRect().x;
            localPoint.y -= m_visualParent->finalGlobalRect().y;
        }

        if (onHitTest(localPoint)) {
            return this;
        }
    }

    return nullptr;
}

void UIElement::onUpdateFrame(float elapsedSeconds)
{
}

void UIElement::onUpdateStyle(const detail::StyleData& finalStyle)
{
}

void UIElement::onUpdateLayout(const Rect& finalGlobalRect)
{
}

Size UIElement::measureOverride(const Size& constraint)
{
	return UILayoutElement::measureOverride(constraint);
}

Size UIElement::arrangeOverride(const Size& finalSize)
{
	return UILayoutElement::arrangeOverride(finalSize);
}

int UIElement::getVisualChildrenCount() const
{
	return 0;
}

UIElement* UIElement::getVisualChild(int index) const
{
	return nullptr;
}

void UIElement::onRender(UIRenderingContext* context)
{
}

void UIElement::updateStyleHierarchical(const detail::StyleData& parentFinalStyle)
{
	UIStyle::updateStyleDataHelper(m_localStyle, &parentFinalStyle, m_context->defaultStyle(), &m_finalStyle);

	onUpdateStyle(m_finalStyle);

	// child elements
	int count = getVisualChildrenCount();
	for (int i = 0; i < count; i++) {
		getVisualChild(i)->updateStyleHierarchical(m_finalStyle);
	}
}

void UIElement::updateLayoutHierarchical(const Rect& parentFinalGlobalRect)
{
	UILayoutElement::updateLayout(parentFinalGlobalRect);

	onUpdateLayout(finalGlobalRect());

    // child elements
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->updateLayoutHierarchical(finalGlobalRect());
    }
}

void UIElement::render(UIRenderingContext* context)
{
    if (isVisible())
    {


        context->pushState();


        {
            Matrix m = Matrix::makeTranslation(-centerPoint());
            m.scale(scale());
            m.rotateQuaternion(rotation());
            m.translate(position());
            context->setBaseTransfrom(m);
        }
        detail::BuiltinEffectData data;
        data.opacity = opacity();
        data.colorScale = colorScale();
        data.blendColor = blendColor();
        data.tone = tone();
        context->setBaseBuiltinEffectData(data);
        context->setBlendMode(blendMode());
        context->setRenderPriority(m_renderPriority);

		// background
		{
			
			if (m_finalStyle.backgroundColor.a > 0.0f) {
				//auto tex = newObject<Texture2D>(u"D:/Proj/LN/HC1/Assets/Windowskin/window.png");
				//auto mat = Material::create(tex);
				context->setMaterial(m_finalStyle.backgroundMaterial);
				context->drawBoxBackground(finalGlobalRect(), Thickness(16), CornerRadius(), BrushImageDrawMode::BorderFrame, Rect(64, 0, 64, 64), m_finalStyle.backgroundColor);
			}
		}


        // TODO: setMaterial
        onRender(context);

        context->popState();	// TODO: scoped

        //onRender(context);

        // child elements
        int count = getVisualChildrenCount();
        for (int i = 0; i < count; i++) {
            getVisualChild(i)->render(context);
        }
    }
}

void UIElement::onRoutedEvent(UIEventArgs* e)
{
}

bool UIElement::onHitTest(const Point& localPoint)
{
    // TODO:
    return true;
    //return m_finalLocalActualRect.makeDeflate(m_renderFrameThickness).contains(localPoint);
}

void UIElement::raiseEventInternal(UIEventArgs* e)
{
    if (LN_REQUIRE(e)) return;

    // まずは this に通知
    onRoutedEvent(e);
    if (e->handled) return;

    // TODO: bubble
    //if (m_visualParent != nullptr)
    //{
    //    m_visualParent->raiseEventInternal(e);
    //}
}

} // namespace ln

