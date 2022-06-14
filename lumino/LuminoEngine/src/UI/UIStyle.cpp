/*
    2019/7/11 時点では、グローバルな単一の Style しか用意していない。
    Style システムを構築するにあたって方針を記しておく。

    記法
    ----------
    最も基本的な記法は次の通り。
    ```
    "UIButton" : {
        "Background" : "red"
    }
    ```
    これは VisualState (Pressed など) に関わらずベースとなる設定。

    VisualState ごとに変えたい場合は次のようにする。
    ```
    "UIButton" : {
        "Background" : "red"
    }
    "UIButton:Pressed" : {
        "Background" : "green"
    }
    "UIButton:Hover" : {
        "Background" : "blue"
    }
    ```

    継承したい場合は次のようにする。
    ```
    "UIToggleButton < UITButton" : {
    }
    "UIToggleButton::Hover < UITButton::Pressed" : {    // VisualState 単位でも可能
    }
    // TODO: less に似た記法でもいいか？
    "UIToggleButton" : {
        "@extend" : "UITButton"
    }
    ```

    サブコントロール。Slider の Up/Down button など。コード上で明示的に指定した特殊なコントロール。（Qt 参考 https://doc.qt.io/archives/qt-4.8/stylesheet-reference.htmls）
    ```
    "UITrack::DecreaseButton" : {
    }
    "UITrack::DecreaseButton:Pressed" : {
    }
    ```

    グローバル設定
    ```
    * {
    }
    ```

    RenderElement.
    描画のみ担当する簡易的な UIElement の代用品。 
    ```
    UIButton {
        "RenderElements" : [
            {
                "Type" : "Image",
                "Image" : "icon.png",
                "UIHAlignment" : "Left"
            }
        ]
    }
    ```

    #(ID) や .(class) は未サポート。直近では予定なし。


    
*/
#include "Internal.hpp"
#include <LuminoFont/Font.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIColors.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include "../../Font/src/FontManager.hpp"
#include "UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UIStyleDecorator

UIStyleDecorator::UIStyleDecorator()
{
}

void UIStyleDecorator::init()
{
	Object::init();
}

void UIStyleDecorator::setIconName(const StringView& value, int size)
{
	m_font = detail::FontManager::instance()->glyphIconFontManager()->getFontAwesomeFont(_TT("Reguler"), size);
	m_codePoint = detail::FontManager::instance()->glyphIconFontManager()->getFontAwesomeCodePoint(value);
}

void UIStyleDecorator::render(UIRenderingContext* context, const Size& slotRect)
{
	if (m_font && m_codePoint > 0) {
		Size areaSize;
		areaSize.width = slotRect.width - (m_margin.left + m_margin.right);
		areaSize.height = slotRect.height - (m_margin.top + m_margin.bottom);

		Size desiredSize = context->measureTextSize(m_font, m_codePoint);
		//desiredSize.width = Math::isNaN(m_width) ? 0.0f : m_width;
		//desiredSize.height = Math::isNaN(m_height) ? 0.0f : m_height;

		Rect localRect;
		detail::LayoutHelper::adjustHorizontalAlignment(areaSize, desiredSize, m_width, m_hAlignment, &localRect);
		detail::LayoutHelper::adjustVerticalAlignment(areaSize, desiredSize, m_height, m_vAlignment, &localRect);

		Matrix transform = Matrix::makeTranslation(localRect.x, localRect.y, 0);

		context->setTransfrom(transform);
		context->drawChar(m_codePoint, m_color, m_font);
	}
}

//==============================================================================
// UIStyle

// layout
const float UIStyle::DefaultWidth = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultHeight = std::numeric_limits<float>::quiet_NaN();
const Thickness UIStyle::DefaultMargin = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
const Thickness UIStyle::DefaultPadding = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
const UIHAlignment UIStyle::DefaultHorizontalAlignment = UIHAlignment::Stretch;	// WPF FrameworkElement default
const UIVAlignment UIStyle::DefaultVerticalAlignment = UIVAlignment::Stretch;
const UIHAlignment UIStyle::DefaultHorizontalContentAlignment = UIHAlignment::Stretch;	// デフォルト Stretch. WPF は Left だが、ListItem が Fill されずにカスタム Item 作るときに毎回混乱していたので。
const UIVAlignment UIStyle::DefaultVerticalContentAlignment = UIVAlignment::Stretch;
const float UIStyle::DefaultMinWidth = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultMinHeight = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultMaxWidth = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultMaxHeight = std::numeric_limits<float>::quiet_NaN();
const UIOverflowBehavior UIStyle::DefaultOverflowX = UIOverflowBehavior::Visible;
const UIOverflowBehavior UIStyle::DefaultOverflowY = UIOverflowBehavior::Visible;

// transform
const Vector3 UIStyle::DefaultPosition = Vector3(0.0f, 0.0f, 0.0f);
const Quaternion UIStyle::DefaultRotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f); // Identity
const Vector3 UIStyle::DefaultScale = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 UIStyle::DefaultCenterPoint = Vector3(0.0f, 0.0f, 0.0f);

// background
const Sprite9DrawMode UIStyle::DefaultBackgroundDrawMode = Sprite9DrawMode::StretchedSingleImage;
const Color UIStyle::DefaultBackgroundColor = Color::Transparency;
const Ref<Texture> UIStyle::DefaultBackgroundImage = nullptr;
const Ref<Shader> UIStyle::DefaultBackgroundShader = nullptr;
const Rect UIStyle::DefaultBackgroundImageRect = Rect::Zero;
const Thickness UIStyle::DefaultBackgroundImageBorder = Thickness::Zero;

// border
const Thickness UIStyle::DefaultBorderThickness = Thickness::Zero;
const CornerRadius UIStyle::DefaultCornerRadius = CornerRadius(0, 0, 0, 0);
const Color UIStyle::DefaultLeftBorderColor = Color::Black;
const Color UIStyle::DefaultTopBorderColor = Color::Black;
const Color UIStyle::DefaultRightBorderColor = Color::Black;
const Color UIStyle::DefaultBottomBorderColor = Color::Black;
const BorderDirection UIStyle::DefaultBorderDirection = BorderDirection::Outside;
const bool UIStyle::DefaultBorderInset = false;	// Web:false, WPF:true. Web に合わせてみる.

// shadow
const float UIStyle::DefaultShadowOffsetX = 0.0f;
const float UIStyle::DefaultShadowOffsetY = 0.0f;
const float UIStyle::DefaultShadowBlurRadius = 0.0f;
const float UIStyle::DefaultShadowSpreadRadius = 0.0f;
const Color UIStyle::DefaultShadowColor = Color(0.0f, 0.0f, 0.0f, 0.25f);
const bool UIStyle::DefaultShadowInset = false;

// text
const Color UIStyle::DefaultTextColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
const String UIStyle::DefaultFontFamily = String::Empty;
const float UIStyle::DefaultFontSize = 12.0f;
const UIFontWeight UIStyle::DefaultFontWeight = UIFontWeight::Normal;
const UIFontStyle UIStyle::DefaultFontStyle = UIFontStyle::Normal;

// effects
const UIVisibility UIStyle::DefaultVisible = UIVisibility::Visible;
const BlendMode UIStyle::DefaultBlendMode = BlendMode::Alpha;
const float UIStyle::DefaultOpacity = 1.0f;
const Color UIStyle::DefaultColorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color UIStyle::DefaultBlendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
const ColorTone UIStyle::DefaultTone = ColorTone(0.0f, 0.0f, 0.0f, 0.0f);

UIStyle::UIStyle()
{
}

UIStyle::~UIStyle()
{
}

void UIStyle::init()
{
    Object::init();
}
void UIStyle::setWidthAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->width = makeObject<UIScalarAnimationTrack>(startValue, targetValue, duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_Width, true);
}

void UIStyle::setHeightAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->height = makeObject<UIScalarAnimationTrack>(startValue, targetValue, duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_Height, true);
}

void UIStyle::setPositionXAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->positionX = makeObject<UIScalarAnimationTrack>(startValue, targetValue, duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_PositionX, true);
}

void UIStyle::setPositionYAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->positionY = makeObject<UIScalarAnimationTrack>(startValue, targetValue, duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_PositionY, true);
}

void UIStyle::setPositionZAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->positionZ = makeObject<UIScalarAnimationTrack>(startValue, targetValue, duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_PositionZ, true);
}

void UIStyle::setBackgroundColorAnimation(const Color& startValue, const Color& targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->backgroundColor = makeObject<UIVector4AnimationTrack>(startValue.toVector4(), targetValue.toVector4(), duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_BackgroundColor, true);
}

void UIStyle::setOpacityAnimation(float startValue, float targetValue, float duration, EasingMode timingFunction, float delay, AnimationWrapMode wrapMode)
{
	auto* d = acquireAnimationData();
	d->opacity = makeObject<UIScalarAnimationTrack>(startValue, targetValue, duration, timingFunction, delay, wrapMode);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_Opacity, true);
}


void UIStyle::setWidthTransition(float target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->width = makeObject<UIScalarTransitionTrack>(target, duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_Width, true);
}

void UIStyle::setHeightTransition(float target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->height = makeObject<UIScalarTransitionTrack>(target, duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_Height, true);
}

void UIStyle::setPositionXTransition(float target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->positionX = makeObject<UIScalarTransitionTrack>(target, duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_PositionX, true);
}

void UIStyle::setPositionYTransition(float target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->positionY = makeObject<UIScalarTransitionTrack>(target, duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_PositionY, true);
}

void UIStyle::setPositionZTransition(float target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->positionZ = makeObject<UIScalarTransitionTrack>(target, duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_PositionZ, true);
}

void UIStyle::setBackgroundColorTransition(const Color& target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->backgroundColor = makeObject<UIVector4TransitionTrack>(target.toVector4(), duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_BackgroundColor, true);
}

void UIStyle::setOpacityTransition(float target, float duration, EasingMode timingFunction, float delay)
{
	auto* d = acquireAnimationData();
	d->opacity = makeObject<UIScalarTransitionTrack>(target, duration, timingFunction, delay);
	d->setLocalValueFlag(detail::UIStyleAnimationElement_Opacity, true);
}

void UIStyle::setupDefault()
{
    // layout
    width = DefaultWidth;
    height = DefaultHeight;
    marginTop = Math::NaN;
    marginRight = Math::NaN;
    marginBottom = Math::NaN;
    marginLeft = Math::NaN;
    borderTop = 0.0f;//Math::NaN;
    borderRight = 0.0f; //Math::NaN;
    borderBottom = 0.0f; //Math::NaN;
    borderLeft = 0.0f;   //Math::NaN;
    paddingTop = 0.0f;   //Math::NaN;
    paddingRight = 0.0f; //Math::NaN;
    paddingBottom = 0.0f; //Math::NaN;
    paddingLeft = 0.0f;   //Math::NaN;
	//margin = DefaultMargin;
	//padding = DefaultPadding;
	//hAlignment = DefaultHorizontalAlignment;
	//vAlignment = DefaultVerticalAlignment;
    horizontalContentAlignment = DefaultHorizontalContentAlignment;
    verticalContentAlignment = DefaultVerticalContentAlignment;
	minWidth = DefaultMinWidth;
	minHeight = DefaultMinHeight;
	maxWidth = DefaultMaxWidth;
	maxHeight = DefaultMaxHeight;
	overflowX = DefaultOverflowX;
	overflowY = DefaultOverflowY;

    // transform
	position = DefaultPosition;
	rotation = DefaultRotation;
	scale = DefaultScale;
	centerPoint = DefaultCenterPoint;

    // background
    backgroundDrawMode = DefaultBackgroundDrawMode;
    backgroundColor = DefaultBackgroundColor;
    backgroundImage = DefaultBackgroundImage;
    backgroundShader = DefaultBackgroundShader;
    backgroundImageRect = DefaultBackgroundImageRect;
    backgroundImageBorder = DefaultBackgroundImageBorder;

    // border
    borderThickness = DefaultBorderThickness;
    cornerRadius = DefaultCornerRadius;
    leftBorderColor = DefaultLeftBorderColor;
    topBorderColor = DefaultTopBorderColor;
    rightBorderColor = DefaultRightBorderColor;
    bottomBorderColor = DefaultBottomBorderColor;
    borderDirection = DefaultBorderDirection;
	borderInset = DefaultBorderInset;

	// shadow
	shadowOffsetX = DefaultShadowOffsetX;
	shadowOffsetY = DefaultShadowOffsetY;
	shadowBlurRadius = DefaultShadowBlurRadius;
	shadowSpreadRadius = DefaultShadowSpreadRadius;
	shadowColor = DefaultShadowColor;
	shadowInset = DefaultShadowInset;

    // text
	textColor = DefaultTextColor;
	fontFamily = DefaultFontFamily;
	fontSize = DefaultFontSize;
	fontWeight = DefaultFontWeight;
	fontStyle = DefaultFontStyle;

    // render effects
	visible = DefaultVisible;
	blendMode = DefaultBlendMode;
	opacity = DefaultOpacity;
	colorScale = DefaultColorScale;
	blendColor = DefaultBlendColor;
	tone = DefaultTone;

	// decorators
	decorators.clear();

	// animations
	m_animationData = nullptr;
}

void UIStyle::reset()
{
	// layout
    width.reset();
    height.reset();
    marginTop.reset();
    marginRight.reset();
    marginBottom.reset();
    marginLeft.reset();
    borderTop.reset();
    borderRight.reset();
    borderBottom.reset();
    borderLeft.reset();
    paddingTop.reset();
    paddingRight.reset();
    paddingBottom.reset();
    paddingLeft.reset();
	//hAlignment.reset();
	//vAlignment.reset();
	horizontalContentAlignment.reset();
	verticalContentAlignment.reset();
	minWidth.reset();
	minHeight.reset();
	maxWidth.reset();
	maxHeight.reset();
	overflowX.reset();
	overflowY.reset();

	// layout transform
	position.reset();
	rotation.reset();
	scale.reset();
	centerPoint.reset();

	// background
	backgroundDrawMode.reset();
	backgroundColor.reset();
	backgroundImage.reset();
	backgroundShader.reset();
	backgroundImageRect.reset();
	backgroundImageBorder.reset();

	// border
	borderThickness.reset();
	cornerRadius.reset();
	leftBorderColor.reset();
	topBorderColor.reset();
	rightBorderColor.reset();
	bottomBorderColor.reset();
	borderDirection.reset();
	borderInset.reset();

	// shadow
	shadowOffsetX.reset();
	shadowOffsetY.reset();
	shadowBlurRadius.reset();
	shadowSpreadRadius.reset();
	shadowColor.reset();
	shadowInset.reset();

	// text
	textColor.reset();
	fontFamily.reset();
	fontSize.reset();
	fontWeight.reset();
	fontStyle.reset();

	// render effects
	visible.reset();
	blendMode.reset();
	opacity.reset();
	colorScale.reset();
	blendColor.reset();
	tone.reset();

	// decorators
	decorators.clear();

	// animations
	m_animationData = nullptr;	// TODO: 捨てるとメモリ効率悪い
}

void UIStyle::mergeFrom(const UIStyle* other)
{
    if (LN_REQUIRE(other)) return;

    // layout
    if (other->width.hasValue()) width = other->width.get();
    if (other->height.hasValue()) height = other->height.get();
    if (other->marginTop.hasValue()) marginTop = other->marginTop.value();
    if (other->marginRight.hasValue()) marginRight = other->marginRight.value();
    if (other->marginBottom.hasValue()) marginBottom = other->marginBottom.value();
    if (other->marginLeft.hasValue()) marginLeft = other->marginLeft.value();
    if (other->borderTop.hasValue()) borderTop = other->borderTop.value();
    if (other->borderRight.hasValue()) borderRight = other->borderRight.value();
    if (other->borderBottom.hasValue()) borderBottom = other->borderBottom.value();
    if (other->borderLeft.hasValue()) borderLeft = other->borderLeft.value();
    if (other->paddingTop.hasValue()) paddingTop = other->paddingTop.value();
    if (other->paddingRight.hasValue()) paddingRight = other->paddingRight.value();
    if (other->paddingBottom.hasValue()) paddingBottom = other->paddingBottom.value();
    if (other->paddingLeft.hasValue()) paddingLeft = other->paddingLeft.value();
    //if (other->hAlignment.hasValue()) hAlignment = other->hAlignment.get();
    //if (other->vAlignment.hasValue()) vAlignment = other->vAlignment.get();
    if (other->horizontalContentAlignment.hasValue()) horizontalContentAlignment = other->horizontalContentAlignment.get();
    if (other->verticalContentAlignment.hasValue()) verticalContentAlignment = other->verticalContentAlignment.get();
    if (other->minWidth.hasValue()) minWidth = other->minWidth.get();
    if (other->minHeight.hasValue()) minHeight = other->minHeight.get();
    if (other->maxWidth.hasValue()) maxWidth = other->maxWidth.get();
    if (other->maxHeight.hasValue()) maxHeight = other->maxHeight.get();
	if (other->overflowX.hasValue()) overflowX = other->overflowX.get();
	if (other->overflowY.hasValue()) overflowY = other->overflowY.get();

    // layout transform
    if (other->position.hasValue()) position = other->position.get();
    if (other->rotation.hasValue()) rotation = other->rotation.get();
    if (other->scale.hasValue()) scale = other->scale.get();
    if (other->centerPoint.hasValue()) centerPoint = other->centerPoint.get();

    // background
    if (other->backgroundDrawMode.hasValue()) backgroundDrawMode = other->backgroundDrawMode.get();
    if (other->backgroundColor.hasValue()) backgroundColor = other->backgroundColor.get();
    if (other->backgroundImage.hasValue()) backgroundImage = other->backgroundImage.get();
    if (other->backgroundShader.hasValue()) backgroundShader = other->backgroundShader.get();
    if (other->backgroundImageRect.hasValue()) backgroundImageRect = other->backgroundImageRect.get();
    if (other->backgroundImageBorder.hasValue()) backgroundImageBorder = other->backgroundImageBorder.get();

    // border
    if (other->borderThickness.hasValue()) borderThickness = other->borderThickness.get();
    if (other->cornerRadius.hasValue()) cornerRadius = other->cornerRadius.get();
    if (other->leftBorderColor.hasValue()) leftBorderColor = other->leftBorderColor.get();
    if (other->topBorderColor.hasValue()) topBorderColor = other->topBorderColor.get();
    if (other->rightBorderColor.hasValue()) rightBorderColor = other->rightBorderColor.get();
    if (other->bottomBorderColor.hasValue()) bottomBorderColor = other->bottomBorderColor.get();
    if (other->borderDirection.hasValue()) borderDirection = other->borderDirection.get();
	if (other->borderInset.hasValue()) borderInset = other->borderInset.get();

	// shadow
	if (other->shadowOffsetX.hasValue()) shadowOffsetX = other->shadowOffsetX.get();
	if (other->shadowOffsetY.hasValue()) shadowOffsetY = other->shadowOffsetY.get();
	if (other->shadowBlurRadius.hasValue()) shadowBlurRadius = other->shadowBlurRadius.get();
	if (other->shadowSpreadRadius.hasValue()) shadowSpreadRadius = other->shadowSpreadRadius.get();
	if (other->shadowColor.hasValue()) shadowColor = other->shadowColor.get();
	if (other->shadowInset.hasValue()) shadowInset = other->shadowInset.get();

    // text
    if (other->textColor.hasValue()) textColor = other->textColor.get();
    if (other->fontFamily.hasValue()) fontFamily = other->fontFamily.get();
    if (other->fontSize.hasValue()) fontSize = other->fontSize.get();
    if (other->fontWeight.hasValue()) fontWeight = other->fontWeight.get();
    if (other->fontStyle.hasValue()) fontStyle = other->fontStyle.get();

    // render effects
    if (other->visible.hasValue()) visible = other->visible.get();
    if (other->blendMode.hasValue()) blendMode = other->blendMode.get();
    if (other->opacity.hasValue()) opacity = other->opacity.get();
    if (other->colorScale.hasValue()) colorScale = other->colorScale.get();
    if (other->blendColor.hasValue()) blendColor = other->blendColor.get();
    if (other->tone.hasValue()) tone = other->tone.get();

	// decorators
	for (auto& d : other->decorators) {
		decorators.add(d);
	}

	// animations
	if (other->m_animationData) {
		AnimationData* d = acquireAnimationData();

#define LN_MERGE_ANIMATION_DATA(field, flagIndex) \
		if (!d->hasLocalValue(flagIndex) && other->m_animationData->field) { \
			d->field = other->m_animationData->field; \
			d->setInheritFlag(flagIndex, true); \
		}

		LN_MERGE_ANIMATION_DATA(width, detail::UIStyleAnimationElement_Width);
		LN_MERGE_ANIMATION_DATA(height, detail::UIStyleAnimationElement_Height);
		LN_MERGE_ANIMATION_DATA(positionX, detail::UIStyleAnimationElement_PositionX);
		LN_MERGE_ANIMATION_DATA(positionY, detail::UIStyleAnimationElement_PositionY);
		LN_MERGE_ANIMATION_DATA(positionZ, detail::UIStyleAnimationElement_PositionZ);
		LN_MERGE_ANIMATION_DATA(backgroundColor, detail::UIStyleAnimationElement_BackgroundColor);
		LN_MERGE_ANIMATION_DATA(opacity, detail::UIStyleAnimationElement_Opacity);
#undef LN_MERGE_ANIMATION_DATA
	}
}

void UIStyle::copyFrom(const UIStyle* other)
{
    if (LN_REQUIRE(other)) return;

    // layout
    width = other->width;
    height = other->height;
    marginTop = other->marginTop;
    marginRight = other->marginRight;
    marginBottom = other->marginBottom;
    marginLeft = other->marginLeft;
    borderTop = other->borderTop;
    borderRight = other->borderRight;
    borderBottom = other->borderBottom;
    borderLeft = other->borderLeft;
    paddingTop = other->paddingTop;
    paddingRight = other->paddingRight;
    paddingBottom = other->paddingBottom;
    paddingLeft = other->paddingLeft;
    //hAlignment = other->hAlignment;
    //vAlignment = other->vAlignment;
    horizontalContentAlignment = other->horizontalContentAlignment;
    verticalContentAlignment = other->verticalContentAlignment;
    minWidth = other->minWidth;
    minHeight = other->minHeight;
    maxWidth = other->maxWidth;
    maxHeight = other->maxHeight;
	overflowX = other->overflowX;
	overflowY = other->overflowY;

    // layout transform
    position = other->position;
    rotation = other->rotation;
    scale = other->scale;
    centerPoint = other->centerPoint;

    // background
    backgroundDrawMode = other->backgroundDrawMode;
    backgroundColor = other->backgroundColor;
    backgroundImage = other->backgroundImage;
    backgroundShader = other->backgroundShader;
    backgroundImageRect = other->backgroundImageRect;
    backgroundImageBorder = other->backgroundImageBorder;

    // border
    borderThickness = other->borderThickness;
    cornerRadius = other->cornerRadius;
    leftBorderColor = other->leftBorderColor;
    topBorderColor = other->topBorderColor;
    rightBorderColor = other->rightBorderColor;
    bottomBorderColor = other->bottomBorderColor;
    borderDirection = other->borderDirection;
	borderInset = other->borderInset;

	// border
	shadowOffsetX = other->shadowOffsetX;
	shadowOffsetY = other->shadowOffsetY;
	shadowBlurRadius = other->shadowBlurRadius;
	shadowSpreadRadius = other->shadowSpreadRadius;
	shadowColor = other->shadowColor;
	shadowInset = other->shadowInset;

    // text
    textColor = other->textColor;
    fontFamily = other->fontFamily;
    fontSize = other->fontSize;
    fontWeight = other->fontWeight;
    fontStyle = other->fontStyle;

    // render effects
    visible = other->visible;
    blendMode = other->blendMode;
    opacity = other->opacity;
    colorScale = other->colorScale;
    blendColor = other->blendColor;
    tone = other->tone;

	// decorators
	for (auto& d : other->decorators) {
		decorators.add(d);
	}

	// animations
	if (other->m_animationData) {
		AnimationData* d = acquireAnimationData();
		d->hasLocalValueFlags = other->m_animationData->hasLocalValueFlags;
		d->inheritFlags = other->m_animationData->inheritFlags;
		d->width = other->m_animationData->width;
		d->height = other->m_animationData->height;
		d->positionX = other->m_animationData->positionX;
		d->positionY = other->m_animationData->positionY;
		d->positionZ = other->m_animationData->positionZ;
		d->backgroundColor = other->m_animationData->backgroundColor;
		d->opacity = other->m_animationData->opacity;
	}
}

UIStyle::AnimationData* UIStyle::acquireAnimationData()
{
	if (!m_animationData) {
		m_animationData = std::make_unique<AnimationData>();
	}
	return m_animationData.get();
}

//==============================================================================
// UIStyleSet

UIStyleSet::UIStyleSet()
{
}

UIStyleSet::~UIStyleSet()
{
}

void UIStyleSet::init(const StringView& elementName)
{
    Object::init();
	m_elementName = elementName;
	m_mainStyleClass = makeObject<UIStyleClass>(_TT(""));
}

void UIStyleSet::addStyleClass(UIStyleClass* styleClass)
{
    if (LN_REQUIRE(styleClass)) return;
	if (LN_REQUIRE(!styleClass->name().isEmpty())) return;
	m_styleClasses.add(styleClass);
}

UIStyleClass* UIStyleSet::addStyleClass(const StringView& className)
{
	auto ptr = makeObject<UIStyleClass>(className);
	addStyleClass(ptr);
	return ptr;
}

UIStyleClass* UIStyleSet::findStyleClass(const StringView& className) const
{
    auto style = m_styleClasses.findIf([&](auto& x) { return String::compare(x->name(), className, CaseSensitivity::CaseInsensitive) == 0; });
    if (style)
        return *style;
    else
        return nullptr;
}

void UIStyleSet::copyFrom(const UIStyleSet* other)
{
	if (LN_REQUIRE(other)) return;

	m_elementName = other->m_elementName;

	m_mainStyleClass->copyFrom(other->m_mainStyleClass);

	m_styleClasses.clear();
	for (auto& otherClass : other->m_styleClasses) {
		auto newClass = makeObject<UIStyleClass>(otherClass->name());
		newClass->copyFrom(otherClass);
		m_styleClasses.add(newClass);
	}
}

void UIStyleSet::mergeFrom(const UIStyleSet* other)
{
    if (LN_REQUIRE(other)) return;

	m_mainStyleClass->mergeFrom(other->mainStyleClass());

    // Merge sub-element style
    for (auto& styleClass : other->m_styleClasses) {
        auto style = findStyleClass(styleClass->name());
        if (!style) {
			// Create with same name
			m_styleClasses.add(makeObject<UIStyleClass>(styleClass->name()));
            style = m_styleClasses.back();
        }
        style->mergeFrom(styleClass);
    }
}

//==============================================================================
// UIStyleClass

UIStyleClass::UIStyleClass()
{
}

UIStyleClass::~UIStyleClass()
{
}

void UIStyleClass::init(const StringView& name)
{
	Object::init();
	m_name = name;
	m_mainStyle = makeObject<UIStyle>();
}

void UIStyleClass::addStateStyle(const StringView& stateName, UIStyle* style)
{
	if (LN_REQUIRE(style)) return;
	m_visualStateStyles.add({ stateName, style });
}

UIStyle* UIStyleClass::addStateStyle(const StringView& stateName)
{
	auto ptr = makeObject<UIStyle>();
	addStateStyle(stateName, ptr);
	return ptr;
}

UIStyle* UIStyleClass::findStateStyle(const StringView& stateName) const
{
	auto style = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, stateName, CaseSensitivity::CaseInsensitive) == 0; });
	if (style)
		return style->style;
	else
		return nullptr;
}

void UIStyleClass::copyFrom(const UIStyleClass* other)
{
	if (LN_REQUIRE(other)) return;

	m_mainStyle->copyFrom(other->m_mainStyle);

	m_visualStateStyles.clear();
	for (auto& otherSlot : other->m_visualStateStyles) {
		auto newStyle = makeObject<UIStyle>();
		newStyle->copyFrom(otherSlot.style);
		m_visualStateStyles.add({ otherSlot.name, newStyle });
	}
}

void UIStyleClass::mergeFrom(const UIStyleClass* other)
{
	if (LN_REQUIRE(other)) return;

	// Merge main style
	m_mainStyle->mergeFrom(other->m_mainStyle);

	// Merge visual-state style
	for (auto& slot : other->m_visualStateStyles) {
		auto style = findStateStyle(slot.name);
		if (!style) {
			m_visualStateStyles.add({ slot.name, makeObject<UIStyle>() });
			style = m_visualStateStyles.back().style;
		}
		style->mergeFrom(slot.style);
	}
}

//==============================================================================
// UIStyleSheet

UIStyleSheet::UIStyleSheet()
{
}

UIStyleSheet::~UIStyleSheet()
{
}

void UIStyleSheet::init()
{
    Object::init();
}

void UIStyleSheet::addStyleSet(const StringView& elementName, UIStyleSet* styleClass)
{
    if (LN_REQUIRE(styleClass)) return;
    m_classes.insert({ elementName, styleClass });
}

Ref<UIStyleSet> UIStyleSheet::addStyleSet(const StringView& elementName)
{
    auto cls = makeObject<UIStyleSet>(elementName);
    addStyleSet(elementName, cls);
    return cls;
}

UIStyleSet* UIStyleSheet::findStyleSet(const StringView& elementName) const
{
    auto itr = m_classes.find(elementName);
    if (itr != m_classes.end())
        return itr->second;
    else
        return nullptr;
}

UIStyle* UIStyleSheet::obtainStyle(const StringView& selector)
{
	String actualSelector = selector;
	int classBegin = actualSelector.lastIndexOf(_TT("."));
	int stateBegin = actualSelector.lastIndexOf(_TT(":"));

	int elementEnd = (classBegin > 0) ? classBegin : ((stateBegin > 0) ? stateBegin : actualSelector.length());
	int classEnd = (stateBegin > 0) ? stateBegin : actualSelector.length();
	int stateEnd = actualSelector.length();

	StringView elementName;
	StringView className;
	StringView stateName;
	if (classBegin == 0 || stateBegin == 0) {
		// omitted
	}
	else {
		elementName = actualSelector.substr(0, elementEnd);
	}
	if (classBegin >= 0) {
		className = actualSelector.substr(classBegin + 1, classEnd - classBegin - 1);
	}
	if (stateBegin >= 0) {
		stateName = actualSelector.substr(stateBegin + 1, stateEnd - stateBegin - 1);
	}

	UIStyleSet* styleSet;
	if (elementName.isEmpty()) {
		LN_NOTIMPLEMENTED();	// TODO: global
		return nullptr;
	}
	else {
		styleSet = findStyleSet(elementName);
		if (!styleSet) {
			styleSet = addStyleSet(elementName);
		}
	}

	UIStyleClass* styleClass;
	if (className.isEmpty()) {
		styleClass = styleSet->mainStyleClass();
	}
	else {
		styleClass = styleSet->findStyleClass(className);
		if (!styleClass) {
			styleClass = styleSet->addStyleClass(className);
		}
	}

	UIStyle* style;
	if (stateName.isEmpty()) {
		style = styleClass->mainStyle();
	}
	else {
		style = styleClass->findStateStyle(stateName);
		if (!style) {
			style = styleClass->addStateStyle(stateName);
		}
	}

	assert(style);
	return style;
}

//==============================================================================
// UIStyleContext

UIStyleContext::UIStyleContext()
{
}

UIStyleContext::~UIStyleContext()
{
}

void UIStyleContext::init()
{
    Object::init();
}

void UIStyleContext::addStyleSheet(UIStyleSheet* sheet)
{
    if (LN_REQUIRE(sheet)) return;
    m_styleSheets.add(sheet);
    build();
}

void UIStyleContext::build()
{
    // first, merge globals
    m_globalStyle = makeObject<UIStyleSet>(_TT(""));
	m_globalStyle->mainStyleClass()->mainStyle()->setupDefault();
    for (auto& sheet : m_styleSheets) {
        auto globalStyle = sheet->findStyleSet(_TT(""));
        if (globalStyle) {
            m_globalStyle->mergeFrom(globalStyle);
        }
    }
    //m_resolvedGlobalStyle = makeRef<detail::UIStyleClassInstance>();
    //m_resolvedGlobalStyle->mergeFrom(m_globalStyle);

    // second, merge other styles
    for (auto& sheet : m_styleSheets) {
        for (auto& pair : sheet->m_classes) {
            auto itr = m_elementStyles.find(pair.first);
            if (itr == m_elementStyles.end()) {
                auto newSet = makeObject<UIStyleSet>(pair.first);
				newSet->copyFrom(m_globalStyle);
				newSet->copyFrom(pair.second);
				m_elementStyles.insert({ newSet->elementName(), newSet });
            }
            else {
                itr->second->mergeFrom(pair.second);
            }
        }
    }
}

UIStyleSet* UIStyleContext::findStyleSet(const StringView& elementName) const
{
    auto itr = m_elementStyles.find(elementName);
    if (itr != m_elementStyles.end())
        return itr->second;
    else
        return nullptr;
}

//detail::UIStyleClassInstance* UIStyleContext::findResolvedStyleClass(const StringView& elementName) const
//{
//    auto itr = m_resolvedClasses.find(elementName);
//    if (itr != m_resolvedClasses.end())
//        return itr->second;
//    else
//        return m_resolvedGlobalStyle;
//}

void UIStyleContext::combineStyle(UIStyle* style, const StringView& elementName, const List<String>* classList) const
{
	auto set = findStyleSet(elementName);
    if (!set) return;   // global は parent に乗って降りてくるので、個々の element では結合不要
#if 1
	style->mergeFrom(set->mainStyleClass()->mainStyle());

	if (classList) {
		for (auto& name : *classList) {
			auto cs = set->findStyleClass(name);
			if (cs) {
				style->mergeFrom(cs->mainStyle());
			}
		}
	}
#else
	bool classFound = false;
	if (classList) {
		for (auto& name : *classList) {
			auto cs = set->findStyleClass(name);
			if (cs) {
				style->mergeFrom(cs->mainStyle());
				classFound = true;
			}
		}
	}

	// class 一致が無ければ elementName のみで識別されるスタイルを使う
	if (!classFound) {
		style->mergeFrom(set->mainStyleClass()->mainStyle());
	}
#endif
}

//==============================================================================
// UIVisualStateManager

UIVisualStateManager::UIVisualStateManager()
    : m_ownerElement(nullptr)
    , m_groups()
    , m_combinedStyle(makeObject<UIStyle>())
    , m_resolvedStyle(makeRef<detail::UIStyleInstance>())
    , m_dirty(true)
{
}

void UIVisualStateManager::init(UIElement* ownerElement)
{
    Object::init();
    m_ownerElement = ownerElement;
}

//UIStyle* UIVisualStateManager::combineStyle(const UIStyleContext* styleContext, const ln::String& elementName)
//{
//    if (isDirty()) {
//        m_combinedStyle->setupDefault();
//        auto styleClass = styleContext->findStyleClass(elementName);
//
//        // main style
//        m_combinedStyle->mergeFrom(styleClass->style());
//
//        for (auto& group : m_groups) {
//            if (group.activeStateIndex >= 0) {
//                m_combinedStyle->mergeFrom(styleClass->findStateStyle(group.stateNames[group.activeStateIndex]));
//            }
//        }
//
//        clearDirty();
//    }
//    return m_combinedStyle;
//}
//
void UIVisualStateManager::combineStyle(UIStyle* style, const UIStyleContext* styleContext, const ln::String& elementName, const List<String>* classList) const
{
	// TODO: これキャッシュできると思う
	auto set = styleContext->findStyleSet(elementName);
	if (!set) return;

#if 1
	auto styleClass = set->mainStyleClass();
	if (styleClass) {
        combineStyle(style, styleClass);
		//for (auto& group : m_groups) {
		//	if (group.activeStateIndex >= 0) {
		//		if (auto stateStyle = styleClass->findStateStyle(group.stateNames[group.activeStateIndex])) {
		//			style->mergeFrom(stateStyle);
		//		}
		//	}
		//}
	}

	if (classList) {
		for (auto& name : *classList) {
			auto cs = set->findStyleClass(name);
			if (cs) {
				for (const auto& slot : cs->visualStateStyles()) {
					auto r = m_groups.findIf([&slot](const auto& x) {
						return x.activeStateIndex >= 0 && String::compare(x.stateNames[x.activeStateIndex], slot.name, CaseSensitivity::CaseInsensitive) == 0; });
					if (r) {
						style->mergeFrom(slot.style);
					}
				}


				//for (auto& group : m_groups) {
				//	if (group.activeStateIndex >= 0) {
				//		if (auto stateStyle = cs->findStateStyle(group.stateNames[group.activeStateIndex])) {
				//			style->mergeFrom(stateStyle);
				//		}
				//	}
				//}
			}
		}
	}

#else
	bool classFound = false;
	if (classList) {
		for (auto& name : *classList) {
			auto cs = set->findStyleClass(name);
			if (cs) {
				classFound = true;
				for (auto& group : m_groups) {
					if (group.activeStateIndex >= 0) {
						if (auto stateStyle = cs->findStateStyle(group.stateNames[group.activeStateIndex])) {
							style->mergeFrom(stateStyle);
						}
					}
				}
			}
		}
	}

	// class 一致が無ければ elementName のみで識別されるスタイルを使う
	if (!classFound) {
		auto styleClass = set->mainStyleClass();
		if (styleClass) {
			for (auto& group : m_groups) {
				if (group.activeStateIndex >= 0) {
					if (auto stateStyle = styleClass->findStateStyle(group.stateNames[group.activeStateIndex])) {
						style->mergeFrom(stateStyle);
					}
				}
			}
		}
	}
#endif
}

void UIVisualStateManager::combineStyle(UIStyle* style, const UIStyleClass* styleClass) const
{
    style->mergeFrom(styleClass->mainStyle());

	for (const auto& slot : styleClass->visualStateStyles()) {
		auto r = m_groups.findIf([&slot](const auto& x) {
			return x.activeStateIndex >= 0 && String::compare(x.stateNames[x.activeStateIndex], slot.name, CaseSensitivity::CaseInsensitive) == 0; });
		if (r) {
			style->mergeFrom(slot.style);
		}
	}
    //for (auto& group : m_groups) {
    //    if (group.activeStateIndex >= 0) {
    //        if (auto stateStyle = styleClass->findStateStyle(group.stateNames[group.activeStateIndex])) {
    //            style->mergeFrom(stateStyle);
    //        }
    //    }
    //}
}

//detail::UIStyleInstance* UIVisualStateManager::resolveStyle(const UIStyleContext* styleContext, const ln::String& className)
//{
//    if (isDirty()) {
//        m_resolvedStyle->setupDefault();
//        auto styleClass = styleContext->findStyleClass(className);
//
//        // main style
//        m_resolvedStyle->mergeFrom(styleClass->style());
//
//        for (auto& group : m_groups) {
//            if (group.activeStateIndex >= 0) {
//                m_resolvedStyle->mergeFrom(styleClass->findStateStyle(group.stateNames[group.activeStateIndex]));
//            }
//        }
//
//        clearDirty();
//    }
//    return m_resolvedStyle;
//}


//==============================================================================
// UITheme
//      命名の基本は <目的>.<属性名>
//      button.background
//      inputValidation.errorBackground
//      例:https://code.visualstudio.com/api/references/theme-color

const Color& UITheme::get(const StringView& name) const
{
    auto itr = m_colors.find(name);
    if (itr != m_colors.end())
        return itr->second;
    else
        return Color::Transparency;
}

UITheme::UITheme()
{
}

void UITheme::add(const StringView& name, const Color& color)
{
    m_colors[name] = color;
}

void UITheme::init()
{
    Object::init();
}

// PC 用 Editor 向け Theme. Material-UI and Blender 2.8 based.
void UITheme::buildLumitelier()
{
	m_defaultStyle = makeObject<UIStyle>();
	m_defaultStyle->setupDefault();
	m_defaultStyle->textColor = Color::White;
	m_defaultStyle->fontSize = 14;

	setSpacing(4);
	m_lineContentHeight = 24;
	m_lineSpacing = m_lineContentHeight + spacing(1);

	// Background
	setColor(UIThemeConstantPalette::DefaultBackgroundColor, Color::parse(_TT("#1E1E1E")));
	setColor(UIThemeConstantPalette::ControlBackgroundColor, UIColors::get(UIColorHues::Grey, 7));// Color::parse(_TT("#303030"));
	//setColor(UIThemeConstantPalette::DefaultBackgroundColor, UIColors::get(UIColorHues::Grey, 8));
	setColor(UIThemeConstantPalette::PaperBackgroundColor, Color::parse(_TT("#424242")));

	// Intentions
	setColor(UIThemeConstantPalette::DefaultMainColor, UIColors::get(UIColorHues::Grey, 2));
	setColor(UIThemeConstantPalette::DefaultTextColor, Color::Black);
	setColor(UIThemeConstantPalette::PrimaryMainColor, UIColors::get(UIColorHues::Green, 5));
	setColor(UIThemeConstantPalette::PrimaryTextColor, Color::White);
	setColor(UIThemeConstantPalette::SecondaryMainColor, UIColors::get(UIColorHues::Orange, 5));
	setColor(UIThemeConstantPalette::SecondaryTextColor, Color::White);
	setColor(UIThemeConstantPalette::ErrorMainColor, UIColors::get(UIColorHues::Red, 5));
	setColor(UIThemeConstantPalette::ErrorTextColor, Color::White);
	setColor(UIThemeConstantPalette::WarningMainColor, UIColors::get(UIColorHues::Orange, 3));
	setColor(UIThemeConstantPalette::WarningTextColor, Color::Black);
	setColor(UIThemeConstantPalette::InfoMainColor, UIColors::get(UIColorHues::Blue, 3));
	setColor(UIThemeConstantPalette::InfoTextColor, Color::White);
	setColor(UIThemeConstantPalette::SuccessMainColor, UIColors::get(UIColorHues::Green, 3));
	setColor(UIThemeConstantPalette::SuccessTextColor, Color::White);

	// Items
	setColor(UIThemeConstantPalette::ItemHoverAction, Color::White.withAlpha(0.1));
	setColor(UIThemeConstantPalette::ItemSelectedAction, Color::White.withAlpha(0.2));

	// Divider
	setColor(UIThemeConstantPalette::DefaultDivider, Color::White.withAlpha(0.3));

	auto sheet = makeObject<UIStyleSheet>();

	//--------------------------------
	// UIButton
	{
		if (auto s = sheet->obtainStyle(_TT("UIButton"))) {
			s->minWidth = 64;
			s->minHeight = lineContentHeight();
			s->setMargin(Thickness(8));   // TODO: spacing?
			s->setPadding(Thickness(spacing(2), 0));
			//s->hAlignment = UIHAlignment::Center;
			//s->vAlignment = UIVAlignment::Center;
			s->horizontalContentAlignment = UIHAlignment::Center;
			s->verticalContentAlignment = UIVAlignment::Center;
			s->backgroundColor = UIColors::get(UIColorHues::Grey, 7);
			s->cornerRadius = CornerRadius(4);
			s->shadowBlurRadius = 1;
			s->shadowOffsetY = 1;
			s->shadowColor = Color(0, 0, 0, 0.5);
		}


		//auto e = sheet->addStyleSet(_TT("UIButton");
		//{
		//	auto s = e->mainStyleClass()->mainStyle();
		//}
		//// UIButton.test
		//{
		//	auto s = makeObject<UIStyle>();
		//	s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
		//	c->addClassStyle(_TT("test", s);
		//}
		// UIButton:MouseOver
		if (auto s = sheet->obtainStyle(_TT("UIButton:MouseOver"))) {
			s->backgroundColor = UIColors::get(UIColorHues::Grey, 5);
		}
		// 
		if (auto s = sheet->obtainStyle(_TT("UIButton:Pressed"))) {
			s->backgroundColor = color(UIThemeConstantPalette::PrimaryMainColor);
			s->shadowBlurRadius = 0;
			s->shadowOffsetY = 0;
		}
	}

	//--------------------------------
	// UICheckBox
	{
		if (auto s = sheet->obtainStyle(_TT("UICheckBox"))) {
			s->minHeight = lineContentHeight();
		}
		if (auto s = sheet->obtainStyle(_TT("UIElement.UICheckBox-CheckMark"))) {
			s->backgroundColor = color(UIThemeConstantPalette::ControlBackgroundColor);
			s->cornerRadius = CornerRadius(4);
			s->width = 14;
			s->height = 14;
			s->setMargin(4);
		}
		if (auto s = sheet->obtainStyle(_TT("UIElement.UICheckBox-CheckMark:Checked"))) {
			auto icon = makeObject<UIStyleDecorator>();
			icon->setIconName(_TT("check"), 12);
			icon->m_color = color(UIThemeConstantPalette::PrimaryMainColor);
			s->decorators.add(icon);
		}
	}
	//--------------------------------
	// UIWindow
	{
		//auto e = sheet->addStyleSet(_TT("UIWindow");
		//{
		//	auto s = e->mainStyleClass()->mainStyle();

		if (auto s = sheet->obtainStyle(_TT("UIWindow"))) {
			s->minWidth = 64;
			s->minHeight = 64;
			s->setPadding(spacing(1));
			s->backgroundColor = color(UIThemeConstantPalette::DefaultBackgroundColor).withAlpha(0.5);
			s->cornerRadius = CornerRadius(4);
			s->borderThickness = 1;
			s->setBorderColor(color(UIThemeConstantPalette::DefaultDivider));
		}
	}
	//--------------------------------
	// UIListBoxItem
	{
		if (auto s = sheet->obtainStyle(_TT("UIListBoxItem"))) {
			s->minHeight = lineContentHeight();
			//s->padding = Thickness(spacing(1), 0);
			//s->setBackgroundColorTransition(color(UIThemeConstantPalette::ItemHoverAction), 1.0f);
			//s->setBackgroundColorTransition(Color::Transparency, 1.0f);
		}
		if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:MouseOver"))) {
			s->backgroundColor = color(UIThemeConstantPalette::ItemHoverAction);
			//s->setBackgroundColorTransition(color(UIThemeConstantPalette::ItemHoverAction), 1.0f);
		}
		if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:Unselected"))) {
			//s->backgroundColor = Color::Red;
			//s->setBackgroundColorTransition(Color::Green, 1.0f);
		}
		if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:Selected"))) {
			//s->backgroundColor = Color::Red;
			////s->setBackgroundColorTransition(Color::Red, 1.0f);
			////
			//s->backgroundColor = color(UIThemeConstantPalette::ItemHoverAction);

			s->setBackgroundColorAnimation(Color::White.withAlpha(0.3), Color::White.withAlpha(0.6), 0.5, EasingMode::Linear, 0.0f, AnimationWrapMode::Alternate);

		}
		if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:Focused"))) {	// 後から書かれたものが優先される
			//s->backgroundColor = Color::Blue;
			//s->backgroundColor = color(UIThemeConstantPalette::ItemSelectedAction);
			//s->setBackgroundColorAnimation(Color::White.withAlpha(0.3), Color::White.withAlpha(0.6), 0.5, EasingMode::Linear, 0.0f, AnimationWrapMode::Alternate);
		}

		if (auto s = sheet->obtainStyle(_TT("UIListBoxItem:Visible"))) {
			s->setOpacityAnimation(0.0f, 1.0f, 5.0f, EasingMode::Linear, 0.0f, AnimationWrapMode::Once);
		}
	}
	//--------------------------------
	// UITreeView
	{
		if (auto s = sheet->obtainStyle(_TT("UITreeView"))) {
			//s->horizontalContentAlignment = UIHAlignment::Left;
		}
	}
	//--------------------------------
	// UITreeItem
	{
		if (auto s = sheet->obtainStyle(_TT("UITreeItem"))) {
			s->minHeight = lineContentHeight();
			//s->hAlignment = UIHAlignment::Stretch;
			//s->vAlignment = UIVAlignment::Top;
		}
		if (auto s = sheet->obtainStyle(_TT("UITreeItem:MouseOver"))) {
			s->backgroundColor = color(UIThemeConstantPalette::ItemHoverAction);
		}
		if (auto s = sheet->obtainStyle(_TT("UITreeItem:Selected"))) {
			s->backgroundColor = color(UIThemeConstantPalette::ItemSelectedAction);
		}
		if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander"))) {   // VisualState によらず常に有効。個別にしたければ:Normalを付ける。
			s->width = 16;
			s->height = 16;
			//s->hAlignment = UIHAlignment::Center;
			//s->vAlignment = UIVAlignment::Center;
			s->backgroundColor = Color::Transparency;
		}
		//if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:MouseOver")) {
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:Pressed")) {
		//}
		if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:Checked"))) {
			auto icon = makeObject<UIStyleDecorator>();
			icon->setIconName(_TT("angle-down"), 12);
			icon->m_color = Color::White;
			s->decorators.add(icon);
		}
		if (auto s = sheet->obtainStyle(_TT("UIToggleButton.UITreeItem-Expander:Unchecked"))) {
			auto icon = makeObject<UIStyleDecorator>();
			icon->setIconName(_TT("angle-right"), 12);
			icon->m_color = Color::White;
			s->decorators.add(icon);
		}
	}


	//--------------------------------
	// UIThumb
	{
		if (auto s = sheet->obtainStyle(_TT("UIThumb.UITrack-Thumb"))) {
			s->backgroundColor = UIColors::get(UIColorHues::Grey, 5);
			s->cornerRadius = CornerRadius(3);
		}

		//auto e = sheet->addStyleSet(_TT("UIThumb");
		//// UIThumb
		//{
		//	auto s = e->mainStyleClass()->mainStyle();
		//}
		//// UIThumb.UITrack-Thumb
		//{
		//	auto s = makeObject<UIStyle>();

		//	s->margin = Thickness(2);
		//	s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
		//	s->cornerRadius = CornerRadius(4);
		//	s->hAlignment = UIHAlignment::Stretch;
		//	s->vAlignment = UIVAlignment::Stretch;

		//	s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
		//	e->mainStyleClass()->addStateStyle(_TT("UITrack-Thumb", s);
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIThumb.SplitterBar"))
		//{
		//	s->backgroundColor = Color(0, 1, 0, 0.2); // debug
		//	s->margin = Thickness(-2, -2, -2, -2);
		//}
	}

	//--------------------------------
	// UIScrollBar
	{
		if (auto s = sheet->obtainStyle(_TT("UIScrollBar"))) {
			//s->backgroundColor = UIColors::get(UIColorHues::Green, 7);
			s->minWidth = 6;
		}
	}
	//--------------------------------
	// UITrack
	{
		if (auto s = sheet->obtainStyle(_TT("UITrack"))) {
			s->backgroundColor = UIColors::get(UIColorHues::Grey, 7);
			s->cornerRadius = CornerRadius(3);
		}
		//if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-DecreaseButton")) {
		//	s->backgroundColor = Color(1, 1, 1, 0);
		//	s->cornerRadius = CornerRadius(0);
		//	s->shadowBlurRadius = 0;
		//	s->shadowOffsetY = 0;
		//	s->hAlignment = UIHAlignment::Stretch;
		//	s->vAlignment = UIVAlignment::Stretch;
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-DecreaseButton:MouseOver")) {	// ベース要素である UIButton の VisualState を全て上書きする必要がある。CSS と同じ動作。
		//	s->backgroundColor = Color::Transparency;
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-DecreaseButton:Pressed")) {
		//	s->backgroundColor = Color::Transparency;
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-IncreaseButton")) {
		//	s->backgroundColor = Color(1, 1, 1, 0);
		//	s->cornerRadius = CornerRadius(0);
		//	s->shadowBlurRadius = 0;
		//	s->shadowOffsetY = 0;
		//	s->hAlignment = UIHAlignment::Stretch;
		//	s->vAlignment = UIVAlignment::Stretch;
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-IncreaseButton:MouseOver")) {
		//	s->backgroundColor = Color::Transparency;
		//}
		//if (auto s = sheet->obtainStyle(_TT("UIButton.UITrack-IncreaseButton:Pressed")) {
		//	s->backgroundColor = Color::Transparency;
		//}

	}

	////--------------------------------
	//// UITabBar
	//{
	//	if (auto s = sheet->obtainStyle(_TT("UITabBar")) {
	//		s->backgroundColor = color(UIThemeConstantPalette::DefaultDivider);
	//	}
	//}

	//--------------------------------
	// UIComboBox
	{
		if (auto s = sheet->obtainStyle(_TT("UIComboBox"))) {
			s->minWidth = 64;
			s->minHeight = lineContentHeight();
			s->setMargin(Thickness(8));   // TODO: spacing?
			s->setPadding(Thickness(spacing(2), 0));
			//s->hAlignment = UIHAlignment::Center;
			//s->vAlignment = UIVAlignment::Center;
			s->horizontalContentAlignment = UIHAlignment::Center;
			s->verticalContentAlignment = UIVAlignment::Center;
			s->backgroundColor = UIColors::get(UIColorHues::Grey, 7);
			s->cornerRadius = CornerRadius(4);
			s->shadowBlurRadius = 1;
			s->shadowOffsetY = 1;
			s->shadowColor = Color(0, 0, 0, 0.5);
		}
	}

	//--------------------------------
	// UITabBarItem
	{
		if (auto s = sheet->obtainStyle(_TT("UITabBarItem"))) {
			s->minHeight = lineContentHeight();
			s->setPadding(Thickness(spacing(2), 0));
			s->backgroundColor = color(UIThemeConstantPalette::DefaultBackgroundColor);
		}
		if (auto s = sheet->obtainStyle(_TT("UITabBarItem:Selected"))) {
			s->backgroundColor = color(UIThemeConstantPalette::ItemSelectedAction);
		}
	}

	//--------------------------------
	// UITreeView
	{
		if (auto s = sheet->obtainStyle(_TT("UIPropertyField"))) {
			s->backgroundColor = color(UIThemeConstantPalette::ControlBackgroundColor);
			s->cornerRadius = CornerRadius(2);
			s->shadowBlurRadius = 1;
			s->shadowOffsetY = 1;
			s->shadowColor = Color(0, 0, 0, 0.5);
		}
	}

	m_styleSheet = sheet;
}

//==============================================================================
// UIVisualStates

const String UIVisualStates::CommonStates = _TT("CommonGroup");
const String UIVisualStates::FocusStates = _TT("FocusGroup");
const String UIVisualStates::Visibility = _TT("VisibilityGroup");
const String UIVisualStates::CheckStates = _TT("CheckStates");
const String UIVisualStates::ValidationStates = _TT("ValidationStates");
const String UIVisualStates::SelectionStates = _TT("SelectionStates");
const String UIVisualStates::DisplayStates = _TT("DisplayStates");

const String UIVisualStates::Normal = _TT("Normal");
const String UIVisualStates::MouseOver = _TT("MouseOver";)
const String UIVisualStates::Pressed = _TT("Pressed");
const String UIVisualStates::Disabled = _TT("Disabled");

const String UIVisualStates::Visible = _TT("Visible");
const String UIVisualStates::Hidden = _TT("Hidden");
const String UIVisualStates::Collapsed = _TT("Collapsed");	// Hidden のときはフェードアウト、Collapse のときは即非表示、といった区別のために用意

const String UIVisualStates::Focused = _TT("Focused");
const String UIVisualStates::Unfocused = _TT("Unfocused");

const String UIVisualStates::CheckedState = _TT("Checked");
const String UIVisualStates::UncheckedState = _TT("Unchecked");

const String UIVisualStates::Unselected = _TT("Unselected");
const String UIVisualStates::Selected = _TT("Selected");

const String UIVisualStates::Opend = _TT("Opend");
const String UIVisualStates::Closed = _TT("Closed");

} // namespace ln

