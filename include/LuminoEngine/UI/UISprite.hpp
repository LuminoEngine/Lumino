#pragma once
#include "UIElement.hpp"

namespace ln {
class Material;

class UISprite
    : public UIElement
{
public:
    static Ref<UISprite> create();
    static Ref<UISprite> create(Texture* texture);

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


    UISprite();
	void initialize();
    void initialize(Texture* texture);

protected:
    virtual void onRender(UIRenderingContext* context);

private:
    Ref<Material> m_material;
    Size m_size;
    Rect m_sourceRect;
};

} // namespace ln

