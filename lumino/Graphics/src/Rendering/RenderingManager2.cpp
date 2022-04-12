/*
[2022/3/28] DrawElement とか周りを整理したいのと、 UE4 メモ
----------
https://docs.unrealengine.com/4.27/ja/ProgrammingAndScripting/Rendering/MeshDrawingPipeline/

FMeshBatch
FStaticMeshBatch
- FMeshBatchElement の配列
- 影落とす？
- どの LODIndex 使う？

FMeshBatchElement
- drawPrimitive に渡す引数をまとめたもの。
- VertexBuffer, IndexBuffer, FirstIndex, NumPrimitives, NumInstances
- struct である。派生はしないようだ。
- Pipeline の ID は持っていない。

FMeshDrawCommand
- drawPrimitive に渡す引数をまとめたもの。
- LOD 解決済み。
- どのシェーダを使うか解決済み。(FMeshDrawShaderBindings)
- Pipeline の ID も持っている。
- StencilRef は FMeshPassProcessor::BuildMeshDrawCommands で、FMeshPassProcessorRenderState から取っている

コールツリー
FMeshPassProcessor::BuildMeshDrawCommands これがいろんなところから呼ばれる。ここで FMeshPassProcessorRenderState, RasterizerState,　PassShaders を統合して FGraphicsMinimalPipelineStateInitializer を作る。
        FCachedPassMeshDrawListContext::FinalizeCommand ここでconst FGraphicsMinimalPipelineStateInitializer& PipelineStateから PipelineId を求める
                FMeshDrawCommand::SetDrawParametersAndFinalize
                ApplyViewOverridesToMeshDrawCommands
                        MeshCommand.Finalize

### UE4が持っているPipelineState (FGraphicsMinimalPipelineStateInitializer)

- FMinimalBoundShaderStateInput	BoundShaderState;
        - FRHIVertexDeclaration* VertexDeclarationRHI = nullptr;
        - TRefCountPtr<FShaderMapResource> VertexShaderResource;
        - TRefCountPtr<FShaderMapResource> HullShaderResource;
        - TRefCountPtr<FShaderMapResource> DomainShaderResource;
        - TRefCountPtr<FShaderMapResource> PixelShaderResource;
        - TRefCountPtr<FShaderMapResource> GeometryShaderResource;
- FRHIBlendState*					BlendState;
- FRHIRasterizerState*			RasterizerState;
- FRHIDepthStencilState*			DepthStencilState;
- FImmutableSamplerState			ImmutableSamplerState;
- EPrimitiveType			PrimitiveType;
普通の RHI の PipelineState と異なり、Shader そのものを持つ。
そしてこれは FShaderMapResource::CreateRHIShader() で RHIShader が作られる。
つまり、 RHIShader の作成は遅延実行である。

なお "Minimal" とあるが、これには RenderPass が含まれていない。
SubmitDraw の中で、現在の RenderPass を使って、最終的な FGraphicsPipelineStateInitializer を作っている。
これからさらにキャッシュを検索し、最終的な RHI の PipelineState を得ている。

### シェーダの選択は誰が？
BuildMeshDrawCommands() の呼び出し元。
FMaterial::TryGetShaders

↑のリンク先にもある FBasePassMeshProcessor::AddMeshBatch() 付近で選択されているようだ。
LuminoのSceneRenderer では Default Shader として、レンダリングパスごとの基本シェーダをフォールバックできるが、
UE4 では FMaterialRenderProxy で取ってくるようだ。
FMaterialRenderProxy はリスト構造（というよりシングルの親子構造）になっている。
FMaterialRenderProxy::GetFallback() で取り出せる。
UMaterial と関連づいているのは、FMaterialRenderProxy の派生である FMaterialInstanceResource。

FMaterialInstanceResource::GetFallback() にて、UMaterial(ユーザー指定のマテリアル) がセットされていればそれを、
そうでなければ、UMaterial::GetDefaultMaterial() から取っている。

### パラメータの取り方
FMaterialRenderProxyはGetVectorValue()やGetTextureValue()をオーバーライドすることで、
特定のレンダーパス固有のパラメータを強制できるようだ。

### UniformShader は？
FMeshDrawShaderBindings::SetShaderBindings() で RHICommandList に Set.
FMeshDrawShaderBindings の生成基になるのが FMeshMaterialShader.
FMeshMaterialShader は FMaterialShader のサブクラス。
FMaterialShader::SetParameters() で、FRHICommandList、RHIShader、Material 等を受け取り、この内部で RHIの UniformBuffer へセットしている。
値の取り出し元は、FMaterialShader自身が持っている FUniformExpressionCache。
FUniformExpressionCache は FUniformExpressionSet::FillUniformBufferで色々セットされるようだ。
そしてこの中から、FMaterialRenderProxy::GetTextureValue() などが呼ばれている。

### FMeshBatch を作るのは？
LuminoのBatchリストのように、誰か統一的なリストで持っているわけではない。
UBoxComponent を例に見てみる。
LuminoのSceneNodeの描画はいわゆるプッシュ式。Node::render から conext へ drwa~() していく。
UE4 の描画はプル式。
各 Component は CreateSceneProxy() を実装し、それぞれの形状に応じた FPrimitiveSceneProxy を返す。
そしてこの FPrimitiveSceneProxy の実装 (BoxComponent なら FBoxSceneProxy) で、メッシュの生成をしたり、スタティックメッシュを返したりする。

### AddMeshBatchは誰が呼ぶ？
いろいろ。

### FMeshBatch.CastShadow

FShadowDepthPassMeshProcessor::AddMeshBatch() とかで参照し、
受け取った MeshBatch を MeshCommand に変換する。

### DrawStaticElements 空の流れ
FModelSceneProxy.DrawStaticElements     ここで FMeshBatch を作る (ローカル変数)
    FStaticPrimitiveDrawInterface::DrawMesh

### FPrimitiveDrawInterface.DrawLine で Element 作ってるけど、Batch にまとめるのはだれ？
FPrimitiveDrawInterfaceが作るのはFBatchedElementsなので注意。FBatchElement ではない。

FPrimitiveDrawInterface の実装は以下２つ。
- FViewElementPDI
- FSimpleElementCollector
FViewElementPDI は FViewInfo.BatchedViewElements  へ追加していく。
ただこれは SceneRendering 関係なく、 RHI へ直接描画しているように見える。
FSimpleElementCollector でもあまり変わらない気がするなぁ…？
FViewElementPDI::DrawMesh に来たときは、FMeshBach を作っている。

### 動的メッシュの生成は？
FTextRenderSceneProxy を見るのがわかりやすいかも。
Luminoの PrimitiveRenderFeature のような、どこかに統一的な VertexBuffer を持った Renderer が居るものではない。
FTextRenderSceneProxyがコマンド化されて RenderingThread で実行されると、
まずは FTextRenderSceneProxy::CreateRenderThreadResources() が呼ばれる。
ここで Font と String を元に、テキストスプライトを作る。
その後 FTextRenderSceneProxy::GetDynamicMeshElements や FTextRenderSceneProxy::DrawStaticElements で、FMeshBatch になる。
FMeshBatch& Mesh = Collector.AllocateMesh();

### 動的メッシュの頂点作成方法とマテリアルは？
FLineBatcherSceneProxy::GetDynamicMeshElements() が参考になる。
FDynamicMeshBuilder というものを使って、AddTriangle(), AddLine() などで形状を追加していき、
最後に GetMesh() へ Material と Collector を渡すことで、FMeshBatch を作っている。

### テキストレンダリング
FTextRenderSceneProxy::BuildStringMesh() 見てみよう。
コードを見る限り、Lumino と同じ SpriteText である。
1枚のテクスチャに入りきらないといは Batch を切り替えるのも同じ。

FTextRenderSceneProxy が参照しているフォントは UFont.
フォントの扱いには EFontCacheType::Offline と EFontCacheType::Runtime がある。Runtime は主にslateで使用される、とのコメントがある。動的フォントはこちら、
FSlateFontAtlasRHI::ConditionalUpdateTexture で、 FSlateFontAtlas.AtlasData を、RHI のテクスチャに書き込んでいるのがある。
FCharacterRenderData がグリフの生ビットマップデータのようだ。
これを FSlateTextureAtlas::AddTexture() で追加しているようだ。
空き場所を探すのは FSlateTextureAtlas::FindSlotForTexture().
これはおそらく一般的なテクスチャアトラスの生成プログラムとおなじ。https://tyfkda.github.io/blog/2013/10/05/texture-pakcer.html

*/

#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include "RenderingManager2.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderingManager2

RenderingManager2::RenderingManager2()
    : m_standardVertexLayout(nullptr) {
}

Result RenderingManager2::init(const Settings& settings) {
    LN_LOG_DEBUG("RenderingManager2 Initialization started.");

    static VertexElement elements[] = {
        { 0, VertexElementType::Float4, VertexElementUsage::Position, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Normal, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::TexCoord, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Color, 0 },
        { 0, VertexElementType::Float4, VertexElementUsage::Tangent, 0 },
    };
    m_standardVertexLayout = makeObject<VertexLayout>(elements, LN_ARRAY_SIZE_OF(elements));

    LN_LOG_DEBUG("RenderingManager2 Initialization finished.");
    return ok();
}

void RenderingManager2::dispose() {
    LN_LOG_DEBUG("RenderingManager2 dispose started.");

    if (m_standardVertexLayout) {
        m_standardVertexLayout->dispose();
        m_standardVertexLayout = nullptr;
    }

    LN_LOG_DEBUG("RenderingManager2 dispose finished.");
}

} // namespace detail
} // namespace ln
