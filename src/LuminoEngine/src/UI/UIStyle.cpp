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
                "HAlignment" : "Left"
            }
        ]
    }
    ```

    #(ID) や .(class) は未サポート。直近では予定なし。


    
*/
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>

namespace ln {

//==============================================================================
// UIStyle

// layout
const Thickness UIStyle::DefaultMargin = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
const Thickness UIStyle::DefaultPadding = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
const HAlignment UIStyle::DefaultHorizontalAlignment = HAlignment::Left;
const VAlignment UIStyle::DefaultVerticalAlignment = VAlignment::Top;
const HAlignment UIStyle::DefaultHorizontalContentAlignment = HAlignment::Stretch;
const VAlignment UIStyle::DefaultVerticalContentAlignment = VAlignment::Stretch;
const float UIStyle::DefaultMinWidth = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultMinHeight = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultMaxWidth = std::numeric_limits<float>::quiet_NaN();
const float UIStyle::DefaultMaxHeight = std::numeric_limits<float>::quiet_NaN();

// transform
const Vector3 UIStyle::DefaultPosition = Vector3(0.0f, 0.0f, 0.0f);
const Quaternion UIStyle::DefaultRotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f); // Identity
const Vector3 UIStyle::DefaultScale = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 UIStyle::DefaultCenterPoint = Vector3(0.0f, 0.0f, 0.0f);

// background
const BrushImageDrawMode UIStyle::DefaultBackgroundDrawMode = BrushImageDrawMode::Image;
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

// shadow
const float UIStyle::DefaultShadowOffsetX = 0.0f;
const float UIStyle::DefaultShadowOffsetY = 0.0f;
const float UIStyle::DefaultShadowBlurRadius = 0.0f;
const float UIStyle::DefaultShadowSpreadRadius = 0.0f;
const Color UIStyle::DefaultShadowColor = Color(0, 0, 0, 0.25);
const bool UIStyle::DefaultShadowInset = false;

// text
const Color UIStyle::DefaultTextColor = Color::Black;
const String UIStyle::DefaultFontFamily = String::Empty;
const float UIStyle::DefaultFontSize = 20.0f;// WPF default は 12 だが、それだとデスクトップアプリ向けなので少し小さい。Lumino としては 20 をデフォルトとする。
const UIFontWeight UIStyle::DefaultFontWeight = UIFontWeight::Normal;
const UIFontStyle UIStyle::DefaultFontStyle = UIFontStyle::Normal;

// effects
const bool UIStyle::DefaultVisible = true;
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

void UIStyle::setupDefault()
{
    // layout
	margin = DefaultMargin;
	padding = DefaultPadding;
	horizontalAlignment = DefaultHorizontalAlignment;
	verticalAlignment = DefaultVerticalAlignment;
    horizontalContentAlignment = DefaultHorizontalContentAlignment;
    verticalContentAlignment = DefaultVerticalContentAlignment;
	minWidth = DefaultMinWidth;
	minHeight = DefaultMinHeight;
	maxWidth = DefaultMaxWidth;
	maxHeight = DefaultMaxHeight;

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
}

void UIStyle::reset()
{
	// layout
	margin.reset();
	padding.reset();
	horizontalAlignment.reset();
	verticalAlignment.reset();
	horizontalContentAlignment.reset();
	verticalContentAlignment.reset();
	minWidth.reset();
	minHeight.reset();
	maxWidth.reset();
	maxHeight.reset();

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
}

void UIStyle::mergeFrom(const UIStyle* other)
{
    if (LN_REQUIRE(other)) return;

    // layout
    if (other->margin.hasValue()) margin = other->margin.get();
    if (other->padding.hasValue()) padding = other->padding.get();
    if (other->horizontalAlignment.hasValue()) horizontalAlignment = other->horizontalAlignment.get();
    if (other->verticalAlignment.hasValue()) verticalAlignment = other->verticalAlignment.get();
    if (other->horizontalContentAlignment.hasValue()) horizontalContentAlignment = other->horizontalContentAlignment.get();
    if (other->verticalContentAlignment.hasValue()) verticalContentAlignment = other->verticalContentAlignment.get();
    if (other->minWidth.hasValue()) minWidth = other->minWidth.get();
    if (other->minHeight.hasValue()) minHeight = other->minHeight.get();
    if (other->maxWidth.hasValue()) maxWidth = other->maxWidth.get();
    if (other->maxHeight.hasValue()) maxHeight = other->maxHeight.get();

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
}

void UIStyle::copyFrom(const UIStyle* other)
{
    if (LN_REQUIRE(other)) return;

    // layout
    margin = other->margin;
    padding = other->padding;
    horizontalAlignment = other->horizontalAlignment;
    verticalAlignment = other->verticalAlignment;
    horizontalContentAlignment = other->horizontalContentAlignment;
    verticalContentAlignment = other->verticalContentAlignment;
    minWidth = other->minWidth;
    minHeight = other->minHeight;
    maxWidth = other->maxWidth;
    maxHeight = other->maxHeight;

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
}

//==============================================================================
// UIStyleSet

UIStyleSet::UIStyleSet()
{
}

UIStyleSet::~UIStyleSet()
{
}

void UIStyleSet::init(const StringRef& elementName)
{
    Object::init();
	m_elementName = elementName;
	m_mainStyleClass = makeObject<UIStyleClass>(u"");
}

void UIStyleSet::addStyleClass(UIStyleClass* styleClass)
{
    if (LN_REQUIRE(styleClass)) return;
	if (LN_REQUIRE(!styleClass->name().isEmpty())) return;
	m_styleClasses.add(styleClass);
}

UIStyleClass* UIStyleSet::addStyleClass(const StringRef& className)
{
	auto ptr = makeObject<UIStyleClass>(className);
	addStyleClass(ptr);
	return ptr;
}

UIStyleClass* UIStyleSet::findStyleClass(const StringRef& className) const
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

void UIStyleClass::init(const StringRef& name)
{
	Object::init();
	m_name = name;
	m_mainStyle = makeObject<UIStyle>();
}

void UIStyleClass::addStateStyle(const StringRef& stateName, UIStyle* style)
{
	if (LN_REQUIRE(style)) return;
	m_visualStateStyles.add({ stateName, style });
}

UIStyle* UIStyleClass::addStateStyle(const StringRef& stateName)
{
	auto ptr = makeObject<UIStyle>();
	addStateStyle(stateName, ptr);
	return ptr;
}

UIStyle* UIStyleClass::findStateStyle(const StringRef& stateName) const
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

void UIStyleSheet::addStyleSet(const StringRef& elementName, UIStyleSet* styleClass)
{
    if (LN_REQUIRE(styleClass)) return;
    m_classes.insert({ elementName, styleClass });
}

Ref<UIStyleSet> UIStyleSheet::addStyleSet(const StringRef& elementName)
{
    auto cls = makeObject<UIStyleSet>(elementName);
    addStyleSet(elementName, cls);
    return cls;
}

UIStyleSet* UIStyleSheet::findStyleSet(const StringRef& elementName) const
{
    auto itr = m_classes.find(elementName);
    if (itr != m_classes.end())
        return itr->second;
    else
        return nullptr;
}

UIStyle* UIStyleSheet::obtainStyle(const StringRef& selector)
{
	String actualSelector = selector;
	int classBegin = actualSelector.lastIndexOf(u".");
	int stateBegin = actualSelector.lastIndexOf(u":");

	int elementEnd = (classBegin > 0) ? classBegin : ((stateBegin > 0) ? stateBegin : actualSelector.length());
	int classEnd = (stateBegin > 0) ? stateBegin : actualSelector.length();
	int stateEnd = actualSelector.length();

	StringRef elementName;
	StringRef className;
	StringRef stateName;
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
}

void UIStyleContext::build()
{
    // first, merge globals
    m_globalStyle = makeObject<UIStyleSet>(u"");
	m_globalStyle->mainStyleClass()->mainStyle()->setupDefault();
    for (auto& sheet : m_styleSheets) {
        auto globalStyle = sheet->findStyleSet(u"");
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

UIStyleSet* UIStyleContext::findStyleSet(const StringRef& elementName) const
{
    auto itr = m_elementStyles.find(elementName);
    if (itr != m_elementStyles.end())
        return itr->second;
    else
        return m_globalStyle;
}

//detail::UIStyleClassInstance* UIStyleContext::findResolvedStyleClass(const StringRef& elementName) const
//{
//    auto itr = m_resolvedClasses.find(elementName);
//    if (itr != m_resolvedClasses.end())
//        return itr->second;
//    else
//        return m_resolvedGlobalStyle;
//}

void UIStyleContext::combineStyle(UIStyle* style, const StringRef& elementName, const List<String>* classList) const
{
	auto set = findStyleSet(elementName);

	style->mergeFrom(set->mainStyleClass()->mainStyle());

	if (classList) {
		for (auto& name : *classList) {
			auto cs = set->findStyleClass(name);
			if (cs) {
				style->mergeFrom(cs->mainStyle());
			}
		}
	}
}


//==============================================================================
// UIStyleInstance

namespace detail {

UIStyleInstance::UIStyleInstance()
{
}

void UIStyleInstance::setupDefault()
{
    margin = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
    padding = Thickness(0.0f, 0.0f, 0.0f, 0.0f);

    // Alignment は HTML のデフォルトに合わせてみる
    horizontalAlignment = HAlignment::Left;
    verticalAlignment = VAlignment::Top;

    horizontalContentAlignment = HAlignment::Stretch;
    verticalContentAlignment = VAlignment::Stretch;

    minWidth = Math::NaN;
    minHeight = Math::NaN;
    maxWidth = Math::NaN;
    maxHeight = Math::NaN;

    position = Vector3::Zero;
    rotation = Quaternion::Identity;
    scale = Vector3::Ones;
    centerPoint = Vector3::Zero;

    backgroundDrawMode = BrushImageDrawMode::Image;
    backgroundImageRect = Rect::Zero;
    backgroundImageBorder = Thickness::Zero;

    textColor = Color::Black;
    fontFamily = String::Empty;
    fontSize = 20.0f;	// WPF default は 12 だが、それだとデスクトップアプリ向けなので少し小さい。Lumino としては 20 をデフォルトとする。
    fontWeight = UIFontWeight::Normal;
    fontStyle = UIFontStyle::Normal;

    visible = true;
    blendMode = BlendMode::Alpha;

    opacity = 1.0f;
    colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
    blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
    tone = ColorTone(0.0f, 0.0f, 0.0f, 0.0f);

 //   auto tmp = makeObject<UIStyle>();
 //   tmp->setupDefault();
	//backgroundMaterial = makeObject<Material>();
 //   mergeFrom(tmp);
}

void UIStyleInstance::mergeFrom(const UIStyle* other)
{
    if (LN_REQUIRE(other)) return;
    
    // layout
    if (other->margin.hasValue()) margin = other->margin.get();
    if (other->padding.hasValue()) padding = other->padding.get();
    if (other->horizontalAlignment.hasValue()) horizontalAlignment = other->horizontalAlignment.get();
    if (other->verticalAlignment.hasValue()) verticalAlignment = other->verticalAlignment.get();
    if (other->horizontalContentAlignment.hasValue()) horizontalContentAlignment = other->horizontalContentAlignment.get();
    if (other->verticalContentAlignment.hasValue()) verticalContentAlignment = other->verticalContentAlignment.get();
    if (other->minWidth.hasValue()) minWidth = other->minWidth.get();
    if (other->minHeight.hasValue()) minHeight = other->minHeight.get();
    if (other->maxWidth.hasValue()) maxWidth = other->maxWidth.get();
    if (other->maxHeight.hasValue()) maxHeight = other->maxHeight.get();

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
}

void UIStyleInstance::copyFrom(const UIStyleInstance* other)
{
    if (LN_REQUIRE(other)) return;

    // layout
    margin = other->margin;
    padding = other->padding;
    horizontalAlignment = other->horizontalAlignment;
    verticalAlignment = other->verticalAlignment;
    horizontalContentAlignment = other->horizontalContentAlignment;
    verticalContentAlignment = other->verticalContentAlignment;
    minWidth = other->minWidth;
    minHeight = other->minHeight;
    maxWidth = other->maxWidth;
    maxHeight = other->maxHeight;

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

	// shadow
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
}

void UIStyleInstance::makeRenderObjects()
{

}

void UIStyleInstance::updateStyleDataHelper(UIStyle* localStyle, const detail::UIStyleInstance* parentStyleData, const UIStyle* combinedStyle, detail::UIStyleInstance* outStyleData)
{
	//const UIStyle* parentStyle = (parentStyleData) ? parentStyleData->sourceLocalStyle : nullptr;
	//if (parentStyle)
	//{
	//	// text 関係は継承する
	//	localStyle->textColor.inherit(parentStyle->textColor);
	//	localStyle->fontFamily.inherit(parentStyle->fontFamily);
	//	localStyle->fontSize.inherit(parentStyle->fontSize);
	//	localStyle->fontWeight.inherit(parentStyle->fontWeight);
	//	localStyle->fontStyle.inherit(parentStyle->fontStyle);
	//}

    // inheritance
    {
        if (parentStyleData) {
            // 1. Local style
            // 2. Style sheet
            // 3. Parent
            outStyleData->textColor = localStyle->textColor.getOrDefault(combinedStyle->textColor.getOrDefault(parentStyleData->textColor));
            outStyleData->fontFamily = localStyle->fontFamily.getOrDefault(combinedStyle->fontFamily.getOrDefault(parentStyleData->fontFamily));
            outStyleData->fontSize = localStyle->fontSize.getOrDefault(combinedStyle->fontSize.getOrDefault(parentStyleData->fontSize));
            outStyleData->fontWeight = localStyle->fontWeight.getOrDefault(combinedStyle->fontWeight.getOrDefault(parentStyleData->fontWeight));
            outStyleData->fontStyle = localStyle->fontStyle.getOrDefault(combinedStyle->fontStyle.getOrDefault(parentStyleData->fontStyle));
        }
        else {
            // for root default style
            outStyleData->textColor = localStyle->textColor.getOrDefault(combinedStyle->textColor.getOrDefault(UIStyle::DefaultTextColor));
            outStyleData->fontFamily = localStyle->fontFamily.getOrDefault(combinedStyle->fontFamily.getOrDefault(UIStyle::DefaultFontFamily));
            outStyleData->fontSize = localStyle->fontSize.getOrDefault(combinedStyle->fontSize.getOrDefault(UIStyle::DefaultFontSize));
            outStyleData->fontWeight = localStyle->fontWeight.getOrDefault(combinedStyle->fontWeight.getOrDefault(UIStyle::DefaultFontWeight));
            outStyleData->fontStyle = localStyle->fontStyle.getOrDefault(combinedStyle->fontStyle.getOrDefault(UIStyle::DefaultFontStyle));
        }
    }

	//outStyleData->sourceLocalStyle = localStyle;


	// layout
	{
		outStyleData->margin = localStyle->margin.getOrDefault(combinedStyle->margin.getOrDefault(UIStyle::DefaultMargin));
		outStyleData->padding = localStyle->padding.getOrDefault(combinedStyle->padding.getOrDefault(UIStyle::DefaultPadding));
		outStyleData->horizontalAlignment = localStyle->horizontalAlignment.getOrDefault(combinedStyle->horizontalAlignment.getOrDefault(UIStyle::DefaultHorizontalAlignment));
		outStyleData->verticalAlignment = localStyle->verticalAlignment.getOrDefault(combinedStyle->verticalAlignment.getOrDefault(UIStyle::DefaultVerticalAlignment));
        outStyleData->horizontalContentAlignment = localStyle->horizontalContentAlignment.getOrDefault(combinedStyle->horizontalContentAlignment.getOrDefault(UIStyle::DefaultHorizontalContentAlignment));
        outStyleData->verticalContentAlignment = localStyle->verticalContentAlignment.getOrDefault(combinedStyle->verticalContentAlignment.getOrDefault(UIStyle::DefaultVerticalContentAlignment));
		outStyleData->minWidth = localStyle->minWidth.getOrDefault(combinedStyle->minWidth.getOrDefault(UIStyle::DefaultMinWidth));
		outStyleData->minHeight = localStyle->minHeight.getOrDefault(combinedStyle->minHeight.getOrDefault(UIStyle::DefaultMinHeight));
		outStyleData->maxWidth = localStyle->maxWidth.getOrDefault(combinedStyle->maxWidth.getOrDefault(UIStyle::DefaultMaxWidth));
		outStyleData->maxHeight = localStyle->maxHeight.getOrDefault(combinedStyle->maxHeight.getOrDefault(UIStyle::DefaultMaxHeight));
	}

	// layout transform
	{
		outStyleData->position = localStyle->position.getOrDefault(combinedStyle->position.getOrDefault(UIStyle::DefaultPosition));
		outStyleData->rotation = localStyle->rotation.getOrDefault(combinedStyle->rotation.getOrDefault(UIStyle::DefaultRotation));
		outStyleData->scale = localStyle->scale.getOrDefault(combinedStyle->scale.getOrDefault(UIStyle::DefaultScale));
		outStyleData->centerPoint = localStyle->centerPoint.getOrDefault(combinedStyle->centerPoint.getOrDefault(UIStyle::DefaultCenterPoint));
	}

	// background
	{
		assert(outStyleData->backgroundMaterial);
        outStyleData->backgroundDrawMode = localStyle->backgroundDrawMode.getOrDefault(combinedStyle->backgroundDrawMode.getOrDefault(UIStyle::DefaultBackgroundDrawMode));
		outStyleData->backgroundColor = localStyle->backgroundColor.getOrDefault(combinedStyle->backgroundColor.getOrDefault(UIStyle::DefaultBackgroundColor));
		outStyleData->backgroundMaterial->setMainTexture(localStyle->backgroundImage.getOrDefault(combinedStyle->backgroundImage.getOrDefault(UIStyle::DefaultBackgroundImage)));
		outStyleData->backgroundMaterial->setShader(localStyle->backgroundShader.getOrDefault(combinedStyle->backgroundShader.getOrDefault(UIStyle::DefaultBackgroundShader)));
        outStyleData->backgroundImageRect = localStyle->backgroundImageRect.getOrDefault(combinedStyle->backgroundImageRect.getOrDefault(UIStyle::DefaultBackgroundImageRect));
        outStyleData->backgroundImageBorder = localStyle->backgroundImageBorder.getOrDefault(combinedStyle->backgroundImageBorder.getOrDefault(UIStyle::DefaultBackgroundImageBorder));
	}

    // border
    {
        outStyleData->borderThickness = localStyle->borderThickness.getOrDefault(combinedStyle->borderThickness.getOrDefault(UIStyle::DefaultBorderThickness));
        outStyleData->cornerRadius = localStyle->cornerRadius.getOrDefault(combinedStyle->cornerRadius.getOrDefault(UIStyle::DefaultCornerRadius));
        outStyleData->leftBorderColor = localStyle->leftBorderColor.getOrDefault(combinedStyle->leftBorderColor.getOrDefault(UIStyle::DefaultLeftBorderColor));
        outStyleData->topBorderColor = localStyle->topBorderColor.getOrDefault(combinedStyle->topBorderColor.getOrDefault(UIStyle::DefaultTopBorderColor));
        outStyleData->rightBorderColor = localStyle->rightBorderColor.getOrDefault(combinedStyle->rightBorderColor.getOrDefault(UIStyle::DefaultRightBorderColor));
        outStyleData->bottomBorderColor = localStyle->bottomBorderColor.getOrDefault(combinedStyle->bottomBorderColor.getOrDefault(UIStyle::DefaultBottomBorderColor));
        outStyleData->borderDirection = localStyle->borderDirection.getOrDefault(combinedStyle->borderDirection.getOrDefault(UIStyle::DefaultBorderDirection));
    }

	// shadow
	{
		outStyleData->shadowOffsetX = localStyle->shadowOffsetX.getOrDefault(combinedStyle->shadowOffsetX.getOrDefault(UIStyle::DefaultShadowOffsetX));
		outStyleData->shadowOffsetY = localStyle->shadowOffsetY.getOrDefault(combinedStyle->shadowOffsetY.getOrDefault(UIStyle::DefaultShadowOffsetY));
		outStyleData->shadowBlurRadius = localStyle->shadowBlurRadius.getOrDefault(combinedStyle->shadowBlurRadius.getOrDefault(UIStyle::DefaultShadowBlurRadius));
		outStyleData->shadowSpreadRadius = localStyle->shadowSpreadRadius.getOrDefault(combinedStyle->shadowSpreadRadius.getOrDefault(UIStyle::DefaultShadowSpreadRadius));
		outStyleData->shadowColor = localStyle->shadowColor.getOrDefault(combinedStyle->shadowColor.getOrDefault(UIStyle::DefaultShadowColor));
		outStyleData->shadowInset = localStyle->shadowInset.getOrDefault(combinedStyle->shadowInset.getOrDefault(UIStyle::DefaultShadowInset));
	}

	// text
	{
		//outStyleData->textColor = localStyle->textColor.getOrDefault(combinedStyle->textColor.get());

		if (outStyleData->font) {
			// 前回の update で選択した font があるのでそのままにする
		}
		else {
			// element 作成直後。まだ font の選択処理を行っていないときはとりあえず親のフォントを参照
			outStyleData->font = (parentStyleData) ? parentStyleData->font : nullptr;
		}

		detail::FontDesc desc;
		desc.Family = outStyleData->fontFamily;
		desc.Size = outStyleData->fontSize;
		desc.isBold = (outStyleData->fontWeight == UIFontWeight::Bold);
		desc.isItalic = (outStyleData->fontStyle == UIFontStyle::Italic);
		desc.isAntiAlias = true;

		bool modified = true;
		if (outStyleData->font) {
			if (detail::FontHelper::equalsFontDesc(outStyleData->font, desc)) {
				modified = false;
			}
		}
		else {
			// 親が font を持っていなかった
		}

		if (modified) {
			outStyleData->font = makeObject<Font>(desc);
		}
	}

	// render effects
	{
		outStyleData->visible = localStyle->visible.getOrDefault(combinedStyle->visible.getOrDefault(UIStyle::DefaultVisible));
		outStyleData->blendMode = localStyle->blendMode.getOrDefault(combinedStyle->blendMode.getOrDefault(UIStyle::DefaultBlendMode));
		outStyleData->opacity = localStyle->opacity.getOrDefault(combinedStyle->opacity.getOrDefault(UIStyle::DefaultOpacity));
		outStyleData->colorScale = localStyle->colorScale.getOrDefault(combinedStyle->colorScale.getOrDefault(UIStyle::DefaultColorScale));
		outStyleData->blendColor = localStyle->blendColor.getOrDefault(combinedStyle->blendColor.getOrDefault(UIStyle::DefaultBlendColor));
		outStyleData->tone = localStyle->tone.getOrDefault(combinedStyle->tone.getOrDefault(UIStyle::DefaultTone));
	}
}

//==============================================================================
// UIStyleClassInstance

UIStyleClassInstance::UIStyleClassInstance()
    : m_style(makeRef<UIStyleInstance>())
{
}

UIStyleInstance* UIStyleClassInstance::findStateStyle(const StringRef& stateName) const
{
    auto style = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, stateName, CaseSensitivity::CaseInsensitive); });
    if (style)
        return style->style;
    else
        return m_style;
}

UIStyleInstance* UIStyleClassInstance::findSubElementStyle(const StringRef& elementName) const
{
    auto style = m_subElements.findIf([&](auto& x) { return String::compare(x.name, elementName, CaseSensitivity::CaseInsensitive); });
    if (style)
        return style->style;
    else
        return nullptr;
}

#if 0
void UIStyleClassInstance::mergeFrom(const UIStyleSet* other)
{
    if (LN_REQUIRE(other)) return;

    // Merge main style
    m_style->mergeFrom(other->m_style);

    // Merge visual-state style
    for (auto& otherSlot : other->m_visualStateStyles) {
        auto slot = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, otherSlot.name, CaseSensitivity::CaseInsensitive); });
        if (slot) {
            slot->style->mergeFrom(otherSlot.style);
        }
        else {
            auto ptr = makeRef<UIStyleInstance>();
            m_visualStateStyles.add({ otherSlot.name, ptr });
            ptr->mergeFrom(otherSlot.style);
        }
    }

    // Merge sub-element style
    for (auto& otherSlot : other->m_classSlots) {
        auto slot = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, otherSlot.name, CaseSensitivity::CaseInsensitive); });
        if (slot) {
            slot->style->mergeFrom(otherSlot.style);
        }
        else {
            auto ptr = makeRef<UIStyleInstance>();
            m_subElements.add({ otherSlot.name, ptr });
            ptr->mergeFrom(otherSlot.style);
        }
    }
}
#endif

} // namespace detail

//==============================================================================
// UIVisualStateManager

UIVisualStateManager::UIVisualStateManager()
    : m_groups()
    , m_combinedStyle(makeObject<UIStyle>())
    , m_resolvedStyle(makeRef<detail::UIStyleInstance>())
    , m_dirty(true)
{
}

void UIVisualStateManager::init()
{
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
void UIVisualStateManager::combineStyle(UIStyle* style, const UIStyleContext* styleContext, const ln::String& elementName, const List<String>* classList)
{
	// TODO: これキャッシュできると思う
	auto set = styleContext->findStyleSet(elementName);

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

	if (classList) {
		for (auto& name : *classList) {
			auto cs = set->findStyleClass(name);
			if (cs) {
				//std::cout << "ff:" << name << std::endl;
				for (auto& group : m_groups) {
					if (group.activeStateIndex >= 0) {
						if (auto stateStyle = cs->findStateStyle(group.stateNames[group.activeStateIndex])) {

							//if (group.stateNames[group.activeStateIndex] == u"Pressed") {
							//	std::cout << group.stateNames[group.activeStateIndex] << std::endl;
							//}
							style->mergeFrom(stateStyle);

						}
					}
				}
			}
		}
	}
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
// UIColors

static bool g_colorsInit = false;
static Color g_colors[20][UIColors::MaxShades];

static const void InitColors()
{
    if (!g_colorsInit)
    {
#define LN_COLOR_DEF(name, depth, r, g, b) g_colors[(int)UIColorHues::name][depth] = Color(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f, 1.0f);
#define LN_COLOR_DEF_A(name, depth, r, g, b)
#include "UIColorsDefine.inl"

        for (int i = 0; i < UIColors::MaxShades; i++) g_colors[(int)UIColorHues::Black][i] = Color(0, 0, 0, 1);
        for (int i = 0; i < UIColors::MaxShades; i++) g_colors[(int)UIColorHues::White][i] = Color(1, 1, 1, 1);

        g_colorsInit = true;
    }
}

const Color& UIColors::get(UIColorHues index, int depth)
{
    if (LN_REQUIRE_RANGE(depth, 0, UIColors::MaxShades)) return Color::Black;
    InitColors();
    return g_colors[(int)index][depth];
}


//==============================================================================
// UIVisualStates

const String UIVisualStates::CommonStates = u"CommonGroup";
const String UIVisualStates::FocusStates = u"FocusGroup";
const String UIVisualStates::CheckStates = u"CheckStates";
const String UIVisualStates::ValidationStates = u"ValidationStates";

const String UIVisualStates::Normal = u"Normal";
const String UIVisualStates::MouseOver = u"MouseOver";
const String UIVisualStates::Pressed = u"Pressed";
const String UIVisualStates::Disabled = u"Disabled";

const String UIVisualStates::CheckedState = u"Checked";
const String UIVisualStates::UncheckedState = u"Unchecked";

} // namespace ln

