/*
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
#include "SceneNode.h"
#include "SceneGraphManager.h"

// TODO: 移動
#include "Camera.h"
#include "Layer.h"
#include "MME/MMERenderingPass.h"
#include "InfomationRenderingPass.h"

namespace Lumino
{
namespace Scene
{
	
//=============================================================================
// SceneGraphManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphManager::SceneGraphManager(const ConfigData& configData)
	: m_fileManager(configData.FileManager, true)
	, m_physicsManager(configData.PhysicsManager, true)
	, m_graphicsManager(configData.GraphicsManager, true)
	, m_modelManager(configData.ModelManager, true)
	//, m_rootNode(NULL)
	, m_time(0.0)
	, m_defaultRoot(NULL)
	, m_default3DRoot(NULL)
	, m_default2DRoot(NULL)
	, m_default3DCamera(NULL)
	, m_default2DCamera(NULL)
	, m_mmdViewPane(NULL)
	, m_default3DLayer(NULL)
	, m_default2DLayer(NULL)
{
	memset(m_mmdRenderingPasses, 0, sizeof(m_mmdRenderingPasses));

	m_geometryRenderer.Attach(Graphics::GeometryRenderer::Create(m_graphicsManager), false);

	// ダミーテクスチャ
	m_dummyTexture.Attach(Graphics::Texture::Create(Size(32, 32)));
	Graphics::BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Graphics::Color::White);
	m_dummyTexture->Unlock();

	// RenderingPass に割り当てる ID
	for (int i = 0; i < RenderingPass::MaxRenderingPass; ++i) {
		m_renderingPassIDStack.Push(i);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphManager::~SceneGraphManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::CreateMMDSceneGraph()
{
	m_defaultRoot = LN_NEW SceneNode();
	m_defaultRoot->Create(this);

	m_default3DRoot = LN_NEW SceneNode();
	m_default3DRoot->Create(this);
	m_defaultRoot->AddChild(m_default3DRoot);

	m_default2DRoot = LN_NEW SceneNode();
	m_default2DRoot->Create(this);
	m_defaultRoot->AddChild(m_default2DRoot);

	m_default3DCamera = LN_NEW Camera(this, CameraProjection_3D);
	m_default3DRoot->AddChild(m_default3DCamera);

	m_default2DCamera = LN_NEW Camera(this, CameraProjection_2D);
	m_default2DRoot->AddChild(m_default2DCamera);
	
	m_default3DLayer = LN_NEW DrawingLayer(this);
	m_default3DLayer->SetCamera(m_default3DCamera);
	m_default3DLayer->SetRenderingRootNode(m_default3DRoot);
	
	m_default2DLayer = LN_NEW DrawingLayer(this);
	m_default2DLayer->SetCamera(m_default2DCamera);
	m_default2DLayer->SetRenderingRootNode(m_default2DRoot);

	m_mmdViewPane = LN_NEW ViewPane(this);
	m_mmdViewPane->GetLayerList()->Add(m_default3DLayer);
	m_mmdViewPane->GetLayerList()->Add(m_default2DLayer);

	//m_mmdRenderingPasses[MMD_PASS_zplot] = LN_NEW MMERenderingPass(this, MMD_PASS_zplot);
	//m_mmdRenderingPasses[MMD_PASS_shadow] = LN_NEW MMERenderingPass(this, MMD_PASS_shadow);
	//m_mmdRenderingPasses[MMD_PASS_edge] = LN_NEW MMERenderingPass(this, MMD_PASS_edge);
	m_mmdRenderingPasses[MMD_PASS_object] = LN_NEW MMERenderingPass(this, MMD_PASS_object);
	//m_mmdRenderingPasses[MMD_PASS_object_ss] = LN_NEW MMERenderingPass(this, MMD_PASS_object_ss);
	m_mmdRenderingPasses[MMD_PASS_Infomation] = LN_NEW InfomationRenderingPass(this);

	m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_object]);
	m_default3DLayer->GetRenderingPasses()->Add(m_mmdRenderingPasses[MMD_PASS_Infomation]);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::ReleaseMMDSceneGraph()
{
	for (int i = 0; i < MMD_PASS_Max; i++) {
		LN_SAFE_RELEASE(m_mmdRenderingPasses[i]);
	}

	LN_SAFE_RELEASE(m_default3DLayer);
	LN_SAFE_RELEASE(m_default2DLayer);
	LN_SAFE_RELEASE(m_defaultRoot);
	LN_SAFE_RELEASE(m_default3DCamera);
	LN_SAFE_RELEASE(m_default2DCamera);
	LN_SAFE_RELEASE(m_default2DRoot);
	LN_SAFE_RELEASE(m_default3DRoot);
	LN_SAFE_RELEASE(m_mmdViewPane);
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
void SceneGraphManager::UpdateFrame(float elapsedTime)
{
	LN_FOREACH(SceneNode* node, m_allNodes) {
		node->UpdateFrame(elapsedTime);
	}

	m_elapsedTime = elapsedTime;
	m_time += elapsedTime;
	m_defaultRoot->UpdateFrameHierarchy(NULL, &m_renderingNodeList);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SceneGraphManager::OnNodeRename(SceneNode* node, const String& oldName, const String& newName)
{
	// もし古い前があればリネームされたということ。一度 map から取り除く
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


} // namespace Scene
} // namespace Lumino
