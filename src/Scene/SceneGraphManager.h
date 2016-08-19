
#pragma once
#include <map>
#include <Lumino/Base/Stack.h>
#include <Lumino/IO/FileManager.h>
#include "../Physics/PhysicsManager.h"
#include "../../src/Graphics/GraphicsManager.h"		// TODO: remove
#include <Lumino/Scene/Common.h>
#include "../Effect/EffectManager.h"
#include "../Modeling/ModelManager.h"
#include "Internal.h"

#include "MME/MMETypes.h"	// TODO: これは別の場所に移動したい・・・

LN_NAMESPACE_BEGIN
class EngineDiagCore;

LN_NAMESPACE_SCENE_BEGIN

/// SceneGraphManager
class SceneGraphManager
	: public RefObject
{
public:

	/// 初期化データ
	struct ConfigData
	{
		EngineDiagCore*				engineDiag = nullptr;
		FileManager*		FileManager;
		detail::PhysicsManager*	PhysicsManager;
		GraphicsManager*	GraphicsManager;
		detail::EffectManager*		effectManager;
		detail::ModelManager*		modelManager = nullptr;
		Viewport*					mainViewport = nullptr;

		//Modeling::ModelManager*		ModelManager;

		ConfigData()
			: FileManager(nullptr)
			, PhysicsManager(nullptr)
			, GraphicsManager(nullptr)
			, effectManager(nullptr)
			//, ModelManager(nullptr)
		{}
	};

public:
	static SceneGraphManager* Instance;

public:
	SceneGraphManager(const ConfigData& configData);
	virtual ~SceneGraphManager();

public:
	SceneGraphRenderingContext* GetRenderingContext() { return m_renderingContext; }

	// TODO: SceneGraphManager はホントに単純なシーングラフ管理だけにとどめておいて、MMD 用のシーン構築は別クラスにしてもいいかも
	void CreateDefaultSceneGraph();
	void ReleaseDefaultSceneGraph();
	SceneGraph* GetDefault3DSceneGraph() { return m_default3DSceneGraph; }
	SceneGraph* GetDefault2DSceneGraph() { return m_default2DSceneGraph; }
	CameraViewportLayer* GetDefault3DCameraViewportLayer() { return m_default3DCameraViewportLayer; }
	CameraViewportLayer* GetDefault2DCameraViewportLayer() { return m_default2DCameraViewportLayer; }
	void UpdateFrameDefaultSceneGraph(float elapsedTime);
	//void RenderDefaultSceneGraph(Texture* renderTarget);

	


	/// 名前を指定してノードを検索する (同名ノードの場合は最初のノード)
	SceneNode* FindNodeFirst(const String& name);


public:	// internal
	EngineDiagCore* GetEngineDiag() const { return m_engineDiag; }
	detail::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	detail::EffectManager* GetEffectManager() { return m_effectManager; }
	detail::ModelManager* GetModelManager() { return m_modelManager; }
	Texture2D* GetDummyWhiteTexture() { return m_dummyWhiteTexture; }

	void OnNodeRename(SceneNode* node, const String& oldName, const String& newName);

	void AddRenderingPass(RenderingPass* pass);
	void RemoveRenderingPass(RenderingPass* pass);
	void AddLight(Light* light);
	void AddShader(MMEShader* shader);
	void RemoveShader(MMEShader* shader);
	SceneShaderList* GetShaderList() { return &m_sceneShaderList; }
	Array<Camera*>* GetAllCameraList() { return &m_allCameraList; }

	/// 指定した座標に近いライトを取得する (取得する数は outList の要素数。あらかじめ Resize() しておくこと)
	void SelectLight(Vector3* pos, LightNodeList* outList);

private:
	typedef std::multimap<String, SceneNode*>	NodeNameMap;
	typedef std::pair<String, SceneNode*>		NodeNamePair;

	EngineDiagCore*						m_engineDiag;
	RefPtr<FileManager>					m_fileManager;
	RefPtr<detail::PhysicsManager>		m_physicsManager;
	RefPtr<GraphicsManager>	m_graphicsManager;
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	RefPtr<Texture2D>			m_dummyWhiteTexture;

	SceneGraphRenderingContext*		m_renderingContext;


	Stack<int>							m_renderingPassIDStack;	///< (0～MaxRenderingPass-1)
	NodeNameMap							m_nodeNameMap;			///< ノードを名前で検索するためのマップ
	//SceneNode*							m_rootNode;
	LightNodeList						m_lightNodeList;		///< 全ての Light のリスト
	SceneShaderList						m_sceneShaderList;

	Array<Camera*>	m_allCameraList;

	SceneGraph*		m_default3DSceneGraph;
	SceneGraph*		m_default2DSceneGraph;

	CameraViewportLayer*	m_default3DCameraViewportLayer;
	CameraViewportLayer*	m_default2DCameraViewportLayer;
	Viewport*				m_mainViewport;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
