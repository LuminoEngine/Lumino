
#pragma once
#include "../Internal.h"
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/Layer.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// DrawingLayer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingLayer::DrawingLayer(SceneGraphManager* manager)
	: m_manager(manager)
	, m_camera(NULL)
	, m_renderingRootNode(NULL)
	, m_renderingPassList(LN_NEW RenderingPassRefList(), false)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingLayer::~DrawingLayer()
{
	LN_SAFE_RELEASE(m_camera);
	LN_SAFE_RELEASE(m_renderingRootNode);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingLayer::PreRender(const SizeF& viewSize)
{
	LN_THROW(m_camera != NULL, InvalidOperationException);
	LN_THROW(m_renderingRootNode != NULL, InvalidOperationException);

	m_renderingNodeList.Clear();
	m_renderingLightList.Clear();
	
	// カメラ行列の更新 (TODO: ここで更新すると、複数レイヤーが同じカメラを参照している時に無駄な計算になるかも)
	m_camera->UpdateMatrices(viewSize);

	// シェーダのカメラ単位データの更新
	//		TODO: とりあえず全シェーダ更新している。そんなにたくさんのシェーダは使わないだろうという想定。
	//		もちろん数が増えてくればこの辺がパフォーマンス的にクリティカルになる。
	//		改善案はあるが、とりあえず。(SceneGraphManaer.cpp 参照)
	LN_FOREACH(MMEShader* shader, *m_manager->GetShaderList()) {
		shader->UpdateCameraParams(m_camera, viewSize);
	}

	// このレイヤーのカメラに依るデータを更新する (視錘台カリングやカメラからの距離の更新)
	m_renderingRootNode->UpdateViewFlustumHierarchy(m_camera, &m_renderingNodeList, &m_renderingLightList);

	// ライト行列の更新
	LN_FOREACH(Light* light, m_renderingLightList) {
		light->UpdateMatrices(viewSize);
	}

	// Z ソート・優先度ソート
	std::stable_sort(m_renderingNodeList.begin(), m_renderingNodeList.end(), SceneNode::CmpZAndPrioritySort);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingLayer::Render()
{
	////RenderingParams params;
	//params.Renderer = m_manager->GetGraphicsManager()->GetRenderer();
	//params.GeometryRenderer = m_manager->GetGeometryRenderer();
	//params.CurrentCamera = m_camera;
	//LN_FOREACH(RenderingPass* pass, *m_renderingPassList)
	//{
	//	params.Pass = pass;	// TODO: いらないかも
	//	LN_FOREACH(SceneNode* node, m_renderingNodeList)
	//	{
	//		pass->RenderNode(params, node);
	//	}
	//	pass->PostRender(params);
	//}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
