
#pragma once
#include <map>
#include <Lumino/Base/Stack.h>
#include <Lumino/IO/FileManager.h>
#include "../Physics/PhysicsManager.h"
#include "../../Source/Graphics/GraphicsManager.h"		// TODO: remove
#include <Lumino/Scene/Common.h>
#include "../Effect/EffectManager.h"
#include "../Modeling/ModelManager.h"
#include "Internal.h"

//#include "MME/MMETypes.h"	// TODO: これは別の場所に移動したい・・・

LN_NAMESPACE_BEGIN
namespace detail { class DocumentsManager; }
class EngineDiagCore;
class SceneGraph2D;
class SceneGraph3D;

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
		detail::GraphicsManager*	GraphicsManager;
		detail::EffectManager*		effectManager;
		detail::ModelManager*		modelManager = nullptr;
		detail::DocumentsManager*	documentsManager = nullptr;
		Viewport*					mainViewport = nullptr;


		ConfigData()
			: FileManager(nullptr)
			, PhysicsManager(nullptr)
			, GraphicsManager(nullptr)
			, effectManager(nullptr)
		{}
	};

public:
	static SceneGraphManager* Instance;

public:
	SceneGraphManager(const ConfigData& configData);
	virtual ~SceneGraphManager();

public:

	// TODO: SceneGraphManager はホントに単純なシーングラフ管理だけにとどめておいて、MMD 用のシーン構築は別クラスにしてもいいかも
	void CreateDefaultSceneGraph();
	void ReleaseDefaultSceneGraph();
	SceneGraph2D* GetDefaultSceneGraph2D() { return m_default2DSceneGraph; }
	SceneGraph3D* GetDefaultSceneGraph3D() { return m_default3DSceneGraph; }
	CameraViewportLayer* GetDefault2DCameraViewportLayer() { return m_default2DCameraViewportLayer; }
	CameraViewportLayer* GetDefault3DCameraViewportLayer() { return m_default3DCameraViewportLayer; }
	void UpdateFrameDefaultSceneGraph(float elapsedTime);
	//void RenderDefaultSceneGraph(Texture* renderTarget);

	


	/// 名前を指定してノードを検索する (同名ノードの場合は最初のノード)
	SceneNode* FindNodeFirst(const String& name);


public:	// internal
	EngineDiagCore* GetEngineDiag() const { return m_engineDiag; }
	detail::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	detail::GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	detail::EffectManager* GetEffectManager() { return m_effectManager; }
	detail::ModelManager* GetModelManager() { return m_modelManager; }
	detail::DocumentsManager* GetDocumentsManager() { return m_documentsManager; }
	Texture2D* GetDummyWhiteTexture() { return m_dummyWhiteTexture; }

	void OnNodeRename(SceneNode* node, const String& oldName, const String& newName);

	void AddLight(Light* light);
	List<Camera*>* GetAllCameraList() { return &m_allCameraList; }

	/// 指定した座標に近いライトを取得する (取得する数は outList の要素数。あらかじめ Resize() しておくこと)
	void SelectLight(Vector3* pos, LightNodeList* outList);

private:
	typedef std::multimap<String, SceneNode*>	NodeNameMap;
	typedef std::pair<String, SceneNode*>		NodeNamePair;

	EngineDiagCore*						m_engineDiag;
	RefPtr<FileManager>					m_fileManager;
	RefPtr<detail::PhysicsManager>		m_physicsManager;	// TODO: remove RefPtr
	RefPtr<detail::GraphicsManager>	m_graphicsManager;	// TODO: remove RefPtr
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	detail::DocumentsManager*			m_documentsManager;
	RefPtr<Texture2D>			m_dummyWhiteTexture;



	//Stack<int>							m_renderingPassIDStack;	///< (0～MaxRenderingPass-1)
	NodeNameMap							m_nodeNameMap;			///< ノードを名前で検索するためのマップ
	//SceneNode*							m_rootNode;
	LightNodeList						m_lightNodeList;		///< 全ての Light のリスト

	List<Camera*>	m_allCameraList;

	RefPtr<SceneGraph2D>		m_default2DSceneGraph;
	RefPtr<SceneGraph3D>		m_default3DSceneGraph;

	CameraViewportLayer*	m_default2DCameraViewportLayer;
	CameraViewportLayer*	m_default3DCameraViewportLayer;
	Viewport*				m_mainViewport;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
