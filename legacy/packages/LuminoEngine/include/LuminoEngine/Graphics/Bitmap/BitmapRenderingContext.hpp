#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/Font/Common.hpp>
#include "Common.hpp"

namespace ln {

/**
 * ビットマップの描画を行うためのクラスです。
 *
 * このクラスは CPU で描画を行います。 (GPU は使いません)
 * テクスチャに事前にテキストを描画したり、他フレームワークとの相互運用のためのビットマップ転送などに使います。
 */
class BitmapRenderingContext : public Object {
    // NOTE: Naming
    //   BitmapRenderer → Renerer というサフィックスはシーンレンダリングで GPU を使うものとしてたくさん使っているので避けたい。
    //   BitmapPainter  → Qt の QPainter のようなニュアンスで、ハイレベルのドローイングに使うものと言ったイメージがある。
    //                     このクラスはもっとローレベルなものであり、クラスコメントに書いたような限定的な用途で使いたい。
    //                     それにもしパスレンダリングなどまでやりたいとしたら、それは RenderTarget を作ってそこへ GPU でレンダリングしたい。
public:
public:
    void drawText(
        const StringView& text,
        const Rect& rect,
        Font* font,
        const Color& color,
        TextAlignment alignment = TextAlignment::Forward,
        TextDrawMode drawing = TextDrawMode::Fill,
        float strokeWidth = 0.0f
    );

private:
    BitmapRenderingContext();
    ~BitmapRenderingContext() override;
    MaybeResult_deprecated init(Texture2D* texture);
    Bitmap2D* getBitmap();

    Texture2D* m_texture;

    friend class Texture2D;
};

} // namespace ln
