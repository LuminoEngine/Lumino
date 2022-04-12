#pragma once
#include <LuminoGraphics/Common.hpp>

namespace ln {

/** 3D 空間での基準方向を表す値 */
enum class SpriteBaseDirection2 {
    XPlus,   /**< X+ 方向 (右向き) */
    YPlus,   /**< Y+ 方向 (上向き) */
    ZPlus,   /**< Z+ 方向 (奥向き) */
    XMinus,  /**< X- 方向 (左向き) */
    YMinus,  /**< Y- 方向 (下向き) */
    ZMinus,  /**< Z- 方向 (手前向き) */
    Basic2D, /**< Z+ 方向、左上原点 */
};

/** ビルボードの計算方法 */
enum class BillboardType2 {
    None,          /**< ビルボードの計算を行わない */
    ToCameraPoint, /**< カメラ (ビュー行列) に対して正面を向く */
    ToScreen,      /**< スクリーン (ビュー平面) に対して正面を向く */
    RotY,          /**< Y 軸回転のみ行う */
};

enum class SpriteFlipFlags2 : uint8_t {
    None = 0,
    FlipX = 1 << 1,
    FlipY = 1 << 2,
};
LN_FLAGS_OPERATORS(SpriteFlipFlags2);

namespace detail {

class SpriteRenderer : public URefObject {
public:
    SpriteRenderer();
    void init();

    void begin();
    void end();
    void render(GraphicsCommandList* commandList);

    // srcRect は UV 座標系上の値を設定する。 (通常0～1)
    // 以前は 2D メインな Sprite なのでピクセル単位で指定していたが、
    // 考え方として他の RenderFeature と同様に「最終的な描画に使うメッシュを作る」方針で統一したい。
    void drawRequest(
        GraphicsCommandList* context,
        const Matrix& transform,
        const Vector2& size,
        const Vector2& anchorRatio,
        const Rect& srcRect,
        const Color& color,
        SpriteBaseDirection2 baseDirection,
        BillboardType2 billboardType,
        SpriteFlipFlags2 flipFlags);


private:
    // TODO: InstancedMeshList::InstanceData と統合したい
    struct InstanceData {
        Vector4 transform0;
        Vector4 transform1;
        Vector4 transform2;
        Vector4 transform3;
        Vector4 uvOffset; // .xy=pos, .zw=scale
        Vector4 colorScale;
    };


    struct BatchData {
        int spriteOffset;
        int spriteCount;
    };

    void prepareBuffers(GraphicsCommandList* context, int spriteCount);

    Matrix m_viewMatrix;
    Matrix m_projMatrix;

    // sprite-batching
    Ref<VertexLayout> m_vertexLayout;
    Ref<VertexBuffer> m_sprite;
    Ref<VertexBuffer> m_instanceBuffer;
    std::vector<InstanceData> m_instanceData;

    int m_buffersReservedSpriteCount;
    int32_t m_spriteCount;
};

} // namespace detail
} // namespace ln
