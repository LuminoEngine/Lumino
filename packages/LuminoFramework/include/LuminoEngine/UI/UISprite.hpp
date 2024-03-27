#pragma once
#include "UIElement.hpp"

namespace ln {

/**
 * UI 要素として 2D 空間上に表示されるスプライトオブジェクトを表します。
 */
LN_CLASS()
class UISprite
    : public UIElement
{
public:
    static Ref<UISprite> create();
    static Ref<UISprite> create(Texture* texture);

	static Ref<UISprite> load(StringView filePath);

    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);
    
	/** テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。 */
	LN_METHOD(Property)
	void setSourceRect(const Rect& rect);

	/** @overload setSourceRect */
	LN_METHOD(OverloadPostfix = "XYWH")
	void setSourceRect(float x, float y, float width, float height);

	/** テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位) */
	LN_METHOD(Property)
	const Rect& sourceRect() const;

    /** setShader */
    LN_METHOD(Property)
    void setShader(Shader* shader);

LN_CONSTRUCT_ACCESS:
    UISprite();

    /** UISprite を作成します。 */
    LN_METHOD()
	void init();

	/** テクスチャを指定して UISprite を作成します。 */
	LN_METHOD(OverloadPostfix = "WithTexture")
    void init(Texture* texture);

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual void onRender(UIRenderingContext* context);

private:
    Ref<Material> m_material;
    Size m_size;
    Rect m_sourceRect;
};

} // namespace ln

