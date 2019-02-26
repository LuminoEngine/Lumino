
#include "Internal.hpp"
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
	horizontalAlignment = HAlignment::Center;
	verticalAlignment = VAlignment::Center;
	minWidth = 0.0f;
	minHeight = 0.0f;
	maxWidth = 0.0f;
	maxHeight = 0.0f;

	position = Vector3::Zero;
	rotation = Quaternion::Identity;
	scale = Vector3::Ones;
	centerPoint = Vector3::Zero;

	textColor = Color::Black;
	fontFamily = String::Empty;
	fontSize = 12.0f;	// WPF default
	fontWeight = UIFontWeight::Normal;
	fontStyle = UIFontStyle::Normal;

	visible = true;
	blendMode = BlendMode::Alpha;

	opacity = 1.0f;
	colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
	blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
	tone = ToneF(0.0f, 0.0f, 0.0f, 0.0f);
}

} // namespace ln

