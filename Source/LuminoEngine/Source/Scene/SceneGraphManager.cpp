
#include "../Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Scene/SceneNode.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"

// TODO: 移動
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/SceneGraph.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
	
//==============================================================================
// SceneGraphManager
//==============================================================================

SceneGraphManager* SceneGraphManager::Instance = NULL;

//------------------------------------------------------------------------------
SceneGraphManager::SceneGraphManager(const ConfigData& configData)
	: m_engineDiag(configData.engineDiag)
	, m_fileManager(configData.FileManager, true)
	, m_physicsManager(configData.PhysicsManager, true)
	, m_graphicsManager(configData.GraphicsManager, true)
	, m_effectManager(configData.effectManager)
	, m_modelManager(configData.modelManager)
	, m_documentsManager(configData.documentsManager)
	//, m_rootNode(NULL)
	, m_default3DSceneGraph(nullptr)
	, m_default2DSceneGraph(nullptr)
	, m_default3DCameraViewportLayer(nullptr)
	, m_default2DCameraViewportLayer(nullptr)
	, m_mainViewport(configData.mainViewport)
{
	m_dummyWhiteTexture = m_graphicsManager->GetDummyWhiteTexture();
}

//------------------------------------------------------------------------------
SceneGraphManager::~SceneGraphManager()
{
}

//------------------------------------------------------------------------------
void SceneGraphManager::CreateDefaultSceneGraph()
{
#if 0
	RefPtr<MMDSceneGraph> sg(LN_NEW MMDSceneGraph(), false);
	sg->CreateCore(this);
	sg.SafeAddRef();
	m_default3DSceneGraph = sg;
#endif

	m_default2DSceneGraph = RefPtr<SceneGraph2D>::MakeRef();
	m_default2DSceneGraph->CreateCore(this);

	m_default3DSceneGraph = RefPtr<SceneGraph3D>::MakeRef();
	m_default3DSceneGraph->CreateCore(this);

	m_default3DCameraViewportLayer = LN_NEW CameraViewportLayer();
	m_default3DCameraViewportLayer->Initialize(this, m_default3DSceneGraph->GetMainCamera());
	m_mainViewport->AddViewportLayer(m_default3DCameraViewportLayer);

	m_default2DCameraViewportLayer = LN_NEW CameraViewportLayer();
	m_default2DCameraViewportLayer->Initialize(this, m_default2DSceneGraph->GetMainCamera());
	m_mainViewport->AddViewportLayer(m_default2DCameraViewportLayer);
}

//------------------------------------------------------------------------------
void SceneGraphManager::ReleaseDefaultSceneGraph()
{
	m_mainViewport->RemoveViewportLayer(m_default3DCameraViewportLayer);
	LN_SAFE_RELEASE(m_default3DCameraViewportLayer);
	m_mainViewport->RemoveViewportLayer(m_default2DCameraViewportLayer);
	LN_SAFE_RELEASE(m_default2DCameraViewportLayer);

	m_default2DSceneGraph.SafeRelease();
	m_default3DSceneGraph.SafeRelease();
}

//------------------------------------------------------------------------------
void SceneGraphManager::UpdateFrameDefaultSceneGraph(float elapsedTime)
{
	if (m_default3DSceneGraph != nullptr) {
		m_default3DSceneGraph->UpdateFrame(elapsedTime);
	}
	if (m_default2DSceneGraph != nullptr) {
		m_default2DSceneGraph->UpdateFrame(elapsedTime);
	}
}

//------------------------------------------------------------------------------
SceneNode* SceneGraphManager::FindNodeFirst(const String& name)
{
	NodeNameMap::iterator itr = m_nodeNameMap.find(name);
	if (itr != m_nodeNameMap.end()) {
		return itr->second;
	}
	return NULL;
}

//------------------------------------------------------------------------------
void SceneGraphManager::OnNodeRename(SceneNode* node, const String& oldName, const String& newName)
{
	// もし古い名前があればリネームされたということ。一度 map から取り除く
	if (!oldName.IsEmpty())
	{
		std::pair<NodeNameMap::iterator, NodeNameMap::iterator> range = m_nodeNameMap.equal_range(oldName);
		for (NodeNameMap::iterator itr = range.first; itr != range.second; ++itr)
		{
			if (itr->second == node)
			{
				m_nodeNameMap.erase(itr);
				break;
			}
		}
	}

	// 新しい名前で map に追加
	if (!newName.IsEmpty()) {
		m_nodeNameMap.insert(NodeNamePair(newName, node));
	}
}

////------------------------------------------------------------------------------
//void SceneGraphManager::AddShader(MMEShader* shader)
//{
//	m_sceneShaderList.Add(shader);
//}
//
////------------------------------------------------------------------------------
//void SceneGraphManager::RemoveShader(MMEShader* shader)
//{
//	m_sceneShaderList.Remove(shader);
//}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
