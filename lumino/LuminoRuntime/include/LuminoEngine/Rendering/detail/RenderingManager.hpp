#pragma once
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Rendering/Common.hpp>
#include <LuminoEngine/Rendering/Kanata/Common.hpp>

namespace ln {
class VertexLayout;
class Shader;
class RenderFeature;

namespace detail {
class IVertexDeclaration;
class LinearAllocatorPageManager;
class DrawElementListBuilder;
class ClearRenderFeature;
class BlitRenderFeature;
class SpriteRenderFeature2;
class MeshRenderFeature;
class MeshGeneraterRenderFeature;
class PrimitiveRenderFeature;
class SpriteTextRenderFeature;
class FrameRectRenderFeature;
class ShapesRenderFeature;
class ShapesRenderFeature2;
class PathRenderFeature;
class ExtensionRenderFeature;
class RenderingProfiler;
class FontManager;

enum class BuiltinShader {
    CopyScreen,
    Sprite,
    ClusteredShadingDefault,
    ForwardGBufferPrepass,
    ShadowCaster,
    // LegacyDiffuse,		// Lambert Shading

    BlackShader,

    InfinitePlaneGrid,

    // for Sky
    SkyLowAltitudeOptimized,
    SkyDome,

    ScreenBlurPostEffect,
    TonePostEffect,

    // for Bloom
    LuminosityHighPassShader,
    SeperableBlur,
    BloomComposite,

    // for SSR
    SSRRayTracing,
    SSRBlur,
    SSRComposite,

    // for LightShaft
    RadialBlur,

    SSAOOcclusionMap,
    FilmicPostEffect,
    Copy, // TODO: CopyScreen と統合でいいかも

    TransitionEffectWithoutMask,
    TransitionEffectWithMask,

    NanoVG,

    BlendShape,

    MToon,
    ImGui,
};

enum class BuiltinMaterial {
    Default,
    Unlit,
};

/*
 * RenderingManager
 *
 * 主要モジュールについて
 * --------
 * ### DrawElement 構築
 * DrawElement は描画ジオメトリの最小単位。「何を描画するか？」を表す。
 * - DrawElement
 * - RenderStage
 * - DrawElementList
 *
 *
 *
 * 主要クラスについて
 * --------
 *
 * ### RenderStage
 * DrawElement に紐づく State を持っておくクラス。
 * 複数の DrawElement から参照されることが多い。
 * SceneNode 1つ分と考えてもよい。
 * 基本的にどんなタイミングでも、「RenderStage を描画」すれば同じジオメトリが表示される。
 * Unity でいうところの CommandBuffer に似て見えるが違うものなので注意。
 * Unity の CommandBuffer は SetRenderTarget → DrawMesh → SetRenderTarget → DrawMesh のようにステート変更と描画を混在できるが、http://tips.hecomi.com/entry/2016/02/19/090000
 * RenderStage はあくまでステートとジオメトリの塊なので↑のようには使うものではない。
 *
 * ### RenderingContext
 * ユーザーが何か直接描画したいときに主に使うことになるクラス。
 * Unity でいうところの Graphics クラス。
 * コマンドリストを構築する。
 * ※描画自体はコマンドリストがあればいいので RenderingContext はなくてもいいが、手動でコマンドリストを作るのは非常に大変なので、ある意味ユーティリティ的な立ち位置のクラス。
 *
 * ### SceneRenderer
 * コマンドリスト (RenderStage の集合) をどのように描画するかを決めるクラス。
 * 3D なら ForwardShading だったり、2D なら NoShading だったり。
 * RenderStage の集合に対して遅延描画などの描画方式の都合（不透明、半透明）や最適化のための並び替えフィルタリング、カリングなどを行い正確に効率よく描画する。
 * 実際の描画は RenderFeature に任せる。
 *
 * ### RenderView
 * シーンレンダリングが行われるフレームバッファと考えてよい。
 * RenderStage の集合 を入力して描画を行う、シーンレンダリングのエントリーポイント。
 * SceneRenderer のインスタンスを持つ。（SceneRendererとは 1:1 の関係）
 * RenderStage の集合 に対しては n:n。
 * これは主にエディタ上で、シーンのプレビューを異なるウィンドウで見れるようにするための仕組み。(通常レンダリングと深度マップを同時表示など)
 * オフスクリーンレンダリングでも使用される。
 *
 * ※Unity の Camera が持っている RenderTarge 関係の情報を抜き出した感じ。Camera クラスと統合しないのは、
 * - Lumino としては World のモジュールの方が上位なので、依存したくない
 * - Camera オブジェクトは World ごとにインスタンスが変わるのに対して、RenderView は UIViewport 側につくので、寿命が異なる。
 *
 * Note:
 * - でも RenderView と Camera の 1:n 関係をサポートすると、Camera に View ⇔ World 座標変換メソッドを持たせることができなくなる。or RenderView を毎回指定しなければならない。
 * - ウィンドウorバックバッファのサイズが欲しいときに Camera クラスを想像するだろうか？
 *
 * ###　RenderFerture
 * Sprite や Text など、様々なコンポーネントの描画に特化した派生クラスがある。
 * これの種類によっても State が異なることになるため RenderStage は拡張性が重要となる。
 * またそれ以上に特にユーザーカスタマイズを考慮する必要がある。
 * 例えばユーザーが外部のパーティクルエンジンを組み込みたいと思ったら・・・
 * - ExParticleRenderFeature のような描画を行うクラスを作る (state less)
 * - ExParticleRenderFeatureState のような State 保持クラスを作る
 * - ExParticleRenderCommand のようなジオメトリ形状保持クラスを作る (0.4.0 のころの DrawElement)
 * - RenderStage へステートとして、
 *   - ExParticleRenderFeature のポインタ
 *   - ExParticleRenderFeatureState
 *   をセットする。
 * - 描画時、ExParticleRenderCommand を add する
 *
 *
 * 主な流れ
 * --------
 * RenderingContext は CommandBuffer を構築する。
 * CommandBuffer は fix したら、それが持つ描画コマンドをもとに RenderStage リストを構築する。
 * SceneRenderer はコマンド実行ポイントで、CommandBuffer が持つ RenderStage リストを実行する。
 *
 * コマンド実行ポイント：https://docs.unity3d.com/ja/current/Manual/GraphicsCommandBuffers.html
 *
 *
 * World と UI
 * --------
 * それぞれ State として必要なものが違うので、RenderStage を継承した
 * WorldRenderStage や UIRenderStage を作ることになる。
 *
 * WorldRenderStage は Material などを持ち、
 * UIRenderStage は Brush や Pen などを持つ。
 *
 *
 * Zソート、ビューカリング、PhongShadingの影響ライトについて
 * --------
 *
 */
class RenderingManager : public Module {
public:
    struct Settings {
        GraphicsManager* graphicsManager;
        FontManager* fontManager;
    };
    static RenderingManager* initialize(const Settings& settings);
    static void terminate();
    static inline RenderingManager* instance() { return s_instance; }



    struct BlendShapeShader {
        Ref<Shader> shader;
        ShaderPass* shaderPass;
        int dstVerticesGID;
        int srcVerticesGID;
        int target0GID;
        int target1GID;
        int target2GID;
        int target3GID;
        int blendInfoGID;
    };
    BlendShapeShader blendShapeShader;


    GraphicsManager* graphicsManager() const { return m_graphicsManager; }
    FontManager* fontManager() const { return m_fontManager; }
    const Ref<VertexLayout>& standardVertexDeclaration() const { return m_standardVertexDeclaration; }
    const Ref<detail::IVertexDeclaration>& standardVertexDeclarationRHI() const { return m_standardVertexDeclarationRHI; }
    // const Ref<DrawElementListBuilder>& renderStageListBuilder() const { return m_renderStageListBuilder; }

    //const Ref<ClearRenderFeature>& clearRenderFeature() const { return m_clearRenderFeature; }
    //const Ref<BlitRenderFeature>& blitRenderFeature() const { return m_blitRenderFeature; }
    //const Ref<SpriteRenderFeature2>& spriteRenderFeature2() const { return m_spriteRenderFeature2; }
    //const Ref<MeshRenderFeature>& meshRenderFeature() const { return m_meshRenderFeature; }
    //const Ref<MeshGeneraterRenderFeature>& meshGeneraterRenderFeature() const { return m_meshGeneraterRenderFeature; }
    //const Ref<PrimitiveRenderFeature>& primitiveRenderFeature() const { return m_primitiveRenderFeature; }
    //const Ref<SpriteTextRenderFeature>& spriteTextRenderFeature() const { return m_spriteTextRenderFeature; }
//    const Ref<FrameRectRenderFeature>& frameRectRenderFeature() const { return m_frameRectRenderFeature; }
//#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
//    const Ref<ShapesRenderFeature2>& shapesRenderFeature() const { return m_shapesRenderFeature; }
//#else
//    const Ref<ShapesRenderFeature>& shapesRenderFeature() const { return m_shapesRenderFeature; }
//#endif
    //const Ref<PathRenderFeature>& pathRenderFeature() const { return m_pathRenderFeature; }
    //const Ref<ExtensionRenderFeature>& extensionRenderFeature() const { return m_extensionRenderFeature; }
    //const List<Ref<RenderFeature>>& renderFeatures() const { return m_renderFeatures; }
    const URef<kanata::ScreenRectangleRenderFeature>& screenRectangleRenderFeature() const { return m_screenRectangleRenderFeature; }
    const URef<kanata::PrimitiveMeshRenderer>& primitiveRenderer() const { return m_primitiveRenderer; }
    const URef<kanata::MeshRenderFeature>& meshRenderFeature() const { return m_meshRenderFeature; }
    const URef<kanata::SpriteRenderFeature>& spriteRenderFeature() const { return m_spriteRenderFeature; }
    const URef<kanata::ShapesRenderFeature>& shapesRenderFeature() const { return m_shapesRenderFeature; }
    const URef<kanata::SpriteTextRenderFeature>& spriteTextRenderFeature() const { return m_spriteTextRenderFeature; }
    const URef<kanata::FrameRectRenderFeature>& frameRectRenderFeature() const { return m_frameRectRenderFeature; }
    
    const Ref<SpriteRenderer>& spriteRenderer() const { return m_spriteRenderer; }
    const Ref<PrimitiveMeshRenderer>& primitiveMeshRenderer() const { return m_primitiveMeshRenderer; }

    const std::unique_ptr<RenderingProfiler>& profiler() const { return m_profiler; }

    const Ref<LinearAllocatorPageManager>& stageDataPageManager() const { return m_stageDataPageManager; }
    const Ref<Material>& defaultMaterial() const { return m_defaultMaterial; }
    const Ref<Texture2D>& randomTexture() const { return m_randomTexture; }
    const Ref<Shader>& builtinShader(BuiltinShader shader) const { return m_builtinShaders[(int)shader]; }
    const Ref<Material>& builtinMaterials(BuiltinMaterial material) const { return m_builtinMaterials[(int)material]; }
    const Ref<Material>& primitiveMeshDefaultMaterial() const { return m_primitiveMeshDefaultMaterial; };

private:
    RenderingManager();
    Result<> init(const Settings& settings);
    void dispose();

    void createBuiltinShader(BuiltinShader index, const Char* name, const void* data, int dataLen);

    GraphicsManager* m_graphicsManager;
    FontManager* m_fontManager;
    Ref<VertexLayout> m_standardVertexDeclaration;
    Ref<detail::IVertexDeclaration> m_standardVertexDeclarationRHI;
    // Ref<DrawElementListBuilder> m_renderStageListBuilder;
//    Ref<ClearRenderFeature> m_clearRenderFeature;
//    Ref<BlitRenderFeature> m_blitRenderFeature;
//    Ref<SpriteRenderFeature2> m_spriteRenderFeature2;
//    Ref<MeshRenderFeature> m_meshRenderFeature;
//    Ref<MeshGeneraterRenderFeature> m_meshGeneraterRenderFeature;
//    Ref<PrimitiveRenderFeature> m_primitiveRenderFeature;
//    Ref<SpriteTextRenderFeature> m_spriteTextRenderFeature;
//    Ref<FrameRectRenderFeature> m_frameRectRenderFeature;
//#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
//    Ref<ShapesRenderFeature2> m_shapesRenderFeature;
//#else
//    Ref<ShapesRenderFeature> m_shapesRenderFeature;
//#endif
//    Ref<PathRenderFeature> m_pathRenderFeature;
//    Ref<ExtensionRenderFeature> m_extensionRenderFeature;
//    List<Ref<RenderFeature>> m_renderFeatures;

    /*
    名前どうしようか問題
    ----------
    候補:
    SpriteRenderFeature
    SpriteRenderer
    SpriteDrawInterface
    SpriteMeshProcessor
    SpriteProcessor
    SpriteBatch
    SpriteContext
    SpriteRenderingContext
    ポイント:
    ・公開する。でも積極的に使うものではない。Sprite や Tilemap、Particle などでは足りないようなカスタム描画するときに使う。
    ・RenderFeature は検索すると Unity のがヒットする。これは Pipeline を拡張するものであり、特定の形状を構築・描画するものではない。
    ・XXProcessor は、 MeshProcessing という言葉があるように、描画するというよりは何かを加工・構築するイメージがある。
    ・Context/RenderingContext は既に同じような名前のものがあるため、それと同列に考えられるのも困る。
    ・SpriteRenderer は逆にそのものズバリ感が強すぎる。全く別の用途として XXXXSceneRenderer などがあり、混乱の可能性が高い。
      ・でも、SceneRenderer を RenderFeature にするのもアリかも。その方が Unity のイメージにも近い。
    */
    URef<kanata::ScreenRectangleRenderFeature> m_screenRectangleRenderFeature;
    URef<kanata::PrimitiveMeshRenderer> m_primitiveRenderer;
    URef<kanata::MeshRenderFeature> m_meshRenderFeature;
    URef<kanata::SpriteRenderFeature> m_spriteRenderFeature;
    URef<kanata::ShapesRenderFeature> m_shapesRenderFeature;
    URef<kanata::SpriteTextRenderFeature> m_spriteTextRenderFeature;
    URef<kanata::FrameRectRenderFeature> m_frameRectRenderFeature;

    Ref<SpriteRenderer> m_spriteRenderer;
    Ref<PrimitiveMeshRenderer> m_primitiveMeshRenderer;

    std::unique_ptr<RenderingProfiler> m_profiler;

    // RenderStage 関係のデータ (ステートやコマンド) 用の LinearAllocatorPageManager
    Ref<LinearAllocatorPageManager> m_stageDataPageManager;

    Ref<Material> m_defaultMaterial;
    Ref<Texture2D> m_randomTexture;
    std::array<Ref<Shader>, 27> m_builtinShaders;
    std::array<Ref<Material>, 2> m_builtinMaterials;
    Ref<Material> m_primitiveMeshDefaultMaterial;       // FIXME: リファクタリングで SceneManager から移動してきたもの。m_builtinMaterials に混ぜていいかも

    static RenderingManager* s_instance;
};

} // namespace detail
} // namespace ln
