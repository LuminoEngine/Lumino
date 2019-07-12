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
}

void UIStyleClass::addStateStyle(const StringRef& stateName, UIStyle* style)
{
    if (LN_REQUIRE(style)) return;
    m_visualStateStyles.add({ stateName, style });
}

UIStyle* UIStyleClass::findStateStyle(const StringRef& stateName) const
{
    auto style = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, stateName, CaseSensitivity::CaseInsensitive); });
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
    auto style = m_subElements.findIf([&](auto& x) { return String::compare(x.name, elementName, CaseSensitivity::CaseInsensitive); });
    if (style)
        return style->style;
    else
        return nullptr;
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

void UIStyleContext::addStyleClass(UIStyleSheet* sheet)
{
    if (LN_REQUIRE(sheet)) return;
    m_styleSheets.add(sheet);
}

void UIStyleContext::build()
{
    // first, merge globals
    m_globalStyle = makeRef<detail::UIStyleClassInstance>();
    m_globalStyle->m_style->setupDefault();
    for (auto& sheet : m_styleSheets) {
        auto globalStyle = sheet->findStyleClass(u"*");
        if (globalStyle) {
            m_globalStyle->mergeFrom(globalStyle);
        }
    }

    // second, merge other styles
    for (auto& sheet : m_styleSheets) {
        for (auto& pair : sheet->m_classes) {
            auto styleClass = findStyleClass(pair.first);
            if (!styleClass) {
                auto instance = makeRef<detail::UIStyleClassInstance>();
                m_classes.insert({ pair.first, instance });
                styleClass = instance;
            }
            styleClass->mergeFrom(pair.second);
        }
    }
}

detail::UIStyleClassInstance* UIStyleContext::findStyleClass(const StringRef& className) const
{
    auto itr = m_classes.find(className);
    if (itr != m_classes.end())
        return itr->second;
    else
        return nullptr;
}

//==============================================================================
// UIStyleInstance

namespace detail {

UIStyleInstance::UIStyleInstance()
{
}

void UIStyleInstance::setupDefault()
{
    auto tmp = makeObject<UIStyle>();
    tmp->setupDefault();
	backgroundMaterial = makeObject<Material>();
    mergeFrom(tmp);
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

void UIStyleInstance::updateStyleDataHelper(UIStyle* localStyle, const detail::UIStyleInstance* parentStyleData, const UIStyle* defaultStyle, detail::UIStyleInstance* outStyleData)
{
	const UIStyle* parentStyle = (parentStyleData) ? parentStyleData->sourceLocalStyle : nullptr;
	if (parentStyle)
	{
		// text 関係は継承する
		localStyle->textColor.inherit(parentStyle->textColor);
		localStyle->fontFamily.inherit(parentStyle->fontFamily);
		localStyle->fontSize.inherit(parentStyle->fontSize);
		localStyle->fontWeight.inherit(parentStyle->fontWeight);
		localStyle->fontStyle.inherit(parentStyle->fontStyle);
	}

	outStyleData->sourceLocalStyle = localStyle;


	// layout
	{
		outStyleData->margin = localStyle->margin.getOrDefault(defaultStyle->margin.get());
		outStyleData->padding = localStyle->padding.getOrDefault(defaultStyle->padding.get());
		outStyleData->horizontalAlignment = localStyle->horizontalAlignment.getOrDefault(defaultStyle->horizontalAlignment.get());
		outStyleData->verticalAlignment = localStyle->verticalAlignment.getOrDefault(defaultStyle->verticalAlignment.get());
		outStyleData->minWidth = localStyle->minWidth.getOrDefault(defaultStyle->minWidth.get());//.getOrDefault(Math::NaN));
		outStyleData->minHeight = localStyle->minHeight.getOrDefault(defaultStyle->minHeight.get());//.getOrDefault(Math::NaN));
		outStyleData->maxWidth = localStyle->maxWidth.getOrDefault(defaultStyle->maxWidth.get());//.getOrDefault(Math::NaN));
		outStyleData->maxHeight = localStyle->maxHeight.getOrDefault(defaultStyle->maxHeight.get());//.getOrDefault(Math::NaN));
	}

	// layout transform
	{
		outStyleData->position = localStyle->position.getOrDefault(defaultStyle->position.get());
		outStyleData->rotation = localStyle->rotation.getOrDefault(defaultStyle->rotation.get());
		outStyleData->scale = localStyle->scale.getOrDefault(defaultStyle->scale.get());
		outStyleData->centerPoint = localStyle->centerPoint.getOrDefault(defaultStyle->centerPoint.get());
	}

	// background
	{
		assert(outStyleData->backgroundMaterial);
        outStyleData->backgroundDrawMode = localStyle->backgroundDrawMode.getOrDefault(defaultStyle->backgroundDrawMode.get());
		outStyleData->backgroundColor = localStyle->backgroundColor.getOrDefault(defaultStyle->backgroundColor.get());
		outStyleData->backgroundMaterial->setMainTexture(localStyle->backgroundImage.getOrDefault(defaultStyle->backgroundImage.get()));
		outStyleData->backgroundMaterial->setShader(localStyle->backgroundShader.getOrDefault(defaultStyle->backgroundShader.get()));
		//outStyleData->backgroundColor = localStyle->backgroundColor.getOrDefault(defaultStyle->backgroundColor.get());
		//outStyleData->backgroundImage = 
        outStyleData->backgroundImageRect = localStyle->backgroundImageRect.getOrDefault(defaultStyle->backgroundImageRect.get());
        outStyleData->backgroundImageBorder = localStyle->backgroundImageBorder.getOrDefault(defaultStyle->backgroundImageBorder.get());
	}

	// text
	{
		outStyleData->textColor = localStyle->textColor.getOrDefault(defaultStyle->textColor.get());

		if (outStyleData->font) {
			// 前回の update で選択した font があるのでそのままにする
		}
		else {
			// element 作成直後。まだ font の選択処理を行っていないときはとりあえず親のフォントを参照
			outStyleData->font = (parentStyleData) ? parentStyleData->font : nullptr;
		}

		detail::FontDesc desc;
		desc.Family = localStyle->fontFamily.get();
		desc.Size = localStyle->fontSize.get();
		desc.isBold = (localStyle->fontWeight.get() == UIFontWeight::Bold);
		desc.isItalic = (localStyle->fontStyle.get() == UIFontStyle::Italic);
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
		outStyleData->visible = localStyle->visible.getOrDefault(defaultStyle->visible.get());
		outStyleData->blendMode = localStyle->blendMode.getOrDefault(defaultStyle->blendMode.get());

		outStyleData->opacity = localStyle->opacity.getOrDefault(defaultStyle->opacity.get());
		outStyleData->colorScale = localStyle->colorScale.getOrDefault(defaultStyle->colorScale.get());
		outStyleData->blendColor = localStyle->blendColor.getOrDefault(defaultStyle->blendColor.get());
		outStyleData->tone = localStyle->tone.getOrDefault(defaultStyle->tone.get());
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
        return nullptr;
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
    for (auto& slot : other->m_visualStateStyles) {
        auto style = findStateStyle(slot.name);
        if (!style) {
            m_visualStateStyles.add({ slot.name, makeRef<UIStyleInstance>() });
            style = m_visualStateStyles.back().style;
        }
        style->mergeFrom(slot.style);
    }

    // Merge sub-element style
    for (auto& slot : other->m_subElements) {
        auto style = findSubElementStyle(slot.name);
        if (!style) {
            m_subElements.add({ slot.name, makeRef<UIStyleInstance>() });
            style = m_subElements.back().style;
        }
        style->mergeFrom(slot.style);
    }
}

} // namespace detail
} // namespace ln

