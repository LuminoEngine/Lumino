﻿
#include "Internal.hpp"
#include <LuminoFont/Font.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include "UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UIStyleInstance

namespace detail {

UIStyleInstance::UIStyleInstance() {
}

void UIStyleInstance::setupDefault() {
    //margin = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
    //padding = Thickness(0.0f, 0.0f, 0.0f, 0.0f);
    //hAlignment = UIHAlignment::Stretch; // Alignment は HTML のデフォルトに合わせてみる
    //vAlignment = UIVAlignment::Stretch;
    //horizontalContentAlignment = UIHAlignment::Stretch;
    //verticalContentAlignment = UIVAlignment::Stretch;
    //minWidth = Math::NaN;
    //minHeight = Math::NaN;
    //maxWidth = Math::NaN;
    //maxHeight = Math::NaN;
    overflowX = UIOverflowBehavior::Visible;
    overflowY = UIOverflowBehavior::Visible;

    position = Vector3::Zero;
    rotation = Quaternion::Identity;
    scale = Vector3::Ones;
    centerPoint = Vector3::Zero;

    backgroundDrawMode = Sprite9DrawMode::StretchedSingleImage;
    backgroundImageRect = Rect::Zero;
    backgroundImageBorder = Thickness::Zero;

    textColor = Color::Black;
    fontFamily = String::Empty;
    fontSize = 20.0f; // WPF default は 12 だが、それだとデスクトップアプリ向けなので少し小さい。Lumino としては 20 をデフォルトとする。
    fontWeight = UIFontWeight::Normal;
    fontStyle = UIFontStyle::Normal;

    visible = UIVisibility::Visible;
    blendMode = BlendMode::Alpha;

    opacity = 1.0f;
    colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
    blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
    tone = ColorTone(0.0f, 0.0f, 0.0f, 0.0f);

    //   auto tmp = makeObject<UIStyle>();
    //   tmp->setupDefault();
    // backgroundMaterial = makeObject<Material>();
    //   mergeFrom(tmp);
}

void UIStyleInstance::mergeFrom(const UIStyle* other) {
    if (LN_REQUIRE(other)) return;

    if (other->flexDirection.hasValue()) flexDirection = other->flexDirection.value();
    if (other->flexBasis.hasValue()) flexBasis = other->flexBasis.value();
    if (other->flexGrow.hasValue()) flexGrow = other->flexGrow.value();
    if (other->flexShrink.hasValue()) flexShrink = other->flexShrink.value();
    if (other->flexWrap.hasValue()) flexWrap = other->flexWrap.value();
    if (other->justifyContent.hasValue()) justifyContent = other->justifyContent.value();
    if (other->alignItems.hasValue()) alignItems = other->alignItems.value();
    if (other->alignSelf.hasValue()) alignSelf = other->alignSelf.value();
    if (other->alignContent.hasValue()) alignContent = other->alignContent.value();
    if (other->width.hasValue()) width = other->width.value();
    if (other->height.hasValue()) height = other->height.value();
    if (other->minWidth.hasValue()) minWidth = other->minWidth.value();
    if (other->minHeight.hasValue()) minHeight = other->minHeight.value();
    if (other->maxWidth.hasValue()) maxWidth = other->maxWidth.value();
    if (other->maxHeight.hasValue()) maxHeight = other->maxHeight.value();
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
    if (other->positionTop.hasValue()) positionTop = other->positionTop.value();
    if (other->positionRight.hasValue()) positionRight = other->positionRight.value();
    if (other->positionBottom.hasValue()) positionBottom = other->positionBottom.value();
    if (other->positionLeft.hasValue()) positionLeft = other->positionLeft.value();
    if (other->positionType.hasValue()) positionType = other->positionType.value();
    if (other->aspectRatio.hasValue()) aspectRatio = other->aspectRatio.value();
    if (other->zIndex.hasValue()) zIndex = other->zIndex.value();
    if (other->overflowX.hasValue()) overflowX = other->overflowX.get();
    if (other->overflowY.hasValue()) overflowY = other->overflowY.get();

    // layout transform
    if (other->origin.hasValue()) position = other->origin.get();
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
}

void UIStyleInstance::copyFrom(const UIStyleInstance* other) {
    if (LN_REQUIRE(other)) return;

    // layout
    flexDirection = other->flexDirection;
    flexBasis = other->flexBasis;
    flexGrow = other->flexGrow;
    flexShrink = other->flexShrink;
    flexWrap = other->flexWrap;
    justifyContent = other->justifyContent;
    alignItems = other->alignItems;
    alignSelf = other->alignSelf;
    alignContent = other->alignContent;
    width = other->width;
    height = other->height;
    minWidth = other->minWidth;
    minHeight = other->minHeight;
    maxWidth = other->maxWidth;
    maxHeight = other->maxHeight;
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
    positionTop = other->positionTop;
    positionRight = other->positionRight;
    positionBottom = other->positionBottom;
    positionLeft = other->positionLeft;
    positionType = other->positionType;
    aspectRatio = other->aspectRatio;
    zIndex = other->zIndex;
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

    // decorators
    for (auto& d : other->decorators) {
        decorators.add(d);
    }
}

void UIStyleInstance::makeRenderObjects() {
}

void UIStyleInstance::updateStyleDataHelper(const UIStyleContext* context, const detail::UIStyleInstance* parentStyleData, const UIStyle* combinedStyle, detail::UIStyleInstance* outStyleData) {
    // animations
    //	StartValue は最新のスタイルの値にする前の、現在値を使いたいので、animation 更新は最初に行う。
    {
        if (combinedStyle->m_animationData) {
            const auto& sd = combinedStyle->m_animationData;
            auto* d = outStyleData->acquireAnimationData();

            // if (!d->backgroundColor) {	// TODO: test
            if (sd->hasValue(UIStyleAnimationElement_BackgroundColor)) {
                if (UIVector4AnimationInstance::realloc(sd->backgroundColor, &d->backgroundColor)) {
                    d->backgroundColor->reset(outStyleData->backgroundColor.toVector4());
                }
            }
            else {
                // TODO: 消すとメモリ効率悪い。poolにする
                d->backgroundColor = nullptr;
            }

            if (sd->hasValue(UIStyleAnimationElement_Opacity)) {
                if (UIScalarAnimationInstance::realloc(sd->opacity, &d->opacity)) {
                    d->opacity->reset(outStyleData->opacity);
                }
            }
            else {
                // TODO: 消すとメモリ効率悪い。poolにする
                d->opacity = nullptr;
            }
        }
        else {
            // TODO: 消すとメモリ効率悪い。poolにする
            outStyleData->m_animationData = nullptr;
        }
    }

    // const UIStyle* parentStyle = (parentStyleData) ? parentStyleData->sourceLocalStyle : nullptr;
    // if (parentStyle)
    //{
    //	// text 関係は継承する
    //	localStyle->textColor.inherit(parentStyle->textColor);
    //	localStyle->fontFamily.inherit(parentStyle->fontFamily);
    //	localStyle->fontSize.inherit(parentStyle->fontSize);
    //	localStyle->fontWeight.inherit(parentStyle->fontWeight);
    //	localStyle->fontStyle.inherit(parentStyle->fontStyle);
    // }

    // inheritance
    {
        if (parentStyleData) {
            // 1. Local style
            // 2. Style sheet
            // 3. Parent
            outStyleData->textColor = (combinedStyle->textColor.getOrDefault(parentStyleData->textColor));
            outStyleData->fontFamily = (combinedStyle->fontFamily.getOrDefault(parentStyleData->fontFamily));
            outStyleData->fontSize = (combinedStyle->fontSize.getOrDefault(parentStyleData->fontSize));
            outStyleData->fontWeight = (combinedStyle->fontWeight.getOrDefault(parentStyleData->fontWeight));
            outStyleData->fontStyle = (combinedStyle->fontStyle.getOrDefault(parentStyleData->fontStyle));
        }
        else {
            // for root default style
            assert(combinedStyle->textColor.hasValue());
            assert(combinedStyle->fontFamily.hasValue());
            assert(combinedStyle->fontSize.hasValue());
            assert(combinedStyle->fontWeight.hasValue());
            assert(combinedStyle->fontStyle.hasValue());
            outStyleData->textColor = (combinedStyle->textColor.getOrDefault(UIStyle::DefaultTextColor));
            outStyleData->fontFamily = (combinedStyle->fontFamily.getOrDefault(UIStyle::DefaultFontFamily));
            outStyleData->fontSize = (combinedStyle->fontSize.getOrDefault(UIStyle::DefaultFontSize));
            outStyleData->fontWeight = (combinedStyle->fontWeight.getOrDefault(UIStyle::DefaultFontWeight));
            outStyleData->fontStyle = (combinedStyle->fontStyle.getOrDefault(UIStyle::DefaultFontStyle));
        }
    }

    // outStyleData->sourceLocalStyle = localStyle;

    // layout
    {
        outStyleData->width = (combinedStyle->width.valueOr(UIStyle::DefaultWidth));
        outStyleData->height = (combinedStyle->height.valueOr(UIStyle::DefaultHeight));

        //outStyleData->margin = (combinedStyle->margin.getOrDefault(UIStyle::DefaultMargin));
        //outStyleData->padding = (combinedStyle->padding.getOrDefault(UIStyle::DefaultPadding));
        //outStyleData->hAlignment = (combinedStyle->hAlignment.getOrDefault(UIStyle::DefaultHorizontalAlignment));
        //outStyleData->vAlignment = (combinedStyle->vAlignment.getOrDefault(UIStyle::DefaultVerticalAlignment));
        //outStyleData->horizontalContentAlignment = (combinedStyle->horizontalContentAlignment.getOrDefault(UIStyle::DefaultHorizontalContentAlignment));
        //outStyleData->verticalContentAlignment = (combinedStyle->verticalContentAlignment.getOrDefault(UIStyle::DefaultVerticalContentAlignment));

        outStyleData->marginTop = combinedStyle->marginTop.valueOr(UIStyleValue::Null);
        outStyleData->marginRight = combinedStyle->marginRight.valueOr(UIStyleValue::Null);
        outStyleData->marginBottom = combinedStyle->marginBottom.valueOr(UIStyleValue::Null);
        outStyleData->marginLeft = combinedStyle->marginLeft.valueOr(UIStyleValue::Null);

        outStyleData->borderTop = combinedStyle->borderTop.valueOr(UIStyleValue::Null);
        outStyleData->borderRight = combinedStyle->borderRight.valueOr(UIStyleValue::Null);
        outStyleData->borderBottom = combinedStyle->borderBottom.valueOr(UIStyleValue::Null);
        outStyleData->borderLeft = combinedStyle->borderLeft.valueOr(UIStyleValue::Null);

        outStyleData->paddingTop = combinedStyle->paddingTop.valueOr(UIStyleValue::Null);
        outStyleData->paddingRight = combinedStyle->paddingRight.valueOr(UIStyleValue::Null);
        outStyleData->paddingBottom = combinedStyle->paddingBottom.valueOr(UIStyleValue::Null);
        outStyleData->paddingLeft = combinedStyle->paddingLeft.valueOr(UIStyleValue::Null);

        outStyleData->positionTop = combinedStyle->positionTop.valueOr(UIStyleValue::Null);
        outStyleData->positionRight = combinedStyle->positionRight.valueOr(UIStyleValue::Null);
        outStyleData->positionBottom = combinedStyle->positionBottom.valueOr(UIStyleValue::Null);
        outStyleData->positionLeft = combinedStyle->positionLeft.valueOr(UIStyleValue::Null);

        outStyleData->positionType = combinedStyle->positionType.valueOr(UILayoutPositionType::Relative);
        outStyleData->aspectRatio = combinedStyle->aspectRatio.valueOr(UIStyleValue::Null);
        outStyleData->zIndex = combinedStyle->zIndex.valueOr(UIStyleValue::Null);

        outStyleData->minWidth = (combinedStyle->minWidth.valueOr(UIStyle::DefaultMinWidth));
        outStyleData->minHeight = (combinedStyle->minHeight.valueOr(UIStyle::DefaultMinHeight));
        outStyleData->maxWidth = (combinedStyle->maxWidth.valueOr(UIStyle::DefaultMaxWidth));
        outStyleData->maxHeight = (combinedStyle->maxHeight.valueOr(UIStyle::DefaultMaxHeight));
        outStyleData->overflowX = (combinedStyle->overflowX.getOrDefault(UIStyle::DefaultOverflowX));
        outStyleData->overflowY = (combinedStyle->overflowY.getOrDefault(UIStyle::DefaultOverflowY));
    }

    // layout transform
    {
        outStyleData->position = (combinedStyle->origin.getOrDefault(UIStyle::DefaultOrigin));
        outStyleData->rotation = (combinedStyle->rotation.getOrDefault(UIStyle::DefaultRotation));
        outStyleData->scale = (combinedStyle->scale.getOrDefault(UIStyle::DefaultScale));
        outStyleData->centerPoint = (combinedStyle->centerPoint.getOrDefault(UIStyle::DefaultCenterPoint));
    }

    // background
    {
        assert(outStyleData->backgroundMaterial);
        outStyleData->backgroundDrawMode = (combinedStyle->backgroundDrawMode.getOrDefault(UIStyle::DefaultBackgroundDrawMode));
        outStyleData->backgroundColor = (combinedStyle->backgroundColor.getOrDefault(UIStyle::DefaultBackgroundColor));
        outStyleData->backgroundMaterial->setMainTexture((combinedStyle->backgroundImage.getOrDefault(UIStyle::DefaultBackgroundImage)));
        outStyleData->backgroundMaterial->setShader((combinedStyle->backgroundShader.getOrDefault(UIStyle::DefaultBackgroundShader)));
        outStyleData->backgroundImageRect = (combinedStyle->backgroundImageRect.getOrDefault(UIStyle::DefaultBackgroundImageRect));
        outStyleData->backgroundImageBorder = (combinedStyle->backgroundImageBorder.getOrDefault(UIStyle::DefaultBackgroundImageBorder));
    }

    // border
    {
        outStyleData->borderThickness = (combinedStyle->borderThickness.getOrDefault(UIStyle::DefaultBorderThickness));
        outStyleData->cornerRadius = (combinedStyle->cornerRadius.getOrDefault(UIStyle::DefaultCornerRadius));
        outStyleData->leftBorderColor = (combinedStyle->leftBorderColor.getOrDefault(UIStyle::DefaultLeftBorderColor));
        outStyleData->topBorderColor = (combinedStyle->topBorderColor.getOrDefault(UIStyle::DefaultTopBorderColor));
        outStyleData->rightBorderColor = (combinedStyle->rightBorderColor.getOrDefault(UIStyle::DefaultRightBorderColor));
        outStyleData->bottomBorderColor = (combinedStyle->bottomBorderColor.getOrDefault(UIStyle::DefaultBottomBorderColor));
        outStyleData->borderDirection = (combinedStyle->borderDirection.getOrDefault(UIStyle::DefaultBorderDirection));
        outStyleData->borderInset = (combinedStyle->borderInset.getOrDefault(UIStyle::DefaultBorderInset));
    }

    // shadow
    {
        outStyleData->shadowOffsetX = (combinedStyle->shadowOffsetX.getOrDefault(UIStyle::DefaultShadowOffsetX));
        outStyleData->shadowOffsetY = (combinedStyle->shadowOffsetY.getOrDefault(UIStyle::DefaultShadowOffsetY));
        outStyleData->shadowBlurRadius = (combinedStyle->shadowBlurRadius.getOrDefault(UIStyle::DefaultShadowBlurRadius));
        outStyleData->shadowSpreadRadius = (combinedStyle->shadowSpreadRadius.getOrDefault(UIStyle::DefaultShadowSpreadRadius));
        outStyleData->shadowColor = (combinedStyle->shadowColor.getOrDefault(UIStyle::DefaultShadowColor));
        outStyleData->shadowInset = (combinedStyle->shadowInset.getOrDefault(UIStyle::DefaultShadowInset));
    }

    // text
    {
        // outStyleData->textColor = localStyle->textColor.getOrDefault(combinedStyle->textColor.get());

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
        outStyleData->visible = (combinedStyle->visible.getOrDefault(UIStyle::DefaultVisible));
        outStyleData->blendMode = (combinedStyle->blendMode.getOrDefault(UIStyle::DefaultBlendMode));
        outStyleData->opacity = (combinedStyle->opacity.getOrDefault(UIStyle::DefaultOpacity));
        outStyleData->colorScale = (combinedStyle->colorScale.getOrDefault(UIStyle::DefaultColorScale));
        outStyleData->blendColor = (combinedStyle->blendColor.getOrDefault(UIStyle::DefaultBlendColor));
        outStyleData->tone = (combinedStyle->tone.getOrDefault(UIStyle::DefaultTone));
    }

    // decorators
    outStyleData->decorators.clear();
    // for (auto& d : localStyle->decorators) {
    //	outStyleData->decorators.add(d);
    // }
    for (auto& d : combinedStyle->decorators) {
        outStyleData->decorators.add(d);
    }
}

void UIStyleInstance::updateAnimationData() {
}

void UIStyleInstance::advanceAnimation(float elapsedTime) {
    if (m_animationData) {
        AnimationData* d = m_animationData.get();
        if (d->width) d->width->advanceTime(elapsedTime);
        if (d->height) d->height->advanceTime(elapsedTime);
        if (d->positionX) d->positionX->advanceTime(elapsedTime);
        if (d->positionY) d->positionY->advanceTime(elapsedTime);
        if (d->positionZ) d->positionZ->advanceTime(elapsedTime);
        if (d->backgroundColor) d->backgroundColor->advanceTime(elapsedTime);
        if (d->opacity) d->opacity->advanceTime(elapsedTime);
    }
}

template<typename T, typename TValue>
static UIElementDirtyFlags applyAnimation(const T& instance, TValue* value, UIElementDirtyFlags flag) {
    if (instance /* && instance->isActive()*/) { // TODO: 今は毎フレーム style の reset してるので、アニメ適用しないと初期値になってしまう
        *value = instance->evaluate();
        return flag;
    }
    else {
        return UIElementDirtyFlags::None;
    }
}

UIElementDirtyFlags UIStyleInstance::applyAnimationValues() {
    Flags<UIElementDirtyFlags> dirty = UIElementDirtyFlags::None;

    if (m_animationData) {
        AnimationData* d = m_animationData.get();
        dirty |= applyAnimation(d->width, &width, UIElementDirtyFlags::Layout);
        dirty |= applyAnimation(d->height, &height, UIElementDirtyFlags::Layout);
        dirty |= applyAnimation(d->positionX, &position.x, UIElementDirtyFlags::Layout);
        dirty |= applyAnimation(d->positionY, &position.y, UIElementDirtyFlags::Layout);
        dirty |= applyAnimation(d->positionZ, &position.z, UIElementDirtyFlags::Layout);
        dirty |= applyAnimation(d->backgroundColor, &backgroundColor, UIElementDirtyFlags::Render);
        dirty |= applyAnimation(d->opacity, &opacity, UIElementDirtyFlags::Render);
        // dirty |= applyAnimation(d->opacity, &builtinEffect.opacity, UIElementDirtyFlags::Render);
    }

    return dirty;
}

//==============================================================================
// UIStyleClassInstance

UIStyleClassInstance::UIStyleClassInstance()
    : m_style(makeRef<UIStyleInstance>()) {
}

UIStyleInstance* UIStyleClassInstance::findStateStyle(const StringView& stateName) const {
    auto style = m_visualStateStyles.findIf([&](auto& x) { return String::compare(x.name, stateName, CaseSensitivity::CaseInsensitive); });
    if (style)
        return style->style;
    else
        return m_style;
}

UIStyleInstance* UIStyleClassInstance::findSubElementStyle(const StringView& elementName) const {
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
} // namespace ln
