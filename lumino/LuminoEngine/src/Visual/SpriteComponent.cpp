
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Reflection/Property.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include "../../../RuntimeCore/src/Asset/AssetManager.hpp"
#include "../Rendering/RenderFeature/SpriteRenderFeature.hpp"

namespace ln {

//=============================================================================
// SpriteFrame

LN_OBJECT_IMPLEMENT(SpriteFrame, Object) {}
//{
//	context->registerType<SpriteFrame>({
//		makeRef<PropertyInfo>("SourceRect", LN_MAKE_GET_SET_PROPERTY_ACCESSOR(SpriteFrame, Rect, sourceRect, setSourceRect)),
//		makeRef<PropertyInfo>("AnchorPoint", LN_MAKE_GET_SET_PROPERTY_ACCESSOR(SpriteFrame, Vector2, anchorPoint, setAnchorPoint)),
//	});
//}

SpriteFrame::SpriteFrame()
	: m_sourceRect()
	, m_anchorPoint()
{
}

void SpriteFrame::init()
{
	Object::init();
	m_anchorPoint = Vector2(Math::NaN, Math::NaN);
}

void SpriteFrame::serialize(Serializer2& ar)
{
    Object::serialize(ar);
    ar & makeNVP(_TT("SourceRect"), m_sourceRect);
    ar & makeNVP(_TT("AnchorPoint"), m_anchorPoint);
}

//=============================================================================
// SpriteSheet
/*
  グリッド分割は ピクセル指定？分割数指定？
  ----------
  Unity は両方可能。
  Tiled はピクセル指定。
  ポイントは変更の容易さだけど、
  - 分割数指定はテクスチャ解像度の変更に強い。
  - ピクセル指定は row を増やすことでのパターン追加に強い。
  SpriteSheet を使うのはドット絵がほとんど。どっちがよくある話かっていうと後者の方が圧倒的に多いだろう。
*/
LN_OBJECT_IMPLEMENT(SpriteSheet, Object) {}

Ref<SpriteSheet> SpriteSheet::load(StringRef path)
{
    auto assetModel = detail::AssetManager::instance()->loadAssetModelFromLocalFile(path);
    if (auto obj = (assetModel) ? assetModel->targetAs<SpriteSheet>() : nullptr)
        return obj;
    else
        return nullptr;
}

Ref<SpriteSheet> SpriteSheet::create(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint)
{
	return makeObject<SpriteSheet>(texture, frameWidth, frameHeight, anchorPoint);
}

SpriteSheet::SpriteSheet()
    : m_frames()
{
}

void SpriteSheet::init()
{
	Object::init();
}

void SpriteSheet::init(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint)
{
	init();

	if (LN_REQUIRE(texture)) return;
	if (LN_REQUIRE(frameWidth > 0)) return;
	if (LN_REQUIRE(frameHeight > 0)) return;
	m_texture = texture;
    m_frameWidth = frameWidth;
    m_frameHeight = frameHeight;
    m_anchorPoint = anchorPoint;

    splitFrames();
}

Texture* SpriteSheet::texture() const
{
    return m_texture;
}

SpriteFrame* SpriteSheet::frame(int index) const
{
	if (0 <= index && index < m_frames.size()) {
		return m_frames[index];
	}
	else {
		return nullptr;
	}
}

void SpriteSheet::clear()
{
    m_frameWidth = 0;
    m_frameHeight = 0;
    m_anchorPoint = Vector2::NaN;
    m_texture = nullptr;
    m_frames.clear();
}

void SpriteSheet::splitFrames()
{
    int cols = m_texture->width() / m_frameWidth;
    int rows = m_texture->height() / m_frameHeight;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            // TODO: モノによっては大量の小オブジェクトができるので、できればまとめて alloc したりキャッシュしたい
            auto frame = makeObject<SpriteFrame>();
            frame->setSourceRect(Rect(x * m_frameWidth, y * m_frameHeight, m_frameWidth, m_frameHeight));
            frame->setAnchorPoint(m_anchorPoint);
            m_frames.add(frame);
        }
    }
}

void SpriteSheet::serialize(Serializer2& ar)
{
    Object::serialize(ar);
    if (ar.isSaving()) {
        ar & makeNVP(_TT("texture"), m_texture);
        if (m_frameWidth != 0 && m_frameHeight != 0) {
            ar & makeNVP(_TT("frameWidth"), m_frameWidth);
            ar & makeNVP(_TT("frameHeight"), m_frameHeight);
            ar & makeNVP(_TT("anchorPoint"), m_anchorPoint);
        }
        else {
            ar & makeNVP(_TT("frames"), m_frames);
        }
    }
    else {
        clear();
        ar & makeNVP(_TT("texture"), m_texture);
        ar & makeNVP(_TT("frameWidth"), m_frameWidth);
        ar & makeNVP(_TT("frameHeight"), m_frameHeight);
        ar & makeNVP(_TT("anchorPoint"), m_anchorPoint);
        if (m_frameWidth != 0 && m_frameHeight != 0) {
            splitFrames();
        }
        else {
            ar & makeNVP(_TT("frames"), m_frames);
        }
    }
}

//=============================================================================
// SpriteComponent
/*
 * 正面方向について
 * --------
 * Luminoは 奥方向を正面としている。なら Sprite も面方向と法線はデフォルトで Z+ を向いた方がよいのでは？
 * → Sprite は普通、常にカメラ方向を向くビルボードとして使われることを想定している。
 *   まぁ、3D 空間に看板オブジェクトみたいに板ポリ置くのに使えないことはないけれど、
 *   それってマップオブジェクトを表現するための Mesh としたほうがいいよね。
 */

LN_OBJECT_IMPLEMENT(SpriteComponent, VisualComponent) {}

SpriteComponent::SpriteComponent()
    : m_material(nullptr)
    , m_size(-1.0f, -1.0f)
    , m_anchorPoint(0.5, 0.5)
	, m_frameIndex(0)
    , m_flipFlags(detail::SpriteFlipFlags::None)
    , m_pixelsParUnit(0.0f)
    , m_billboardType(BillboardType::None)
{
}

SpriteComponent::~SpriteComponent()
{
}

bool SpriteComponent::init()
{
    if (!VisualComponent::init()) return false;
    m_sourceRect.set(0, 0, -1, -1);
    //setSize(Size(1, 1));

    m_material = makeObject<Material>();
    //m_material->setEmissive(Color(1,1,1,0.5));
    setBlendMode(BlendMode::Alpha);
    setCullMode(CullMode::None);
	setShadingModel(ShadingModel::Unlit);
    return true;
}

void SpriteComponent::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
}

Texture* SpriteComponent::texture() const
{
    return m_material->mainTexture();
}

void SpriteComponent::setSize(const Size& size)
{
    m_size = size;
}

void SpriteComponent::setSourceRect(const Rect& rect)
{
    m_sourceRect = rect;
}

void SpriteComponent::setFrameSet(SpriteSheet* value)
{
	m_frameSet = value;
    m_material->setMainTexture(m_frameSet->texture());
}

void SpriteComponent::setFrameIndex(int index)
{
	m_frameIndex = index;
}

SpriteSheet* SpriteComponent::spriteSheet() const
{
    return m_frameSet;
}

void SpriteComponent::onRender(RenderingContext* context)
{
#if 1
	// Find anchor and sourceRect. SpriteSheet priority.
	Vector2 anchorPoint = m_anchorPoint;
	Rect actualSourceRect = m_sourceRect;
	Texture* actualTexture = texture();
	if (m_frameSet) {
		if (SpriteFrame* frame = m_frameSet->frame(m_frameIndex)) {
			actualSourceRect = frame->sourceRect();
			anchorPoint = frame->anchorPoint();
			actualTexture = m_frameSet->texture();

			anchorPoint.x = (Math::isNaN(anchorPoint.x)) ? m_anchorPoint.x : anchorPoint.x;
			anchorPoint.y = (Math::isNaN(anchorPoint.y)) ? m_anchorPoint.y : anchorPoint.y;
		}
	}
	Size actualTextureSize = (actualTexture) ? Size(actualTexture->width(), actualTexture->height()) : Size::Zero;
	if (actualSourceRect.width < 0 && actualSourceRect.height < 0) {
		// Source-rect undefined. use texture size.
		actualSourceRect.width = actualTextureSize.width;
		actualSourceRect.height = actualTextureSize.height;
	}

	if (actualSourceRect.isSquashed()) return;

	// Calculate actual size.
	Size actualSize = m_size;
	{
		if (actualSize.width < 0.0f && actualSize.height < 0.0f) {
			// Both is undefined.
			if (m_pixelsParUnit > 0.0f) {
				actualSize.width = actualSourceRect.width / m_pixelsParUnit;
				actualSize.height = actualSourceRect.height / m_pixelsParUnit;
			}
			else {
				// Height=1, Width=aspect.
				actualSize.height = 1.0f;
				actualSize.width = (actualSourceRect.width / actualSourceRect.height);
			}
		}
		else if (actualSize.width < 0.0f) {
			// Width is undefined.
			actualSize.width = actualSize.height * (actualSourceRect.width / actualSourceRect.height);
		}
		else if (actualSize.height < 0.0f) {
			// Height is undefined.
			actualSize.height = actualSize.width * (actualSourceRect.height / actualSourceRect.width);
		}

		if (actualSize.width < 0.0f) {
			actualSize.width = 1.0f;
		}
		if (actualSize.height < 0.0f) {
			actualSize.height = 1.0f;
		}

		// be set 0 directly by user program.
		if (actualSize.isSquashed()) return;
	}

	Rect uvRect(
		actualSourceRect.x / actualTextureSize.width,
		actualSourceRect.y / actualTextureSize.height,
		actualSourceRect.width / actualTextureSize.width,
		actualSourceRect.height / actualTextureSize.height);

#ifdef LN_COORD_RH
    const SpriteBaseDirection frontDir = SpriteBaseDirection::ZPlus;
#else
    const SpriteBaseDirection frontDir = SpriteBaseDirection::ZMinus;
#endif

	context->drawSprite(
		Matrix(), actualSize, anchorPoint, uvRect, Color::White,
        frontDir, m_billboardType, m_flipFlags, m_material);

#else
    Vector2 anchorPoint = m_anchorPoint;
	Rect sourceRect = m_sourceRect;
    Texture* tex = texture();

	if (sourceRect.isEmpty() && m_frameSet) {
		if (SpriteFrame* frame = m_frameSet->frame(m_frameIndex)) {
			sourceRect = frame->sourceRect();
            anchorPoint = frame->anchorPoint();
            tex = m_frameSet->texture();

            anchorPoint.x = (Math::isNaN(anchorPoint.x)) ? m_anchorPoint.x : anchorPoint.x;
            anchorPoint.y = (Math::isNaN(anchorPoint.y)) ? m_anchorPoint.y : anchorPoint.y;
		}
	}

    Size size = m_size;
    if (m_pixelsParUnit > 0.0f) {
		if (sourceRect.width > 0.0f && sourceRect.height > 0.0f) {
			size.width = sourceRect.width / m_pixelsParUnit;
			size.height = sourceRect.height / m_pixelsParUnit;
		}
    }

    Size renderSize;
    Rect renderSourceRect;
    detail::SpriteRenderFeature2::makeRenderSizeAndSourceRectHelper(
        tex, size, sourceRect, &renderSize, &renderSourceRect);

    context->drawSprite(
        Matrix(), renderSize, anchorPoint, renderSourceRect, Color::White,
        SpriteBaseDirection::ZMinus, BillboardType::None, m_flipFlags, m_material);
#endif
}
//
//void SpriteComponent::serialize(Archive& ar)
//{
//    VisualComponent::serialize(ar);
//    ar & makeNVP(_TT("material", m_material);
//}

void SpriteComponent::serialize(Serializer2& ar)
{
    VisualComponent::serialize(ar);
    ar & makeNVP(_TT("material"), m_material);
}

} // namespace ln

