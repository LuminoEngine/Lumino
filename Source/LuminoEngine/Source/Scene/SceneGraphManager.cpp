
#include "../Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Scene/SceneNode.h>
#include "SceneGraphManager.h"

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
{
	m_dummyWhiteTexture = m_graphicsManager->getDummyWhiteTexture();
}

//------------------------------------------------------------------------------
SceneGraphManager::~SceneGraphManager()
{
}

//------------------------------------------------------------------------------
void SceneGraphManager::createDefaultSceneGraph()
{
	// TODO: いらない
}

//------------------------------------------------------------------------------
void SceneGraphManager::releaseDefaultSceneGraph()
{
	// TODO: いらない
}

//------------------------------------------------------------------------------
void SceneGraphManager::updateFrameDefaultSceneGraph(float elapsedTime)
{
	// TODO: いらない
}

//------------------------------------------------------------------------------
SceneNode* SceneGraphManager::findNodeFirst(const String& name)
{
	NodeNameMap::iterator itr = m_nodeNameMap.find(name);
	if (itr != m_nodeNameMap.end()) {
		return itr->second;
	}
	return NULL;
}

//------------------------------------------------------------------------------
void SceneGraphManager::onNodeRename(SceneNode* node, const String& oldName, const String& newName)
{
	// もし古い名前があればリネームされたということ。一度 map から取り除く
	if (!oldName.isEmpty())
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
	if (!newName.isEmpty()) {
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
