
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
#include <LuminoEngine/UI/Controls/UIControl.hpp>
#include <LuminoEngine/UI/UIText.hpp>
#include "../Rendering/RenderStage.hpp"
#include "UIStyleInstance.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIViewModel

void UIViewModel::notify(const StringRef& propertyName)
{
    auto args = UINotifyPropertyChangedEventArgs::create(nullptr, UIEvents::NotifyPropertyChanged, propertyName);
    notify(args);
}

void UIViewModel::notify(UINotifyPropertyChangedEventArgs* e)
{
    for (auto& target : m_observers) {
        target->onSourcePropertyChanged(e);
    }
}

//==============================================================================
// UIElement::Builder
/*
UIElement::Builder::Builder()
	: Builder(makeRef<Details>())
{
}

UIElement::Builder::Builder(Details* d)
	: BuilderBase(d)
{
}

UIElement::Builder& UIElement::Builder::height(float value)
{
	detailsAs<Details>()->height = value;
	return *this;
}

UIElement::Builder& UIElement::Builder::backgroundColor(const Color& value)
{
	detailsAs<Details>()->backgroundColor = value;
	return *this;
}

Ref<UIElement> UIElement::Builder::build()
{
	return buildAs<UIElement>();
}

Ref<Object> UIElement::Builder::Details::build()
{
	auto ptr = makeObject<UIElement>();
	if (height) ptr->setHeight(*height);
	if (backgroundColor) ptr->setBackgroundColor(*backgroundColor);
	return ptr;
}
*/

void UIElement::BuilderDetails::apply(UIElement* p) const
{
    if (width) p->setWidth(*width);
    if (height) p->setHeight(*height);
    if (backgroundColor) p->setBackgroundColor(*backgroundColor);
    if (hAlignment) p->setHAlignment(*hAlignment);
    if (vAlignment) p->setVAlignment(*vAlignment);
}

//==============================================================================
// UIElement

LN_OBJECT_IMPLEMENT(UIElement, UILayoutElement) {}

UIElement::UIElement()
    : m_manager(nullptr)
	, m_objectManagementFlags(detail::ObjectManagementFlags::None)
    //, m_context(nullptr)
    , m_visualParent(nullptr)
    , m_logicalParent(nullptr)
    , m_localStyle(nullptr)
    , m_finalStyle(makeRef<detail::UIStyleInstance>())
	, m_internalVisibility(UIVisibility::Visible)
    , m_renderPriority(0)
    , m_hitTestMode(detail::UIHitTestMode::Visible)
	, m_focusable(false)
    , m_clipToBounds(false)
    , m_dirtyFlags(detail::UIElementDirtyFlags::None)
{
    m_localStyle = makeObject<UIStyleClass>(String::Empty); // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
    m_localStyle->setMainStyle(makeObject<UIStyle>());
    m_specialElementFlags.set(detail::UISpecialElementFlags::Enabled, true);
    m_specialElementFlags.set(detail::UISpecialElementFlags::InternalEnabled, true);
}

UIElement::~UIElement()
{
    if (m_viewModel) {
        m_viewModel->unsubscribe(this);
    }
}

void UIElement::onDispose(bool explicitDisposing)
{
    if (m_manager) {
        m_manager->onElementDisposing(this);
        m_manager = nullptr;
    }

    UILayoutElement::onDispose(explicitDisposing);
}

bool UIElement::init()
{
	UILayoutElement::init(m_finalStyle);
    m_manager = detail::EngineDomain::uiManager();
    //if (LN_REQUIRE(m_manager->mainContext())) return false;

    m_dirtyFlags.set(detail::UIElementDirtyFlags::InitialLoading);

	// TODO: Material も、実際に描画が必要な Element に限って作成した方がいいだろう
	m_finalStyle->backgroundMaterial = makeObject<Material>();

	//if (context->m_autoAddToPrimaryElement &&
 //       m_objectManagementFlags.hasFlag(detail::ObjectManagementFlags::AutoAddToPrimaryElement)) {
	//	attemptAddToPrimaryElement();
	//}


    //if (m_manager->mainContext()) {
        //m_manager->mainContext()->addElement(this);
    //}

    //onSetup();

    return true;
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

void UIElement::setHAlignment(UIHAlignment value)
{
	m_localStyle->mainStyle()->hAlignment = value;
}

UIHAlignment UIElement::hAlignment() const
{
	return m_localStyle->mainStyle()->hAlignment;
}

void UIElement::setVAlignment(UIVAlignment value)
{
	m_localStyle->mainStyle()->vAlignment = value;
}

UIVAlignment UIElement::vAlignment() const
{
	return m_localStyle->mainStyle()->vAlignment;
}

void UIElement::setAlignments(UIHAlignment halign, UIVAlignment valign)
{
	setHAlignment(halign);
	setVAlignment(valign);
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

void UIElement::setEnabled(bool value)
{
    bool oldValue = isEnabled();
    m_specialElementFlags.set(detail::UISpecialElementFlags::Enabled, value);
    if (oldValue != isEnabled()) {
        updateEnabledPropertyOnChildren();
        onEnabledChanged();
    }
}

bool UIElement::isEnabled() const
{
    if (!m_specialElementFlags.hasFlag(detail::UISpecialElementFlags::InternalEnabled)) {
        // Parent is disabled.
        return false;
    }

    return m_specialElementFlags.hasFlag(detail::UISpecialElementFlags::Enabled);
}

void UIElement::setData(Variant* value)
{
    m_data = value;
}

Variant* UIElement::data() const
{
    return m_data;
}

void UIElement::setBackgroundDrawMode(Sprite9DrawMode value)
{
    m_localStyle->mainStyle()->backgroundDrawMode = value;
}

Sprite9DrawMode UIElement::backgroundDrawMode() const
{
    return m_localStyle->mainStyle()->backgroundDrawMode.getOrDefault(Sprite9DrawMode::StretchedSingleImage);
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

void UIElement::setCornerRadius(const CornerRadius& value)
{
    m_localStyle->mainStyle()->cornerRadius = value;
}

const CornerRadius& UIElement::cornerRadius() const
{
    return m_localStyle->mainStyle()->cornerRadius.getOrDefault(CornerRadius::Zero);
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

void UIElement::setVisibility(UIVisibility value)
{
    m_localStyle->mainStyle()->visible = value;
}

UIVisibility UIElement::visibility() const
{
    return m_localStyle->mainStyle()->visible.getOrDefault(UIVisibility::Visible);
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

//UIContext* UIElement::getContext() const
//{
//    if (m_context) {
//        return m_context;
//    }
//    if (m_visualParent) {
//        return m_visualParent->getContext();
//    }
//    LN_ERROR();
//    return nullptr;
//}

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
        if (old) {
            old->unsubscribe(this);
        }

		m_viewModel = value;
        m_viewModel->subscribe(this);
		onViewModelChanged(m_viewModel, old);
	}
}

void UIElement::setContent(UIElement* content)
{
    LN_UNREACHABLE();
}

void UIElement::setContent(const String& content)
{
    // TODO: CreationContext とか用意したほうがいいかも。init を public にしないとダメだし。
    //m_expanderButton = makeObject<UIToggleButton>();
    auto textblock = makeRef<UIText>();
    textblock->m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
    textblock->init();

    //auto textblock = makeObject<UIText>();
    textblock->setText(content);
    setContent(textblock);
}

void UIElement::addChild(UIElement* child)
{
    onAddChild(child);
}

void UIElement::addChild(const String& child)
{
    auto textblock = makeObject<UIText>();
    textblock->setText(child);
    addChild(textblock);
}

//void UIElement::activate()
//{
//	//if (m_focusable) {
//	//	activateInternal();
//	//}
//	m_manager->tryGetInputFocus(this);
//}

void UIElement::addInto(UIElement* parent)
{
    UIElement* primaryElement = (parent) ? parent : m_manager->primaryElement();
    primaryElement->addChild(this);
}

void UIElement::setRenderPriority(int value)
{
    m_renderPriority = value;
}

void UIElement::updateFrame(float elapsedSeconds)
{
    onUpdateFrame(elapsedSeconds);

    if (m_finalStyle && m_finalStyle->hasAnimationData()) {
        m_finalStyle->advanceAnimation(elapsedSeconds);

        // Style 更新が要求されているなら、次の updateStyle で同じ Animation 適用の処理が動くので、ここで行う必要はない。
        if (m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Style)) {
            auto dirtyFlags = m_finalStyle->applyAnimationValues();
            invalidate(dirtyFlags, true);
        }
    }

    // child elements
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->updateFrame(elapsedSeconds);
    }
}

void UIElement::raiseEvent(UIEventArgs* e, UIEventRoutingStrategy strategy)
{
    raiseEventInternal(e, strategy);
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

UIFrameRenderView* UIElement::getRenderView()
{
    if (m_renderView) {
        return m_renderView;
    }
    if (m_visualParent) {
        return m_visualParent->getRenderView();
    }
    return nullptr;
}

UIElement* UIElement::lookupMouseHoverElement(const Point& frameClientPosition)
{
	if (!isRenderVisible()) return nullptr;
    if (!isEnabled()) return nullptr;

	if (m_hitTestMode == detail::UIHitTestMode::Visible ||
		m_hitTestMode == detail::UIHitTestMode::InvisiblePanel)
	{
		if (m_orderdVisualChildren) {
			for (int i = m_orderdVisualChildren->size() - 1; i >= 0; i--) {
				auto* e = static_cast<UIElement*>(m_orderdVisualChildren[i])->lookupMouseHoverElement(frameClientPosition);
				if (e) {
					return e;
				}
			}
		}
	}

	if (m_hitTestMode == detail::UIHitTestMode::Visible)
	{
#if 1
#else
        // 後ろからループする。後のモノが上に描画されるので、この方が自然。
        // TODO: Zオーダーは別のリストにしたほうがいい気がする・・・
        int count = getVisualChildrenCount();
        for (int i = count - 1; i >= 0; i--)
        {
            UIElement* e = static_cast<UIElement*>(getVisualChild(i))->lookupMouseHoverElement(frameClientPosition);
            if (e != nullptr) return e;
        }
#endif

		if (onHitTest(frameClientPosition)) {
			return this;
		}
    }

    return nullptr;
}

void UIElement::focus()
{
    if (focusable()) {
        UIElement* leaf = findFocusedVisualChildLeaf();
        if (LN_REQUIRE(leaf)) return;

        m_manager->tryGetInputFocus(leaf);
    }
}

void UIElement::retainCapture()
{
	m_manager->retainCapture(this);
}

void UIElement::releaseCapture()
{
	m_manager->releaseCapture(this);
}

// NOTE: このあたりの grab/release はプロパティにしないの？
// → get で現在のキャプチャされているかどうかを知ることができない。(OS によって厳しく制御されるため)
//   なのでふるまいではなく状態として扱っても、その状態を取り合わせる仕組みが無い。
//   プロパティとしてはちょっと不自然。
void UIElement::grabCursor()
{
    m_manager->grabCursor(this);
}

void UIElement::releaseCursor()
{
    m_manager->releaseCursor(this);
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
		m_orderdVisualChildren = ln::makeList<UIElement*>();
	}

	m_visualChildren->add(element);
	m_orderdVisualChildren->add(element);
	element->m_visualParent = this;


    updateEnabledPropertyOnChildren();

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
	m_orderdVisualChildren->remove(element);
	element->m_visualParent = nullptr;

    if (m_focusedVisualChild == element)
        m_focusedVisualChild = nullptr;

    {
        bool oldValue = element->isEnabled();
        element->m_specialElementFlags.set(detail::UISpecialElementFlags::InternalEnabled, false);
        if (oldValue != element->isEnabled()) {
            element->updateEnabledPropertyOnChildren();
            element->onEnabledChanged();
        }
    }

    invalidateLayout();
}

void UIElement::removeAllVisualChild()
{
    if (m_visualChildren) {
        for (auto& child : *m_visualChildren) child->m_visualParent = nullptr;
        m_visualChildren->clear();
        m_orderdVisualChildren->clear();
    }
    invalidateLayout();
}

//void UIElement::onSetup()
//{
//}

void UIElement::onViewModelChanged(UIViewModel* newViewModel, UIViewModel* oldViewModel)
{
    if (oldViewModel) {
        LN_NOTIMPLEMENTED();
    }

    newViewModel->subscribe(this);
}

void UIElement::onSourcePropertyChanged(UINotifyPropertyChangedEventArgs* e)
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

Size UIElement::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	// Note: Web は padding が大きくなると、要素のサイズ自体も大きくなる。というより、
	// Web: width,height はコンテンツエリアのサイズ。
	// WPF: width,height は margin の内側のエリアのサイズ。

	auto size = UILayoutElement::measureOverride(layoutContext, constraint);
    //size.width += m_finalStyle->borderThickness.width();
    //size.height += m_finalStyle->borderThickness.height();

	//size.width += m_finalStyle->padding.width();
	//size.height += m_finalStyle->padding.height();



    return size;
}

Size UIElement::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	return UILayoutElement::arrangeOverride(layoutContext, finalArea);
}

void UIElement::arrangeLayout(UILayoutContext* layoutContext, const Rect& localSlotRect)
{
    UILayoutElement::arrangeLayout(layoutContext, localSlotRect);


    //updateFinalRects(layoutContext, (m_visualParent) ? m_visualParent->m_combinedFinalRenderTransform : Matrix::Identity);

    //// child elements
    //int count = getVisualChildrenCount();
    //for (int i = 0; i < count; i++) {
    //    getVisualChild(i)->updateFinalLayoutHierarchical(layoutContext, m_combinedFinalRenderTransform);
    //}

    m_dirtyFlags.unset(detail::UIElementDirtyFlags::Layout);

    // Re-draw
    // TODO: 本当に描画に影響するプロパティが変わったときだけにしたい
    invalidate(detail::UIElementDirtyFlags::Render, true);
}

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

    const UIVisualStateManager* actualVisualStateManager = nullptr;
    if (m_partParent && m_partParent->m_visualStateManager) {
        actualVisualStateManager = m_partParent->m_visualStateManager;
    }
    else if (m_visualStateManager) {
        actualVisualStateManager = m_visualStateManager;
    }


	if (actualVisualStateManager) {
        actualVisualStateManager->combineStyle(m_combinedStyle, styleContext, elementName(), m_classList);
        actualVisualStateManager->combineStyle(m_combinedStyle, m_localStyle);
        //actualVisualStateManager->printActive();
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

	detail::UIStyleInstance::updateStyleDataHelper(styleContext, /*m_localStyle, */parentFinalStyle, m_combinedStyle, m_finalStyle);

	m_finalStyle->theme = styleContext->mainTheme;
    
    m_finalStyle->updateAnimationData();
    m_finalStyle->applyAnimationValues();

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

void UIElement::updateFinalLayoutHierarchical(UILayoutContext* layoutContext, const Matrix& parentCombinedRenderTransform)
{
    // NOTE: 行列更新パスはいったん arrangeLayout でやることにしてみる。Popup でターゲットの位置を知りたいときに、グローバル座標が決定していないと計算が大変になるので。
    // 
    // [2020/8/19] やっぱり arrangeLayout でやるのは都合が悪かった。
    // 親要素は子要素の arrangeLayout の結果を arrangeOverride で受け取ってからローカル位置を決定する。
    // この時 子要素の arrangeLayout を呼んでいるので、arrangeLayoutでグローバル行列更新するということは、親のグローバル行列もこの時点で決まっている必要がある。
    // しかし、このパスだとそれは不可能。
    // もともとは Popup のターゲット位置のための対応だったが、やむなし。
    // Popup なら普通は前フレームで位置は計算済みなので、それを使う、でもいいかも。

    updateFinalRects(layoutContext, parentCombinedRenderTransform);

    // child elements
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->updateFinalLayoutHierarchical(layoutContext, m_combinedFinalRenderTransform);
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

void UIElement::render(UIRenderingContext* context, const Matrix& parentTransform)
{
    bool enable = false;
    if (m_specialElementFlags.hasFlag(detail::UISpecialElementFlags::Popup)) {
        if (context->m_adornerRendering) {
            enable = true;
        }
    }
    else {
        enable = true;
    }


    if (enable && isRenderVisible())
    {
        m_actuialBuiltinEffectData.opacity = m_finalStyle->opacity;
        m_actuialBuiltinEffectData.colorScale = m_finalStyle->colorScale;
        m_actuialBuiltinEffectData.blendColor = m_finalStyle->blendColor;
        m_actuialBuiltinEffectData.tone = m_finalStyle->tone;

        if (m_visualParent) {
            m_actuialBuiltinEffectData.inherit(m_visualParent->m_actuialBuiltinEffectData);
        }

        if (!isEnabled()) {
            // Grayscale
            m_actuialBuiltinEffectData.tone.s = 1.0f;
        }

        if (m_actuialBuiltinEffectData.opacity < 0.0001f ||
            m_actuialBuiltinEffectData.colorScale.a < 0.0001f) {
            // Alpha test
        }
        else {
            context->m_theme = m_finalStyle->theme;
            renderClient(context, m_combinedFinalRenderTransform);
        }

    }

    m_dirtyFlags.unset(detail::UIElementDirtyFlags::Render);
}

void UIElement::renderClient(UIRenderingContext* context, const Matrix& combinedTransform)
{
	context->pushState();

	if (m_clipToBounds) {
		Vector2 points[] = {
			Vector3::transformCoord({ 0, 0, 0 }, combinedTransform).xy(),
			Vector3::transformCoord({ actualSize().width, 0, 0 }, combinedTransform).xy(),
			Vector3::transformCoord({ 0, actualSize().height, 0 }, combinedTransform).xy(),
			Vector3::transformCoord({ actualSize().width, actualSize().height, 0 }, combinedTransform).xy(),
		};
		Vector2 minPos = points[0];
		Vector2 maxPos = points[0];
		for (int i = 1; i < 4; i++) {
			minPos = Vector2::min(minPos, points[i]);
			maxPos = Vector2::max(maxPos, points[i]);
		}
		context->setScissorRect(RectI(minPos.x, minPos.y, maxPos.x - minPos.x, maxPos.y - minPos.y));
	}

	{
		//Matrix m = Matrix::makeTranslation(-centerPoint());
		//m.scale(scale());
		//m.rotateQuaternion(rotation());
		//m.translate(position());
		//m.translate(Vector3(m_finalGlobalRect.x, m_finalGlobalRect.y, 0));
		context->setBaseTransfrom(combinedTransform);
	}


	context->setBaseBuiltinEffectData(m_actuialBuiltinEffectData);
    //context->setBaseBuiltinEffectData(m_finalStyle->builtinEffect);
	context->setBlendMode(blendMode());
	context->setRenderPriority(m_renderPriority);

	context->setFont(m_finalStyle->font);
	context->setTextColor(m_finalStyle->textColor);
		

	// background
	{
		context->setMaterial(m_finalStyle->backgroundMaterial);

        if (m_finalStyle->backgroundMaterial && m_finalStyle->backgroundMaterial->mainTexture()) {
            context->drawImageBox(Rect(0, 0, actualSize()), m_finalStyle->backgroundDrawMode, m_finalStyle->backgroundImageRect, m_finalStyle->backgroundImageBorder, Color::White);
        }

#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
        {
            BoxElementShapeBaseStyle baseStyle;
            BoxElementShapeBackgroundStyle backgroundStyle;
            BoxElementShapeBorderStyle borderStyle;
            BoxElementShapeShadowStyle shadowStyle;

            baseStyle.baseRect = Rect(0, 0, actualSize());// .makeDeflate(m_finalStyle->borderThickness);
            baseStyle.baseRect.width -= m_finalStyle->borderThickness.width();
            baseStyle.baseRect.height -= m_finalStyle->borderThickness.height();
            baseStyle.cornerRadius = m_finalStyle->cornerRadius;

            const BoxElementShapeBackgroundStyle* actualBackgroundStyle = nullptr;
            if (m_finalStyle->backgroundColor.a > 0.0f) {
                backgroundStyle.color = m_finalStyle->backgroundColor;
                actualBackgroundStyle = &backgroundStyle;
            }

            const BoxElementShapeBorderStyle* actualBorderStyle = nullptr;
            if (!m_finalStyle->borderThickness.isZero()) {
                borderStyle.borderLeftColor = m_finalStyle->leftBorderColor;
                borderStyle.borderTopColor = m_finalStyle->topBorderColor;
                borderStyle.borderRightColor = m_finalStyle->rightBorderColor;
                borderStyle.borderBottomColor = m_finalStyle->bottomBorderColor;
                borderStyle.borderThickness = m_finalStyle->borderThickness;
                borderStyle.borderInset = false;    // CSS default
                actualBorderStyle = &borderStyle;
            }

            const BoxElementShapeShadowStyle* actualShadowStyle = nullptr;
            if (m_finalStyle->shadowBlurRadius > 0.0f) {
                shadowStyle.shadowOffset = Vector2(m_finalStyle->shadowOffsetX, m_finalStyle->shadowOffsetY);
                shadowStyle.shadowColor = m_finalStyle->shadowColor;
                shadowStyle.shadowWidth = m_finalStyle->shadowSpreadRadius;
                shadowStyle.shadowBlur = m_finalStyle->shadowBlurRadius;
                shadowStyle.shadowInset = m_finalStyle->shadowInset;
                actualShadowStyle = &shadowStyle;
            }

            if (actualBackgroundStyle || actualBorderStyle || actualShadowStyle) {
                context->drawBoxElement(baseStyle, actualBackgroundStyle, actualBorderStyle, actualShadowStyle);
            }
        }
#else

		if (m_finalStyle->shadowBlurRadius > 0.0f)
		{
			context->drawBoxShadow(Rect(0, 0, actualSize()), m_finalStyle->cornerRadius, Vector2(m_finalStyle->shadowOffsetX, m_finalStyle->shadowOffsetY), m_finalStyle->shadowColor, m_finalStyle->shadowBlurRadius, m_finalStyle->shadowSpreadRadius, m_finalStyle->shadowInset);

		}
		Rect rect(0, 0, actualSize());
		rect = rect.makeDeflate(m_finalStyle->borderThickness);

		if (m_finalStyle->backgroundColor.a > 0.0f) {
			context->drawSolidRectangle(rect, m_finalStyle->backgroundColor);
			//auto tex = makeObject<Texture2D>(u"D:/Proj/LN/HC1/Assets/Windowskin/window.png");
			//auto mat = Material::create(tex);
			//context->drawBoxBackground(rect, m_finalStyle->cornerRadius, m_finalStyle->backgroundDrawMode, m_finalStyle->backgroundImageRect, m_finalStyle->backgroundColor);
			//context->drawBoxBackground(finalGlobalRect(), Thickness(16), CornerRadius(), BrushImageDrawMode::BorderFrame, Rect(64, 0, 64, 64), m_finalStyle->backgroundColor);
		}

		if (m_finalStyle->backgroundMaterial && m_finalStyle->backgroundMaterial->mainTexture()) {
			context->drawImageBox(rect, m_finalStyle->backgroundDrawMode, m_finalStyle->backgroundImageRect, m_finalStyle->backgroundImageBorder, Color::White);
		}

		if (!m_finalStyle->borderThickness.isZero()) {
			context->drawBoxBorderLine(rect, m_finalStyle->borderThickness, m_finalStyle->leftBorderColor, m_finalStyle->topBorderColor, m_finalStyle->rightBorderColor, m_finalStyle->bottomBorderColor, m_finalStyle->cornerRadius, false);
		}
#endif
	}


	//Matrix tm = combinedTransform;
	//tm.translate(m_finalStyle->borderThickness.left, m_finalStyle->borderThickness.top, 0);
	//context->setBaseTransfrom(tm);

	// TODO: setMaterial
	onRender(context);

	// onRender からの drawVisual 内で設定されることに備えて再設定（ComboBox など）
	// TODO: やっぱり transform も push pop がいいと思う
	context->setBaseTransfrom(combinedTransform);
	for (auto& d : m_finalStyle->decorators) {
		d->render(context, actualSize());
	}


	// child elements
	if (m_orderdVisualChildren) {
		for (auto& e : m_orderdVisualChildren) {
			e->render(context, combinedTransform);
		}
	}

	context->popState();	// TODO: scoped
}

void UIElement::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
        if (m_focusable) {
            focus();
            e->handled = true;
            return;
        }
	}
}

bool UIElement::onHitTest(const Point& frameClientPosition)
{
    const auto pos = frameClientPositionToLocalPosition(frameClientPosition);

    if (0 <= pos.x && pos.x < actualSize().width &&
        0 <= pos.y && pos.y < actualSize().height) {
        return true;
    }
    else {
        return false;
    }
}

void UIElement::onAddChild(UIElement* child)
{
    LN_UNREACHABLE();
}

void UIElement::onEnabledChanged()
{
    if (!isEnabled() && m_manager) {
        m_manager->clearFocus(this);
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

void UIElement::attemptAddToPrimaryElement()
{
	//if (m_objectManagementFlags.hasFlag(detail::ObjectManagementFlags::AutoAddToActiveScene)) {
		UIControl* primaryElement = m_manager->primaryElement();
		if (primaryElement) {
			primaryElement->addElement(this);
		}
	//}
}

Point UIElement::frameClientPositionToLocalPosition(const Point& frameClientPosition) const
{
    auto inv = Matrix::makeInverse(m_combinedFinalRenderTransform);
    auto pos = Vector3::transformCoord(Vector3(frameClientPosition.x, frameClientPosition.y, .0f), inv);
    return Point(pos.x, pos.y);
}

void UIElement::raiseEventInternal(UIEventArgs* e, UIEventRoutingStrategy strategy)
{
    if (LN_REQUIRE(e)) return;

    e->strategy = strategy;

    // まずは this に通知
    onRoutedEvent(e);
    if (e->handled) return;

    // routing
    if (strategy == UIEventRoutingStrategy::Bubble) {
        if (m_visualParent) {
            m_visualParent->raiseEventInternal(e, strategy);
        }
        //else {
        //    m_manager->handleRootBubbleEvent(e);
        //}
    }
}

void UIElement::invalidate(detail::UIElementDirtyFlags flags, bool toAncestor)
{
    if (flags == detail::UIElementDirtyFlags::None) return;

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

bool UIElement::isRenderVisible() const
{
    return m_finalStyle->visible == UIVisibility::Visible && m_internalVisibility == UIVisibility::Visible;
}

void UIElement::moveVisualChildToForeground(UIElement* child)
{
	if (LN_REQUIRE(m_orderdVisualChildren)) return;
	if (m_orderdVisualChildren->remove(child)) {
		m_orderdVisualChildren->add(child);
	}
}

void UIElement::handleDetachFromUITree()
{
    m_manager->handleDetachFromUITree(this);

    // call children
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->handleDetachFromUITree();
    }
}

UIElement* UIElement::findFocusedVisualChildLeaf()
{
    if (m_focusedVisualChild) {
        return m_focusedVisualChild->findFocusedVisualChildLeaf();
    }
    return this;
}

UIVisualStateManager* UIElement::getVisualStateManager()
{
    if (!m_visualStateManager) {
        m_visualStateManager = makeObject<UIVisualStateManager>(this);
    }
    return m_visualStateManager;
}

void UIElement::updateEnabledPropertyOnChildren()
{
    if (m_visualChildren) {
        bool value = isEnabled();
        for (auto& child : m_visualChildren) {
            bool oldValue = child->isEnabled();
            child->m_specialElementFlags.set(detail::UISpecialElementFlags::InternalEnabled, value);
            if (oldValue != child->isEnabled()) {
                child->updateEnabledPropertyOnChildren();
                child->onEnabledChanged();
            }
        }
    }
}

} // namespace ln

