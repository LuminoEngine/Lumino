
#pragma once
#include <LuminoGraphics/Mesh/Common.hpp>
#include "../../Visual/VisualComponent.hpp"
#include "../VisualObject.hpp"

namespace ln {
class InstancedMeshList;


class InstancedSpritesModel : public Object
{
public:
    /** @copydoc InstancedSprites::setMaterial */
    void setMaterial(Material* value);
    /** @copydoc InstancedSprites::material */
    Material* material() const;
    /** @copydoc InstancedSprites::clearSprites */
    void clearSprites();
    /** @copydoc InstancedSprites::setSize */
    void setSize(const Size& value);
    /** @copydoc InstancedSprites::setAnchor */
    void setAnchor(const Vector2& value);
    /** @copydoc InstancedSprites::setFrameRect */
    void setFrameRect(const Rect& value);
    /** @copydoc InstancedSprites::setFrameIndex */
    void setFrameIndex(int value);
    /** @copydoc InstancedSprites::setFlipX */
    void setFlipX(bool value);
    /** @copydoc InstancedSprites::setFlipY */
    void setFlipY(bool value);
    /** @copydoc InstancedSprites::setBillboard */
    void setBillboard(BillboardType value);
    /** @copydoc InstancedSprites::setColor */
    void setColor(const Color& value);
    /** @copydoc InstancedSprites::setTransform */
    void setTransform(const Matrix& value);
    /** @copydoc InstancedSprites::addSprite */
    void addSprite(const Vector3& pos);

    void render(RenderingContext* context);
    
LN_CONSTRUCT_ACCESS:
    InstancedSpritesModel();
    virtual ~InstancedSpritesModel();
    bool init();

private:
    struct State
    {
        Size size;
        Vector2 anchor;
        Rect frameRect;
        int frameIndex;
        bool flipX;
        bool flipY;
        BillboardType billboard;
        Color color;
        Matrix transform;
        bool hasTransform;
    };

    struct Sprite
    {
        Matrix transform;   // size * anchor * transform * pos
        Vector2 uvScale;
        Vector2 uvOffset;
        Color color;
    };

    Ref<MeshPrimitive> m_mesh;
    Ref<InstancedMeshList> m_meshList;
    Ref<Material> m_material;
    State m_state;
    std::vector<Sprite> m_sprites;
};

class InstancedSpritesComponent
    : public VisualComponent
{
public:
    /** @copydoc InstancedSprites::setMaterial */
    void setMaterial(Material* value) { m_model->setMaterial(value); }
    /** @copydoc InstancedSprites::material */
    Material* material() const { m_model->material(); }
    /** @copydoc InstancedSprites::clearSprites */
    void clearSprites() { m_model->clearSprites(); }
    /** @copydoc InstancedSprites::setSize */
    void setSize(const Size& value) { m_model->setSize(value); }
    /** @copydoc InstancedSprites::setAnchor */
    void setAnchor(const Vector2& value) { m_model->setAnchor(value); }
    /** @copydoc InstancedSprites::setFrameRect */
    void setFrameRect(const Rect& value) { m_model->setFrameRect(value); }
    /** @copydoc InstancedSprites::setFrameIndex */
    void setFrameIndex(int value) { m_model->setFrameIndex(value); }
    /** @copydoc InstancedSprites::setFlipX */
    void setFlipX(bool value) { m_model->setFlipX(value); }
    /** @copydoc InstancedSprites::setFlipY */
    void setFlipY(bool value) { m_model->setFlipY(value); }
    /** @copydoc InstancedSprites::setBillboard */
    void setBillboard(BillboardType value) { m_model->setBillboard(value); }
    /** @copydoc InstancedSprites::setColor */
    void setColor(const Color& value) { m_model->setColor(value); }
    /** @copydoc InstancedSprites::setTransform */
    void setTransform(const Matrix& value) { m_model->setTransform(value); }
    /** @copydoc InstancedSprites::addSprite */
    void addSprite(const Vector3& pos) { m_model->addSprite(pos); }

    void setModel(InstancedSpritesModel* value);

    InstancedSpritesModel* model() const;

protected:
    void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    InstancedSpritesComponent();
    virtual ~InstancedSpritesComponent();
    bool init();

private:
    Ref<InstancedSpritesModel> m_model;
};

/**
 */
class InstancedSprites
	: public VisualObject
{
    LN_BUILDER;
public:
    /** マテリアルを設定します。 */
    void setMaterial(Material* value) { m_component->setMaterial(value); }

    /** マテリアルを取得します。 */
    Material* material() const { m_component->material(); }

    /** 追加されているスプライトをすべて削除し、ステートをリセットします。 */
    void clearSprites() { m_component->clearSprites(); }

    /** 以降に追加するスプライトのサイズを設定します。default: (1, 1) */
    void setSize(const Size& value) { m_component->setSize(value); }

    /** 以降に追加するスプライトのアンカーポイント (原点) を設定します。default: 中心(0.5, 0.5) */
    void setAnchor(const Vector2& value) { m_component->setAnchor(value); }

    /** 以降に追加するスプライトのテクスチャ転送矩形を設定します。(default: Empty(全体転送)) */
    void setFrameRect(const Rect& value) { m_component->setFrameRect(value); }

    /** 以降に追加するスプライトのフレームインデックスを設定します。デフォルトは -1 で、これはスプライトシートを使用しないことを示します。 */
    void setFrameIndex(int value) { m_component->setFrameIndex(value); }

    /** 以降に追加するスプライトの水平反転の有無を設定します。(default: false) */
    void setFlipX(bool value) { m_component->setFlipX(value); }

    /** 以降に追加するスプライトの垂直反転の有無を設定します。(default: false) */
    void setFlipY(bool value) { m_component->setFlipY(value); }

    /** 以降に追加するスプライトのビルボードモードを設定します。(default: None) */
    void setBillboard(BillboardType value) { m_component->setBillboard(value); }

    /** 以降に追加するスプライトのカラースケールを設定します。(default: White(1,1,1,1)) */
    void setColor(const Color& value) { m_component->setColor(value); }

    /** 以降に追加するスプライトの座標変換行列を設定します。(default: Identity) */
    void setTransform(const Matrix& value) { m_component->setTransform(value); }

    /** 指定した位置にスプライトを追加します。 */
    void addSprite(const Vector3& pos) { m_component->addSprite(pos); }

    InstancedSpritesComponent* instancedSpritesComponent() const;

LN_CONSTRUCT_ACCESS:
	InstancedSprites();
	virtual ~InstancedSprites();
	bool init();

private:
    Ref<InstancedSpritesComponent> m_component;
};

//==============================================================================
// InstancedSprites::Builder

struct InstancedSprites::BuilderDetails : public VisualObject::BuilderDetails
{
    LN_BUILDER_DETAILS(InstancedSprites);

    Ref<Material> material;

    BuilderDetails();
    void apply(InstancedSprites* p) const;
};

template<class T, class B, class D>
struct InstancedSprites::BuilderCore : public VisualObject::BuilderCore<T, B, D>
{
    LN_BUILDER_CORE(VisualObject::BuilderCore);

    B& material(Material* value) { d()->material = value; return self(); }
};

LN_BUILDER_IMPLEMENT(InstancedSprites);

} // namespace ln
