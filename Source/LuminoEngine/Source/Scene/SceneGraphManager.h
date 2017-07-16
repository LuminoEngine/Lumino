
#pragma once
#include <map>
#include <Lumino/Base/Stack.h>
#include <Lumino/IO/FileManager.h>
#include "../Physics/PhysicsManager.h"
#include "../../Source/Graphics/GraphicsManager.h"		// TODO: remove
#include <Lumino/Scene/Common.h>
#include "../Effect/EffectManager.h"
#include "../Mesh/ModelManager.h"
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
	void createDefaultSceneGraph();
	void releaseDefaultSceneGraph();
	//CameraViewportLayer* GetDefault2DCameraViewportLayer() { return m_default2DCameraViewportLayer; }
	//CameraViewportLayer* GetDefault3DCameraViewportLayer() { return m_default3DCameraViewportLayer; }
	void updateFrameDefaultSceneGraph(float elapsedTime);
	//void RenderDefaultSceneGraph(Texture* renderTarget);

	


	/// 名前を指定してノードを検索する (同名ノードの場合は最初のノード)
	SceneNode* findNodeFirst(const String& name);


public:	// internal
	EngineDiagCore* getEngineDiag() const { return m_engineDiag; }
	detail::PhysicsManager* getPhysicsManager() { return m_physicsManager; }
	detail::GraphicsManager* getGraphicsManager() { return m_graphicsManager; }
	detail::EffectManager* getEffectManager() { return m_effectManager; }
	detail::ModelManager* getModelManager() { return m_modelManager; }
	detail::DocumentsManager* getDocumentsManager() { return m_documentsManager; }
	Texture2D* getDummyWhiteTexture() { return m_dummyWhiteTexture; }

	void onNodeRename(SceneNode* node, const String& oldName, const String& newName);

	void addLight(LightComponent* light);

	/// 指定した座標に近いライトを取得する (取得する数は outList の要素数。あらかじめ Resize() しておくこと)
	void selectLight(Vector3* pos, LightNodeList* outList);

private:
	typedef std::multimap<String, SceneNode*>	NodeNameMap;
	typedef std::pair<String, SceneNode*>		NodeNamePair;

	EngineDiagCore*						m_engineDiag;
	Ref<FileManager>					m_fileManager;
	Ref<detail::PhysicsManager>		m_physicsManager;	// TODO: remove Ref
	Ref<detail::GraphicsManager>	m_graphicsManager;	// TODO: remove Ref
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	detail::DocumentsManager*			m_documentsManager;
	Ref<Texture2D>			m_dummyWhiteTexture;



	//Stack<int>							m_renderingPassIDStack;	///< (0～MaxRenderingPass-1)
	NodeNameMap							m_nodeNameMap;			///< ノードを名前で検索するためのマップ
	LightNodeList						m_lightNodeList;		///< 全ての LightComponent のリスト



	//CameraViewportLayer*	m_default2DCameraViewportLayer;
	//CameraViewportLayer*	m_default3DCameraViewportLayer;
	//Viewport*				m_mainViewport;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
