
#pragma once
#include "Internal.h"
#include <Lumino/Scene/Camera.h>

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

class Layer
	: public RefObject
{
public:
	Layer() {}
	virtual ~Layer() {}

public:

	/// 前描画
	virtual void PreRender(const SizeF& viewSize) {}

	/// 本描画
	virtual void Render() {}

	/// 後描画
	virtual void PostRender() {}

private:
	friend class LayerList;

};


class RenderingPassRefList
	: public RefObjectListBase<RenderingPass>
{
public:
	RenderingPassRefList() {}
	virtual ~RenderingPassRefList() {}
};

class DrawingLayer
	: public Layer
{
public:
	DrawingLayer(SceneGraphManager* manager);
	virtual ~DrawingLayer();
	
public:
	void SetCamera(Camera* camera)  { LN_REFOBJ_SET(m_camera, camera); }
	Camera* GetCamera() const { return m_camera; }
	void SetRenderingRootNode(SceneNode* node) { LN_REFOBJ_SET(m_renderingRootNode, node); }
	SceneNode* GetRenderingRootNode() const { return m_renderingRootNode; }
	RenderingPassRefList* GetRenderingPasses() { return m_renderingPassList; }
	

	virtual void PreRender(const SizeF& viewSize);
	virtual void Render();

private:

private:
	SceneGraphManager*				m_manager;
	Camera*							m_camera;
	SceneNode*						m_renderingRootNode;
	RefPtr<RenderingPassRefList>	m_renderingPassList;

	SceneNodeList					m_renderingNodeList;	///< 視錘台カリング等を行った後の、実際に描画するべきノードのリスト
	LightNodeList					m_renderingLightList;	///< 描画ルート以下のライト (他の描画空間にライティングの影響を与えないようにするため)
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
