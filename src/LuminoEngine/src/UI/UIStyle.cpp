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
	margin = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
	padding = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
	horizontalAlignment = HAlignment::Left;  // Alignment は HTML のデフォルトに合わせてみる
	verticalAlignment = VAlignment::Top;
	minWidth = Math::NaN;
	minHeight = Math::NaN;
	maxWidth = Math::NaN;
	maxHeight = Math::NaN;

    // layout transform
	position = Vector3::Zero;
	rotation = Quaternion::Identity;
	scale = Vector3::Ones;
	centerPoint = Vector3::Zero;

    // background
    backgroundDrawMode = BrushImageDrawMode::Image;
    backgroundColor = Color::Transparency;
    backgroundImage = nullptr;
    backgroundShader = nullptr;
    backgroundImageRect = Rect::Zero;
    backgroundImageBorder = Thickness::Zero;

    // border
    borderThickness = Thickness::Zero;
    cornerRadius = CornerRadius(0, 0, 0, 0);
    leftBorderColor = Color::Black;
    topBorderColor = Color::Black;
    rightBorderColor = Color::Black;
    bottomBorderColor = Color::Black;
    borderDirection = BorderDirection::Outside;

    // text
	textColor = Color::Black;
	fontFamily = String::Empty;
	fontSize = 20.0f;	// WPF default は 12 だが、それだとデスクトップアプリ向けなので少し小さい。Lumino としては 20 をデフォルトとする。
	fontWeight = UIFontWeight::Normal;
	fontStyle = UIFontStyle::Normal;

    // render effects
	visible = true;
	blendMode = BlendMode::Alpha;
	opacity = 1.0f;
	colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
	blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
	tone = ColorTone(0.0f, 0.0f, 0.0f, 0.0f);
}

void UIStyle::mergeFrom(const UIStyle* other)
{
    if (LN_REQUIRE(other)) return;

    // layout
    if (other->margin.hasValue()) margin = other->margin.get();
    if (other->padding.hasValue()) padding = other->padding.get();
    if (other->horizontalAlignment.hasValue()) horizontalAlignment = other->horizontalAlignment.get();
    if (other->verticalAlignment.hasValue()) verticalAlignment = other->verticalAlignment.get();
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
    borderThickness = borderThickness;
    cornerRadius = cornerRadius;
    leftBorderColor = leftBorderColor;
    topBorderColor = topBorderColor;
    rightBorderColor = rightBorderColor;
    bottomBorderColor = bottomBorderColor;
    borderDirection = borderDirection;

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
// UIStyleClass

UIStyleClass::UIStyleClass()
{
}

UIStyleClass::~UIStyleClass()
{
}

void UIStyleClass::init()
{
    Object::init();
    m_style = makeObject<UIStyle>();
}

void UIStyleClass::addStateStyle(const StringRef& stateName, UIStyle* style)
{
    if (LN_REQUIRE(style)) return;
    m_visualStateStyles.add({ stateName, style });
}

UIStyle* UIStyleClass::findStateStyle(const StringRef& stateName) const
{
    auto style = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, stateName, CaseSensitivity::CaseInsensitive) == 0; });
    if (style)
        return style->style;
    else
        return nullptr;
}

void UIStyleClass::addSubElementStyle(const StringRef& elementName, UIStyle* style)
{
    if (LN_REQUIRE(style)) return;
    m_subElements.add({ elementName, style });
}

UIStyle* UIStyleClass::findSubElementStyle(const StringRef& elementName) const
{
    auto style = m_subElements.findIf([&](auto& x) { return String::compare(x.name, elementName, CaseSensitivity::CaseInsensitive) == 0; });
    if (style)
        return style->style;
    else
        return nullptr;
}

void UIStyleClass::mergeFrom(const UIStyleClass* other)
{
    if (LN_REQUIRE(other)) return;

    // Merge main style
    m_style->mergeFrom(other->m_style);

    // Merge visual-state style
    for (auto& slot : other->m_visualStateStyles) {
        auto style = findStateStyle(slot.name);
        if (!style) {
            m_visualStateStyles.add({ slot.name, makeObject<UIStyle>() });
            style = m_visualStateStyles.back().style;
        }
        style->mergeFrom(slot.style);
    }

    // Merge sub-element style
    for (auto& slot : other->m_subElements) {
        auto style = findSubElementStyle(slot.name);
        if (!style) {
            m_subElements.add({ slot.name, makeObject<UIStyle>() });
            style = m_subElements.back().style;
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

void UIStyleSheet::addStyleClass(const StringRef& className, UIStyleClass* styleClass)
{
    if (LN_REQUIRE(styleClass)) return;
    m_classes.insert({ className, styleClass });
}

Ref<UIStyleClass> UIStyleSheet::addStyleClass(const StringRef& className)
{
    auto cls = makeObject<UIStyleClass>();
    addStyleClass(className, cls);
    return cls;
}

UIStyleClass* UIStyleSheet::findStyleClass(const StringRef& className) const
{
    auto itr = m_classes.find(className);
    if (itr != m_classes.end())
        return itr->second;
    else
        return nullptr;
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
    m_globalStyle = makeObject<UIStyleClass>();
    m_globalStyle->m_style->setupDefault();
    for (auto& sheet : m_styleSheets) {
        auto globalStyle = sheet->findStyleClass(u"*");
        if (globalStyle) {
            m_globalStyle->mergeFrom(globalStyle);
        }
    }
    m_resolvedGlobalStyle = makeRef<detail::UIStyleClassInstance>();
    m_resolvedGlobalStyle->mergeFrom(m_globalStyle);

    // second, merge other styles
    for (auto& sheet : m_styleSheets) {
        for (auto& pair : sheet->m_classes) {
            auto itr = m_classes.find(pair.first);
            if (itr == m_classes.end()) {
                auto instance = makeObject<UIStyleClass>();
                instance->m_style->copyFrom(m_globalStyle->m_style);
                m_classes.insert({ pair.first, instance });
                instance->mergeFrom(pair.second);

                {
                    auto ptr2 = makeRef<detail::UIStyleClassInstance>();
                    ptr2->m_style->setupDefault();
                    ptr2->mergeFrom(pair.second);
                    m_resolvedClasses.insert({ pair.first, ptr2 });
                }
            }
            else {
                itr->second->mergeFrom(pair.second);

                m_resolvedClasses[pair.first]->mergeFrom(pair.second);
            }
        }
    }
}

UIStyleClass* UIStyleContext::findStyleClass(const StringRef& className) const
{
    auto itr = m_classes.find(className);
    if (itr != m_classes.end())
        return itr->second;
    else
        return m_globalStyle;
}

detail::UIStyleClassInstance* UIStyleContext::findResolvedStyleClass(const StringRef& className) const
{
    auto itr = m_resolvedClasses.find(className);
    if (itr != m_resolvedClasses.end())
        return itr->second;
    else
        return m_resolvedGlobalStyle;
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
    borderThickness = borderThickness;
    cornerRadius = cornerRadius;
    leftBorderColor = leftBorderColor;
    topBorderColor = topBorderColor;
    rightBorderColor = rightBorderColor;
    bottomBorderColor = bottomBorderColor;
    borderDirection = borderDirection;

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
            outStyleData->textColor = localStyle->textColor.getOrDefault(combinedStyle->textColor.get());
            outStyleData->fontFamily = localStyle->fontFamily.getOrDefault(combinedStyle->fontFamily.get());
            outStyleData->fontSize = localStyle->fontSize.getOrDefault(combinedStyle->fontSize.get());
            outStyleData->fontWeight = localStyle->fontWeight.getOrDefault(combinedStyle->fontWeight.get());
            outStyleData->fontStyle = localStyle->fontStyle.getOrDefault(combinedStyle->fontStyle.get());
        }
    }

	//outStyleData->sourceLocalStyle = localStyle;


	// layout
	{
		outStyleData->margin = localStyle->margin.getOrDefault(combinedStyle->margin.get());
		outStyleData->padding = localStyle->padding.getOrDefault(combinedStyle->padding.get());
		outStyleData->horizontalAlignment = localStyle->horizontalAlignment.getOrDefault(combinedStyle->horizontalAlignment.get());
		outStyleData->verticalAlignment = localStyle->verticalAlignment.getOrDefault(combinedStyle->verticalAlignment.get());
		outStyleData->minWidth = localStyle->minWidth.getOrDefault(combinedStyle->minWidth.get());//.getOrDefault(Math::NaN));
		outStyleData->minHeight = localStyle->minHeight.getOrDefault(combinedStyle->minHeight.get());//.getOrDefault(Math::NaN));
		outStyleData->maxWidth = localStyle->maxWidth.getOrDefault(combinedStyle->maxWidth.get());//.getOrDefault(Math::NaN));
		outStyleData->maxHeight = localStyle->maxHeight.getOrDefault(combinedStyle->maxHeight.get());//.getOrDefault(Math::NaN));
	}

	// layout transform
	{
		outStyleData->position = localStyle->position.getOrDefault(combinedStyle->position.get());
		outStyleData->rotation = localStyle->rotation.getOrDefault(combinedStyle->rotation.get());
		outStyleData->scale = localStyle->scale.getOrDefault(combinedStyle->scale.get());
		outStyleData->centerPoint = localStyle->centerPoint.getOrDefault(combinedStyle->centerPoint.get());
	}

	// background
	{
		assert(outStyleData->backgroundMaterial);
        outStyleData->backgroundDrawMode = localStyle->backgroundDrawMode.getOrDefault(combinedStyle->backgroundDrawMode.get());
		outStyleData->backgroundColor = localStyle->backgroundColor.getOrDefault(combinedStyle->backgroundColor.get());
		outStyleData->backgroundMaterial->setMainTexture(localStyle->backgroundImage.getOrDefault(combinedStyle->backgroundImage.get()));
		outStyleData->backgroundMaterial->setShader(localStyle->backgroundShader.getOrDefault(combinedStyle->backgroundShader.get()));
		//outStyleData->backgroundColor = localStyle->backgroundColor.getOrDefault(combinedStyle->backgroundColor.get());
		//outStyleData->backgroundImage = 
        outStyleData->backgroundImageRect = localStyle->backgroundImageRect.getOrDefault(combinedStyle->backgroundImageRect.get());
        outStyleData->backgroundImageBorder = localStyle->backgroundImageBorder.getOrDefault(combinedStyle->backgroundImageBorder.get());
	}


    // border
    {
        outStyleData->borderThickness = localStyle->borderThickness.getOrDefault(combinedStyle->borderThickness.get());
        outStyleData->cornerRadius = localStyle->cornerRadius.getOrDefault(combinedStyle->cornerRadius.get());
        outStyleData->leftBorderColor = localStyle->leftBorderColor.getOrDefault(combinedStyle->leftBorderColor.get());
        outStyleData->topBorderColor = localStyle->topBorderColor.getOrDefault(combinedStyle->topBorderColor.get());
        outStyleData->rightBorderColor = localStyle->rightBorderColor.getOrDefault(combinedStyle->rightBorderColor.get());
        outStyleData->bottomBorderColor = localStyle->bottomBorderColor.getOrDefault(combinedStyle->bottomBorderColor.get());
        outStyleData->borderDirection = localStyle->borderDirection.getOrDefault(combinedStyle->borderDirection.get());
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
		outStyleData->visible = localStyle->visible.getOrDefault(combinedStyle->visible.get());
		outStyleData->blendMode = localStyle->blendMode.getOrDefault(combinedStyle->blendMode.get());

		outStyleData->opacity = localStyle->opacity.getOrDefault(combinedStyle->opacity);
		outStyleData->colorScale = localStyle->colorScale.getOrDefault(combinedStyle->colorScale.get());
		outStyleData->blendColor = localStyle->blendColor.getOrDefault(combinedStyle->blendColor.get());
		outStyleData->tone = localStyle->tone.getOrDefault(combinedStyle->tone.get());
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

void UIStyleClassInstance::mergeFrom(const UIStyleClass* other)
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
    for (auto& otherSlot : other->m_subElements) {
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

UIStyle* UIVisualStateManager::combineStyle(const UIStyleContext* styleContext, const ln::String& className)
{
    if (isDirty()) {
        m_combinedStyle->setupDefault();
        auto styleClass = styleContext->findStyleClass(className);

        // main style
        m_combinedStyle->mergeFrom(styleClass->style());

        for (auto& group : m_groups) {
            if (group.activeStateIndex >= 0) {
                m_combinedStyle->mergeFrom(styleClass->findStateStyle(group.stateNames[group.activeStateIndex]));
            }
        }

        clearDirty();
    }
    return m_combinedStyle;
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


} // namespace ln

