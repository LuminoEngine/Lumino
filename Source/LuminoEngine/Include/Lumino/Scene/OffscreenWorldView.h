﻿
#pragma once
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class CameraComponent;
class VisualComponent;

/**
	@brief		
*/
class OffscreenWorldView
	: public Object
{
public:
	//CameraComponent* GetCamera() const;
	RenderTargetTexture* GetRenderTarget() const;

	void HideVisual(VisualComponent* renderObject);

protected:
	virtual void OnUpdateRenderViewPoint(RenderView* renderView);

LN_INTERNAL_ACCESS:
	OffscreenWorldView();
	virtual ~OffscreenWorldView();
	void Initialize();
	void SetId(int id) { m_id = id; }
	int GetId() const { return m_id; }
	void RenderWorld(World* world, CameraComponent* mainViewCamera, RenderView* mainRenderView);
	bool FilterRenderObject(VisualComponent* renderObject);
	detail::OffscreenFilterInfo* UpdateRenderObjectFilterInfo(VisualComponent* renderObject);

private:
	RefPtr<RenderTargetTexture>	m_renderTarget;
	RefPtr<DrawList>			m_renderer;
	RefPtr<RenderView>			m_renderView;
	int							m_id;
};


class MirrorComponent
	: public VisualComponent
{
public:

protected:
	virtual void OnRender2(DrawList* renderer) override;

LN_INTERNAL_ACCESS:
	MirrorComponent();
	virtual ~MirrorComponent();
	void Initialize();

private:
	RefPtr<OffscreenWorldView>	m_offscreen;
	RefPtr<Material>			m_material;
};

LN_NAMESPACE_END
