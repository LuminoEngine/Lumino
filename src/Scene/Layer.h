
#pragma once

#include "Common.h"
#include "Camera.h"

namespace Lumino
{
namespace Scene
{
class Layer
	: public RefObject
{
public:
	Layer() {}
	virtual ~Layer() {}

public:

	/// ‘O•`‰æ
	virtual void PreRender(const SizeF& viewSize) {}

	/// –{•`‰æ
	virtual void Render() {}

	/// Œã•`‰æ
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

	SceneNodeList					m_renderingNodeList;	///< Ž‹Ž‘äƒJƒŠƒ“ƒO“™‚ðs‚Á‚½Œã‚ÌAŽÀÛ‚É•`‰æ‚·‚é‚×‚«ƒm[ƒh‚ÌƒŠƒXƒg
	LightNodeList					m_renderingLightList;	///< •`‰æƒ‹[ƒgˆÈ‰º‚Ìƒ‰ƒCƒg (‘¼‚Ì•`‰æ‹óŠÔ‚Éƒ‰ƒCƒeƒBƒ“ƒO‚Ì‰e‹¿‚ð—^‚¦‚È‚¢‚æ‚¤‚É‚·‚é‚½‚ß)
};

} // namespace Scene
} // namespace Lumino
