
#pragma once
#include <map>
#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "../Modeling/ModelManager.h"
#include "Internal.h"

#include "MME/MMETypes.h"	// TODO: これは別の場所に移動したい・・・

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

/// SceneGraphManager
class SceneGraphManager
	: public RefObject
{
public:

	/// 初期化データ
	struct ConfigData
	{
		Lumino::FileManager*		FileManager;
		Physics::PhysicsManager*	PhysicsManager;
		Graphics::GraphicsManager*	GraphicsManager;
		Modeling::ModelManager*		ModelManager;

		ConfigData()
			: FileManager(NULL)
			, PhysicsManager(NULL)
			, GraphicsManager(NULL)
			, ModelManager(NULL)
		{}
	};

public:
	SceneGraphManager(const ConfigData& configData);
	virtual ~SceneGraphManager();

public:

	// TODO: SceneGraphManager はホントに単純なシーングラフ管理だけにとどめておいて、MMD 用のシーン構築は別クラスにしてもいいかも
	void CreateMMDSceneGraph();
	void ReleaseMMDSceneGraph();
	ViewPane* GetMMDViewPane() { return m_mmdViewPane; }
	SceneNode* GetRootNode() { return m_defaultRoot; }
	SceneNode* Get2DRootNode() { return m_default2DRoot; }
	SceneNode* Get3DRootNode() { return m_default3DRoot; }
	Camera* GetDefault3DCamera() { return m_default3DCamera; }

	


	/// 名前を指定してノードを検索する (同名ノードの場合は最初のノード)
	SceneNode* FindNodeFirst(const String& name);

	/// 全てのノードを階層的に更新する
	void UpdateFrame(float elapsedTime);

	/// 現在の時間を取得する (秒)
	double GetTime() const { return m_time; }

	/// 前回フレームからの経過時間を取得する (秒)
	float GetElapsedTime() const { return m_elapsedTime; }

public:	// internal

	Physics::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	Graphics::GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Modeling::ModelManager* GetModelManager() { return m_modelManager; }
	Graphics::GeometryRenderer* GetGeometryRenderer() { return m_geometryRenderer; }
	Graphics::Texture* GetDummyTexture() { return m_dummyTexture; }

	void OnNodeRename(SceneNode* node, const String& oldName, const String& newName);

	void AddNode(SceneNode* node) { m_allNodes.Add(node); }
	void RemoveNode(SceneNode* node) { m_allNodes.Remove(node); }
	void AddRenderingPass(RenderingPass* pass);
	void RemoveRenderingPass(RenderingPass* pass);
	void AddLight(Light* light);
	void AddShader(MMEShader* shader);
	void RemoveShader(MMEShader* shader);
	SceneShaderList* GetShaderList() { return &m_sceneShaderList; }

	/// 指定した座標に近いライトを取得する (取得する数は outList の要素数。あらかじめ Resize() しておくこと)
	void SelectLight(Vector3* pos, LightNodeList* outList);

private:
	typedef std::multimap<String, SceneNode*>	NodeNameMap;
	typedef std::pair<String, SceneNode*>		NodeNamePair;

	RefPtr<FileManager>					m_fileManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	RefPtr<Modeling::ModelManager>		m_modelManager;
	RefPtr<Graphics::GeometryRenderer>	m_geometryRenderer;
	RefPtr<Graphics::Texture>			m_dummyTexture;

	double								m_time;					///< 時間処理の開始通知からの経過時間 (秒)
	float								m_elapsedTime;			///< 前回フレームからの経過時間 (秒)

	SceneNodeList						m_allNodes;

	Stack<int>							m_renderingPassIDStack;	///< (0～MaxRenderingPass-1)
	NodeNameMap							m_nodeNameMap;			///< ノードを名前で検索するためのマップ
	//SceneNode*							m_rootNode;
	LightNodeList						m_lightNodeList;		///< 全ての Light のリスト
	SceneNodeList						m_renderingNodeList;	///< (いらないかも？)最後の UpdateFrame() で抽出された描画可能なノードのリスト
	SceneShaderList						m_sceneShaderList;

	SceneNode*		m_defaultRoot;
	SceneNode*		m_default3DRoot;
	SceneNode*		m_default2DRoot;
	Camera*			m_default3DCamera;
	Camera*			m_default2DCamera;
	ViewPane*		m_mmdViewPane;
	DrawingLayer*	m_default3DLayer;
	DrawingLayer*	m_default2DLayer;
	RenderingPass*	m_mmdRenderingPasses[MMD_PASS_Max];

	
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
