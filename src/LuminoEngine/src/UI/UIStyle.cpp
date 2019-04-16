
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
	tone = ToneF(0.0f, 0.0f, 0.0f, 0.0f);
}

//==============================================================================
// UIStyleInstance

namespace detail {

UIStyleInstance::UIStyleInstance()
{
}

void UIStyleInstance::init()
{
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
			outStyleData->font = newObject<Font>(desc);
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

} // namespace detail
} // namespace ln

