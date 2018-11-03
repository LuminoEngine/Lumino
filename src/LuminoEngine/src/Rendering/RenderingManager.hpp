#pragma once

namespace ln {
class VertexDeclaration;
class Shader;

namespace detail {
class LinearAllocatorPageManager;
class RenderTargetTextureCacheManager;
class FrameBufferCache;
class DepthBufferCacheManager;
class DrawElementListBuilder;
class BlitRenderFeature;
class SpriteRenderFeature;
class MeshRenderFeature;

enum class BuiltinShader
{
	Sprite,
	ClusteredShadingDefault,
	DepthPrepass,
	ShadowCaster,
	//LegacyDiffuse,		// Lambert Shading
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
class RenderingManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
	};

	RenderingManager();
	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }
	const Ref<RenderTargetTextureCacheManager>& renderTargetTextureCacheManager() const { return m_renderTargetTextureCacheManager; }
	const Ref<DepthBufferCacheManager>& depthBufferCacheManager() const { return m_depthBufferCacheManager; }
    const Ref<FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }
	const Ref<VertexDeclaration>& standardVertexDeclaration() const { return m_standardVertexDeclaration; }
	const Ref<DrawElementListBuilder>& renderStageListBuilder() const { return m_renderStageListBuilder; }
    const Ref<BlitRenderFeature>& blitRenderFeature() const { return m_blitRenderFeature; }
	const Ref<SpriteRenderFeature>& spriteRenderFeature() const { return m_spriteRenderFeature; }
	const Ref<MeshRenderFeature>& meshRenderFeature() const { return m_meshRenderFeature; }
	const Ref<LinearAllocatorPageManager>& stageDataPageManager() const { return m_stageDataPageManager; }
	const Ref<Shader>& builtinShader(BuiltinShader shader) const { return m_builtinShaders[(int)shader]; }

private:
	GraphicsManager* m_graphicsManager;
	Ref<RenderTargetTextureCacheManager> m_renderTargetTextureCacheManager;
	Ref<DepthBufferCacheManager> m_depthBufferCacheManager;
    Ref<FrameBufferCache> m_frameBufferCache;
	Ref<VertexDeclaration> m_standardVertexDeclaration;
	Ref<DrawElementListBuilder> m_renderStageListBuilder;
    Ref<BlitRenderFeature> m_blitRenderFeature;
	Ref<SpriteRenderFeature> m_spriteRenderFeature;
	Ref<MeshRenderFeature> m_meshRenderFeature;

	// RenderStage 関係のデータ (ステートやコマンド) 用の LinearAllocatorPageManager
	Ref<LinearAllocatorPageManager> m_stageDataPageManager;

	std::array<Ref<Shader>, 4> m_builtinShaders;
};

} // namespace detail
} // namespace ln

