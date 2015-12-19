/*
	[2015/12/2] RenderingPass::RenderNode/RenderSubset と VisualNote::Render について
		従来は RenderingPass::RenderNode の中でサブセットループまわしていた。
		でも、VisualNode を拡張して自前描画したいとき、DrawSubset をオーバーライドするっていうのは少し直感的ではない。
		Render なり OnRender なりをオーバーロードし、基底を呼び出せばデフォルトの処理、
		完全自前にしたければ基底は呼び出さない、が自然。
		また、外部に「自前描画するのでデフォルトのシェーダ使った描画は必要ないよ、でもメインの(MME用など)シーン内に描画するよ」フラグは必要なくなる。
	
	[2015/11/27] ViewPane 廃止
		廃止というか、SceneGraph クラスにした。
		半年くらい間空いた後に見直したけど、やっぱり直感的ではない。


	[2015/5/11]	シェーダ周りのパフォーマンス改善案
		・シェーダ変数は Scene 単位データ、Node 単位データのようにグループ化する
			今は全ての変数を1つの配列で持っており、サブセット単位の描画したいときも Scene 単位の変数を for で見に行っている。

		・Camera 単位データ更新で全てのシェーダを更新しているが、本当に必要なのだけにする。
			3D シーンの描画なのに、2D の分を更新するのは無駄。逆も然り。現状では DrawingLayer の数だけ無駄な更新が走る。
			後述の「リビジョン」と組み合わせて、Node 単位データ更新と同じタイミングで最新に更新できると無駄が無くなる。

		「リビジョン」…
			データが更新されたとき、フレーム更新回数と関連するオブジェクト(例えば↑のCameraポインタ) をキーとして、
			そのデータをマークする。次に更新処理が呼ばれたとき、このキーが一致していれば更新しない。
			遅延評価の考えに近いかも。
			・本当に必要になったときに更新する。
			・更新するデータの元が同じ場合は更新しない。


	[2015/5/11]
		ツリー構造の参照関係について…

		親→子の場合	★これにする
			・子はユーザーが Release してもすぐ消えない。RemoveChild() を明示的に呼び出す必要がある。
			・親は Release したらすぐ消える。子はユーザーが Release するまで消えない。
			→ 子は常に親が NULL になることを考慮する必要がある。親は、消えるときに子.Parent に NULL を格納する。

		親←子の場合
			・親は全ての子を Release しないと消えない。
			→ エディタアプリでシーングラフを編集するとき、ノードの削除がものすごく面倒になる。
			子を持つノードを削除するときは全ての子を走査して Release しなければならない。

	[2015/5/10] MMM エフェクトの細かい仕様調査
		・複数のテクニックがパスの条件にマッチしても、使用されるのはファイル上で先に定義されているテクニック1つのみ。
		・Subset アノテーションが指定されている場合でも↑のとおり。
		  サブセット範囲が重なる2つのテクニックがあっても、使用されるのは先に定義されているもの。


	[2015/5/7]
		・MME シェーダで、CONTROLOBJECT の name アノテーションが "(OffscreenOwner)" であるシェーダは共有できない。

		・デフォルトライトの ViewProjMatrix 等は、ノードに影響する [0] のライトを使用する。
		  なぜかというと、そもそもデフォルトライトは描画空間ごとに1つ存在するべきで、シーン全体の
		  グローバル要素ではないため。

	[2015/5/6]
		以前は SceneNode を作った時、デフォルトで追加される座標空間を指定してデフォルトシェーダを割り当てていた。
		今回はそれはナシ。SetShader() しない限り SceneNode が Shader を持つことはない。
		描画時の RenderPass にデフォルトのシェーダを持たせ、これを使用する。


	描画の流れ
		
		・トップレベル。ViewPane::Render() をよびだせば全てのシーンが描画されるイメージ。(アタッチされているカメラによるが)
			ViewPane::Render()
				for Layer::PreRender()
				for Layer::Render()
				for Layer::PostRender()

			DrawingLayer::Render()
				SceneGraph::Render(camera, rootNode)

			SceneGraph::Render()
				視錐台カリング
				ライトのソート (各ノードに近い4つを割り振る)
				for RenderPass::Render(rootNode)

			RenderPass::Render()
				ノードを再帰的に描画


		・RenderPass
			前までの ContextDrawParam と OffscreenScene に代わるもの。
			OffscreenScene だけでなく、遅延シェーディングなんかは 1枚の絵を描くのに
			ノードツリーを何回も描画するのが普通。
			その時、1回目は法線を描画する、2回目は深度を描画する、3回目はスペキュラ、4回目はテクスチャ… 等、
			描画ごとに使用するシェーダも異なる。




		・ViewPane ごとに行う処理
			・Zソート
			・視錘台カリング
*/

#include "../Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Graphics/Color.h>
#include "ViewPane.h"
#include <Lumino/Scene/SceneNode.h>
#include "SceneGraphManager.h"
#include "SceneHelper.h"
#include "RenderingPass.h"
#include <Lumino/Scene/MMDSceneGraph.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>

// TODO: 移動
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Layer.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
	
//=============================================================================
// SceneGraphManager
//=============================================================================

SceneGraphManager* SceneGraphManager::Instance = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphManager::SceneGraphManager(const ConfigData& configData)
	: m_fileManager(configData.FileManager, true)
	, m_physicsManager(configData.PhysicsManager, true)
	, m_graphicsManager(configData.GraphicsManager, true)
	, m_effectManager(configData.effectManager)
	, m_modelManager(configData.ModelManager, true)
	//, m_rootNode(NULL)
	, m_default3DSceneGraph(nullptr)
	, m_default2DSceneGraph(nullptr)
{

	m_geometryRenderer.Attach(GeometryRenderer::Create(m_graphicsManager), false);

	// ダミーテクスチャ
	m_dummyTexture.Attach(Texture2D::Create(Size(32, 32)));
	BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Color::White);
	m_dummyTexture->Unlock();

	// RenderingPass に割り当てる ID
	for (int i = 0; i < RenderingPass::MaxRenderingPass; ++i) {
		m_renderingPassIDStack.Push(i);
	}

	m_renderingContext = LN_NEW SceneGraphRenderingContext(m_graphicsManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphManager::~SceneGraphManager()
{
	LN_SAFE_RELEASE(m_renderingContext);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::CreateDefaultSceneGraph()
{
	RefPtr<MMDSceneGraph> sg(LN_NEW MMDSceneGraph(), false);
	sg->CreateCore(this);
	sg.SafeAddRef();
	m_default3DSceneGraph = sg;

	RefPtr<Basic2DSceneGraph> sg2d(LN_NEW Basic2DSceneGraph(), false);
	sg2d->CreateCore(this);
	sg2d.SafeAddRef();
	m_default2DSceneGraph = sg2d;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::ReleaseDefaultSceneGraph()
{
	LN_SAFE_RELEASE(m_default3DSceneGraph);
	LN_SAFE_RELEASE(m_default2DSceneGraph);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::UpdateFrameDefaultSceneGraph(float elapsedTime)
{
	if (m_default3DSceneGraph != nullptr) {
		m_default3DSceneGraph->UpdateFrame(elapsedTime);
	}
	if (m_default2DSceneGraph != nullptr) {
		m_default2DSceneGraph->UpdateFrame(elapsedTime);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::RenderDefaultSceneGraph(Texture* renderTarget)
{
	for (Camera* camera : m_allCameraList)
	{
		camera->GetOwnerSceneGraph()->Render(renderTarget, camera);
	}
	//if (m_default3DSceneGraph != nullptr) {
	//	m_default3DSceneGraph->Render(renderTarget);
	//}
	//if (m_default2DSceneGraph != nullptr) {
	//	m_default2DSceneGraph->Render(renderTarget);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneNode* SceneGraphManager::FindNodeFirst(const String& name)
{
	NodeNameMap::iterator itr = m_nodeNameMap.find(name);
	if (itr != m_nodeNameMap.end()) {
		return itr->second;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::OnNodeRename(SceneNode* node, const String& oldName, const String& newName)
{
	// もし古い名前があればリネームされたということ。一度 map から取り除く
	if (!oldName.IsEmpty())
	{
		std::pair<NodeNameMap::iterator, NodeNameMap::iterator> range = m_nodeNameMap.equal_range(oldName);
		for (NodeNameMap::iterator itr = range.first; itr != range.second; ++itr) {
			if (itr->second == node) {
				m_nodeNameMap.erase(itr);
			}
		}
	}

	// 新しい名前で map に追加
	if (!newName.IsEmpty()) {
		m_nodeNameMap.insert(NodeNamePair(newName, node));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::AddRenderingPass(RenderingPass* pass)
{
	// ID 割り当て
	pass->m_internalEntryID = m_renderingPassIDStack.GetTop();
	m_renderingPassIDStack.Pop();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::RemoveRenderingPass(RenderingPass* pass)
{
	// ID 返却
	m_renderingPassIDStack.Push(pass->m_internalEntryID);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::AddShader(MMEShader* shader)
{
	m_sceneShaderList.Add(shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::RemoveShader(MMEShader* shader)
{
	m_sceneShaderList.Remove(shader);
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
