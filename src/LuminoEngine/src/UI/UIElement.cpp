
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIControl.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>
#include "../Rendering/RenderStage.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIElement
LN_OBJECT_IMPLEMENT(UIElement, UILayoutElement) {}

UIElement::UIElement()
    : m_manager(nullptr)
	, m_objectManagementFlags(detail::ObjectManagementFlags::AutoAddToActiveScene)
    , m_context(nullptr)
    , m_visualParent(nullptr)
    , m_logicalParent(nullptr)
    , m_localStyle(nullptr)
    , m_finalStyle(makeRef<detail::UIStyleInstance>())
	, m_internalVisibility(UIVisibility::Visible)
    , m_renderPriority(0)
    , m_isHitTestVisible(true)
    , m_dirtyFlags(detail::UIElementDirtyFlags::None)
{
    m_localStyle = makeObject<UIStyleClass>(String::Empty); // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    m_localStyle->setMainStyle(makeObject<UIStyle>());
}

UIElement::~UIElement()
{
}

void UIElement::init()
{
	UILayoutElement::init(m_finalStyle);
    m_manager = detail::EngineDomain::uiManager();
    if (LN_REQUIRE(m_manager->mainContext())) return;

    m_dirtyFlags.set(detail::UIElementDirtyFlags::InitialLoading);

	// TODO: Material も、実際に描画が必要な Element に限って作成した方がいいだろう
	m_finalStyle->backgroundMaterial = makeObject<Material>();

	//if (m_objectManagementFlags.hasFlag(detail::ObjectManagementFlags::AutoAddToActiveScene)) {
	//	UIContainerElement* primaryElement = m_manager->primaryElement();
	//	if (primaryElement) {
	//		primaryElement->addElement(this);
	//	}
	//}


    //if (m_manager->mainContext()) {
        m_manager->mainContext()->addElement(this);
    //}

    //onSetup();
}

void UIElement::setWidth(float value)
{
    m_localStyle->mainStyle()->width = value;
}

float UIElement::width() const
{
    return m_localStyle->mainStyle()->width.getOrDefault(UIStyle::DefaultWidth);
}

void UIElement::setHeight(float value)
{
    m_localStyle->mainStyle()->height = value;
}

float UIElement::height() const
{
    return m_localStyle->mainStyle()->height.getOrDefault(UIStyle::DefaultHeight);
}

void UIElement::setMargin(const Thickness& margin)
{
    m_localStyle->mainStyle()->margin = margin;
}

const Thickness& UIElement::margin() const
{
    return m_localStyle->mainStyle()->margin;
}

void UIElement::setPadding(const Thickness& padding)
{
    m_localStyle->mainStyle()->padding = padding;
}

const Thickness& UIElement::padding() const
{
    return m_localStyle->mainStyle()->padding;
}

void UIElement::setHorizontalAlignment(HAlignment value)
{
	m_localStyle->mainStyle()->horizontalAlignment = value;
}

HAlignment UIElement::horizontalAlignment() const
{
	return m_localStyle->mainStyle()->horizontalAlignment;
}

void UIElement::setVerticalAlignment(VAlignment value)
{
	m_localStyle->mainStyle()->verticalAlignment = value;
}

VAlignment UIElement::verticalAlignment() const
{
	return m_localStyle->mainStyle()->verticalAlignment;
}

void UIElement::setPosition(const Vector3 & pos)
{
    m_localStyle->mainStyle()->position = pos;
}

const Vector3 & UIElement::position() const
{
    return m_localStyle->mainStyle()->position.getOrDefault(Vector3::Zero);
}

void UIElement::setRotation(const Quaternion & rot)
{
    m_localStyle->mainStyle()->rotation = rot;
}

const Quaternion & UIElement::rotation() const
{
    return m_localStyle->mainStyle()->rotation.getOrDefault(Quaternion::Identity);
}

void UIElement::setScale(const Vector3 & scale)
{
    m_localStyle->mainStyle()->scale = scale;
}

const Vector3 & UIElement::scale() const
{
    return m_localStyle->mainStyle()->scale.getOrDefault(Vector3::Ones);
}

void UIElement::setCenterPoint(const Vector3 & value)
{
    m_localStyle->mainStyle()->centerPoint = value;
}

const Vector3& UIElement::centerPoint() const
{
    return m_localStyle->mainStyle()->centerPoint.getOrDefault(Vector3::Zero);
}

void UIElement::setBackgroundDrawMode(BrushImageDrawMode value)
{
    m_localStyle->mainStyle()->backgroundDrawMode = value;
}

BrushImageDrawMode UIElement::backgroundDrawMode() const
{
    return m_localStyle->mainStyle()->backgroundDrawMode.getOrDefault(BrushImageDrawMode::Image);
}

void UIElement::setBackgroundColor(const Color& value)
{
	m_localStyle->mainStyle()->backgroundColor = value;
}

const Color& UIElement::backgroundColor() const
{
	return m_localStyle->mainStyle()->backgroundColor;
}

void UIElement::setBackgroundImage(Texture* value)
{
	m_localStyle->mainStyle()->backgroundImage = value;
}

Texture* UIElement::backgroundImage() const
{
	return m_localStyle->mainStyle()->backgroundImage.getOrDefault(nullptr);
}

void UIElement::setBackgroundShader(Shader* value)
{
	m_localStyle->mainStyle()->backgroundShader = value;
}

Shader* UIElement::backgroundShader() const
{
	return m_localStyle->mainStyle()->backgroundShader.getOrDefault(nullptr);
}

void UIElement::setBackgroundImageRect(const Rect& value)
{
    m_localStyle->mainStyle()->backgroundImageRect = value;
}

const Rect& UIElement::backgroundImageRect() const
{
    return m_localStyle->mainStyle()->backgroundImageRect.getOrDefault(Rect::Zero);
}

void UIElement::setBackgroundImageBorder(const Thickness& value)
{
    m_localStyle->mainStyle()->backgroundImageBorder = value;
}

const Thickness& UIElement::backgroundImageBorder() const
{
    return m_localStyle->mainStyle()->backgroundImageBorder.getOrDefault(Thickness::Zero);
}

void UIElement::setBorderThickness(const Thickness& value)
{
	m_localStyle->mainStyle()->borderThickness = value;
}

const Thickness& UIElement::borderThickness() const
{
	return m_localStyle->mainStyle()->borderThickness.getOrDefault(Thickness::Zero);
}

void UIElement::setBorderColor(const Color& value)
{
	m_localStyle->mainStyle()->setBorderColor(value);
}

const Color& UIElement::borderColor() const
{
	return m_localStyle->mainStyle()->backgroundColor;
}

void UIElement::setTextColor(const Color& value)
{
	m_localStyle->mainStyle()->textColor = value;
}

const Color& UIElement::textColor() const
{
	return m_localStyle->mainStyle()->textColor;
}

void UIElement::setFontFamily(const String& value)
{
	m_localStyle->mainStyle()->fontFamily = value;
}

const String& UIElement::fontFamily() const
{
	return m_localStyle->mainStyle()->fontFamily.getOrDefault(String::Empty);
}

void UIElement::setFontSize(float value)
{
	m_localStyle->mainStyle()->fontSize = value;
}

float UIElement::fontSize() const
{
	return m_localStyle->mainStyle()->fontSize.getOrDefault(0);
}

void UIElement::setFontWeight(UIFontWeight value)
{
	m_localStyle->mainStyle()->fontWeight = value;
}

UIFontWeight UIElement::fontWeight() const
{
	return m_localStyle->mainStyle()->fontWeight.getOrDefault(UIFontWeight::Normal);
}

void UIElement::setFontStyle(UIFontStyle value)
{
	m_localStyle->mainStyle()->fontStyle = value;
}

UIFontStyle UIElement::fontStyle() const
{
	return m_localStyle->mainStyle()->fontStyle.getOrDefault(UIFontStyle::Normal);
}

void UIElement::setVisible(bool value)
{
    m_localStyle->mainStyle()->visible = value;
}

bool UIElement::isVisible() const
{
    return m_localStyle->mainStyle()->visible.getOrDefault(true);
}

void UIElement::setBlendMode(const Optional<BlendMode>& value)
{
    if (value.hasValue()) {
        m_localStyle->mainStyle()->blendMode = value.value();
    }
    else {
        m_localStyle->mainStyle()->blendMode.reset();
    }
}

BlendMode UIElement::blendMode() const
{
    return m_localStyle->mainStyle()->blendMode.getOrDefault(BlendMode::Alpha);
}

void UIElement::setOpacity(float value)
{
    m_localStyle->mainStyle()->opacity = value;
}

float UIElement::opacity() const
{
    return m_localStyle->mainStyle()->opacity.getOrDefault(detail::BuiltinEffectData::DefaultValue.opacity);
}

void UIElement::setColorScale(const Color & value)
{
    m_localStyle->mainStyle()->colorScale = value;
}

const Color & UIElement::colorScale() const
{
    return m_localStyle->mainStyle()->colorScale.getOrDefault(detail::BuiltinEffectData::DefaultValue.colorScale);
}

void UIElement::setBlendColor(const Color & value)
{
    m_localStyle->mainStyle()->blendColor = value;
}

const Color & UIElement::blendColor() const
{
    return m_localStyle->mainStyle()->blendColor.getOrDefault(detail::BuiltinEffectData::DefaultValue.blendColor);
}

void UIElement::setTone(const ColorTone & value)
{
    m_localStyle->mainStyle()->tone = value;
}

const ColorTone & UIElement::tone() const
{
    return m_localStyle->mainStyle()->tone.getOrDefault(detail::BuiltinEffectData::DefaultValue.tone);
}

UIContext* UIElement::getContext() const
{
    if (m_context) {
        return m_context;
    }
    if (m_visualParent) {
        return m_visualParent->getContext();
    }
    LN_ERROR();
    return nullptr;
}

void UIElement::addClass(const StringRef& className)
{
    if (!m_classList) {
        m_classList = makeList<String>();
    }
    m_classList->add(className);
}

void UIElement::setViewModel(UIViewModel* value)
{
	if (m_viewModel != value) {
		auto old = m_viewModel;
		m_viewModel = value;
		onViewModelChanged(m_viewModel, old);
	}
}

void UIElement::setContent(UIElement* content)
{
    LN_UNREACHABLE();
}

void UIElement::setContent(const String& content)
{
    auto textblock = makeObject<UITextBlock>();
    textblock->setText(content);
    setContent(textblock);
}

void UIElement::addChild(UIElement* child)
{
    LN_UNREACHABLE();
}

void UIElement::addChild(const String& child)
{
    auto textblock = makeObject<UITextBlock>();
    textblock->setText(child);
    addChild(textblock);
}

void UIElement::addAction(UIAction* action)
{
    if (!m_actions) {
        m_actions = ln::makeList<Ref<UIAction>>();
    }
    m_actions->add(action);
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

void UIElement::postEvent(UIEventArgs* e)
{
    m_manager->postEvent(this, e);
}

UIElement* UIElement::getFrameWindow()
{
	if (m_specialElementFlags.hasFlag(detail::UISpecialElementFlags::FrameWindow)) {
		return this;
	}
	if (m_visualParent) {
		return m_visualParent->getFrameWindow();
	}
	return nullptr;
}

UIElement* UIElement::lookupMouseHoverElement(const Point& frameClientPosition)
{
    // 後ろからループする。後のモノが上に描画されるので、この方が自然。
    // TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
    int count = getVisualChildrenCount();
    for (int i = count - 1; i >= 0; i--)
    {
        UIElement* e = static_cast<UIElement*>(getVisualChild(i))->lookupMouseHoverElement(frameClientPosition);
        if (e != nullptr) return e;
    }

    if (m_isHitTestVisible)
    {
        //Point localPoint = frameClientPosition;
        //if (m_visualParent != nullptr)
        //{
        //    localPoint.x -= m_visualParent->finalGlobalRect().x;
        //    localPoint.y -= m_visualParent->finalGlobalRect().y;
        //}

        if (onHitTest(frameClientPosition)) {
            return this;
        }
    }

    return nullptr;
}

void UIElement::focus()
{
    m_manager->focus(this);
}

void UIElement::retainCapture()
{
	m_manager->retainCapture(this);
}

void UIElement::releaseCapture()
{
	m_manager->releaseCapture(this);
}

// Note: 結局 WPF のように getVisualChildCount() と getVisualChild() をオーバーライドする方式はやめた。
// 全ての Element は parent を持つ必要があるし、それの主な理由は Event の Bubble 実装のため。
// なので、この parent は visual-parent でなければならない。
// そうするとあらゆる set, add 子要素なところで setParent() する必要があるし、↑の2つを実装しなければならない。
// また、フォーカス変化などの全面表示(ZOrder) と タブオーダーは区別する必要があり、ひとつのリストだけでは非常に大変。
// - m_visualChildren -> ZOrder (フレームワーク内部で順序を変えることがある)
// - m_logicalChildren -> タブオーダー (フレームワーク内部で順序を変えることはない)
// みたいにしておくといろいろやりやすい。
// ダブルでリスト持つメモリ消費量はあるけど、その最適化はアーキテクチャ固めてからかな・・・。
	// マウスのヒットテスト (Hierarchical)
	// スタイル更新 (Hierarchical)
	// フレーム更新 (Hierarchical)
	// 描画 (Hierarchical)
	// ※レイアウトは対象外
void UIElement::addVisualChild(UIElement* element)
{
	if (LN_REQUIRE(element != nullptr)) return;
	if (LN_REQUIRE(element->m_visualParent == nullptr)) return;

	// リストが作成されていなければ、ここで始めて作る (メモリ消費量対策)
	if (!m_visualChildren) {
		m_visualChildren = makeList<Ref<UIElement>>();
	}

	m_visualChildren->add(element);
	element->m_visualParent = this;

	// TODO: ZOrder
	//std::stable_sort(
	//	m_visualChildren->begin(), m_visualChildren->end(),
	//	[](const Ref<UIElement>& lhs, const Ref<UIElement>& rhs)
	//	{
	//		// TODO: ZOrder 指定
	//		int li = lhs->readCoreFlag(detail::UICoreFlags_AdornerLayer) ? 1 : 0;
	//		int ri = rhs->readCoreFlag(detail::UICoreFlags_AdornerLayer) ? 1 : 0;
	//		return li < ri;
	//	});

    element->invalidate(detail::UIElementDirtyFlags::Style | detail::UIElementDirtyFlags::Layout, true);
}

void UIElement::removeVisualChild(UIElement* element)
{
	//LN_FAIL_CHECK_ARG(element != nullptr) return;
	if (element == nullptr) return;
	if (m_visualChildren == nullptr) return;

	m_visualChildren->remove(element);
	element->m_visualParent = nullptr;
}

//void UIElement::onSetup()
//{
//}

void UIElement::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
}

void UIElement::onLoaded()
{
}

void UIElement::onUpdateFrame(float elapsedSeconds)
{
}

void UIElement::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
}
//
//void UIElement::onUpdateLayout(const Rect& finalGlobalRect)
//{
//}

Size UIElement::measureOverride(const Size& constraint)
{
	// Note: Web は padding が大きくなると、要素のサイズ自体も大きくなる。というより、
	// Web: width,height はコンテンツエリアのサイズ。
	// WPF: width,height は margin の内側のエリアのサイズ。

	auto size = UILayoutElement::measureOverride(constraint);
    //size.width += m_finalStyle->borderThickness.width();
    //size.height += m_finalStyle->borderThickness.height();

	//size.width += m_finalStyle->padding.width();
	//size.height += m_finalStyle->padding.height();



    return size;
}

Size UIElement::arrangeOverride(const Size& finalSize)
{
	return UILayoutElement::arrangeOverride(finalSize);
}

//int UIElement::getVisualChildrenCount() const
//{
//	return 0;
//}
//
//UIElement* UIElement::getVisualChild(int index) const
//{
//	return nullptr;
//}

void UIElement::onRender(UIRenderingContext* context)
{
}

void UIElement::updateStyleHierarchical(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle)
{
	if (!m_combinedStyle) {
		m_combinedStyle = makeObject<UIStyle>();
	}

	m_combinedStyle->reset();
	styleContext->combineStyle(m_combinedStyle, elementName(), m_classList);
	if (m_visualStateManager) {
		m_visualStateManager->combineStyle(m_combinedStyle, styleContext, elementName(), m_classList);
        m_visualStateManager->combineStyle(m_combinedStyle, m_localStyle);
	}
    else {
        m_combinedStyle->mergeFrom(m_localStyle->mainStyle());
    }

    //UIStyle* combinedStyle;
    //if (m_visualStateManager) {
    //    combinedStyle = m_visualStateManager->combineStyle(styleContext, elementName());
    //}
    //else {
    //    auto sc = styleContext->findStyleClass(elementName());
    //    combinedStyle = sc->style();
    //    
    //    //auto sc = styleContext->findResolvedStyleClass();
    //    //resolvedStyle = sc->style();
    //}

	detail::UIStyleInstance::updateStyleDataHelper(/*m_localStyle, */parentFinalStyle, m_combinedStyle, m_finalStyle);

	onUpdateStyle(styleContext, m_finalStyle);

	// child elements
	int count = getVisualChildrenCount();
	for (int i = 0; i < count; i++) {
		getVisualChild(i)->updateStyleHierarchical(styleContext, m_finalStyle);
	}

	m_dirtyFlags.unset(detail::UIElementDirtyFlags::Style);

	// Re-layout
    // TODO: 本当にレイアウトに影響するプロパティが変わったときだけにしたい
	invalidate(detail::UIElementDirtyFlags::Layout, true);
}

void UIElement::updateFinalLayoutHierarchical(const Rect& parentFinalGlobalRect)
{
    updateFinalRects(parentFinalGlobalRect);

    // child elements
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->updateFinalLayoutHierarchical(m_finalGlobalRect);
    }

	m_dirtyFlags.unset(detail::UIElementDirtyFlags::Layout);

	// Re-draw
    // TODO: 本当に描画に影響するプロパティが変わったときだけにしたい
	invalidate(detail::UIElementDirtyFlags::Render, true);
}

//void UIElement::updateLayoutHierarchical(const Rect& parentFinalGlobalRect)
//{
//	UILayoutElement::updateLayout(parentFinalGlobalRect);
//
//	//onUpdateLayout(finalGlobalRect());
//
// //   // child elements
// //   int count = getVisualChildrenCount();
// //   for (int i = 0; i < count; i++) {
// //       getVisualChild(i)->updateLayoutHierarchical(finalGlobalRect());
// //   }
//}

void UIElement::render(UIRenderingContext* context)
{
    if (isVisible() && (m_internalVisibility == UIVisibility::Visible))
    {


        context->pushState();


        {
            Matrix m = Matrix::makeTranslation(-centerPoint());
            m.scale(scale());
            m.rotateQuaternion(rotation());
            m.translate(position());
            m.translate(Vector3(m_finalGlobalRect.x, m_finalGlobalRect.y, 0));
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
			context->setMaterial(m_finalStyle->backgroundMaterial);

			if (m_finalStyle->shadowBlurRadius > 0.0f)
			{
				context->drawBoxShadow(Rect(0, 0, m_finalGlobalRect.getSize()), m_finalStyle->cornerRadius, Vector2(m_finalStyle->shadowOffsetX, m_finalStyle->shadowOffsetY), m_finalStyle->shadowColor, m_finalStyle->shadowBlurRadius, m_finalStyle->shadowSpreadRadius, m_finalStyle->shadowInset);

			}
			Rect rect(0, 0, m_finalGlobalRect.getSize());
			rect = rect.makeDeflate(m_finalStyle->borderThickness);
			
			if (m_finalStyle->backgroundColor.a > 0.0f) {
				//auto tex = makeObject<Texture2D>(u"D:/Proj/LN/HC1/Assets/Windowskin/window.png");
				//auto mat = Material::create(tex);
                context->drawBoxBackground(rect, m_finalStyle->cornerRadius, m_finalStyle->backgroundDrawMode, m_finalStyle->backgroundImageRect, m_finalStyle->backgroundColor);
				//context->drawBoxBackground(finalGlobalRect(), Thickness(16), CornerRadius(), BrushImageDrawMode::BorderFrame, Rect(64, 0, 64, 64), m_finalStyle->backgroundColor);
			}
			if (!m_finalStyle->borderThickness.isZero()) {
				context->drawBoxBorderLine(rect, m_finalStyle->borderThickness, m_finalStyle->leftBorderColor, m_finalStyle->topBorderColor, m_finalStyle->rightBorderColor, m_finalStyle->bottomBorderColor, m_finalStyle->cornerRadius, false);
			}
		}


        // TODO: setMaterial
        onRender(context);

		for (auto& d : m_finalStyle->decorators) {
			d->render(context, m_actualSize);
		}

        context->popState();	// TODO: scoped

        // child elements
        int count = getVisualChildrenCount();
        for (int i = 0; i < count; i++) {
            getVisualChild(i)->render(context);
        }
    }

    m_dirtyFlags.unset(detail::UIElementDirtyFlags::Render);
}

void UIElement::onRoutedEvent(UIEventArgs* e)
{
    if (detail::UICommandInternal::handleCommandRoutedEvent(e, m_actions)) {
        return;
    }
}

bool UIElement::onHitTest(const Point& frameClientPosition)
{
    auto inv = Matrix::makeInverse(m_combinedFinalTransform);
    auto pos = Vector3::transformCoord(Vector3(frameClientPosition.x, frameClientPosition.y, .0f), inv);

    if (0 <= pos.x && pos.x < m_actualSize.width &&
        0 <= pos.y && pos.y < m_actualSize.height) {
        return true;
    }
    else {
        return false;
    }
}

bool UIElement::isMouseHover() const
{
    return m_manager->mouseHoverElement() == this;
}

void UIElement::removeFromLogicalParent()
{
    if (m_logicalParent) {
        m_logicalParent->removeElement(this);
    }
}

void UIElement::raiseEventInternal(UIEventArgs* e)
{
    if (LN_REQUIRE(e)) return;

    // まずは this に通知
    onRoutedEvent(e);
    if (e->handled) return;

    // bubble
    if (m_visualParent) {
        m_visualParent->raiseEventInternal(e);
    }
}

void UIElement::invalidate(detail::UIElementDirtyFlags flags, bool toAncestor)
{
    m_dirtyFlags.set(flags);
    if (toAncestor && m_visualParent) {
        m_visualParent->invalidate(flags, toAncestor);
    }
}

detail::GridLayoutInfo* UIElement::getGridLayoutInfo()
{
    if (!m_gridLayoutInfo) {
        m_gridLayoutInfo = std::make_unique<detail::GridLayoutInfo>();
    }
    return m_gridLayoutInfo.get();
}

UIVisualStateManager* UIElement::getVisualStateManager()
{
    if (!m_visualStateManager) {
        m_visualStateManager = makeObject<UIVisualStateManager>(this);
    }
    return m_visualStateManager;
}

} // namespace ln

