
#pragma once
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class CameraComponent;
class VisualComponent;

/**
	@brief		
*/
class OffscreenWorldView
	: public RenderView
{
public:
	RenderTargetTexture* getRenderTarget() const;

	void hideVisual(VisualComponent* renderObject);

protected:
	virtual Matrix calculateViewMatrix(CameraComponent* mainViewCamera);
	virtual Matrix calculateProjectionMatrix(CameraComponent* mainViewCamera);

LN_INTERNAL_ACCESS:
	OffscreenWorldView();
	virtual ~OffscreenWorldView();
	void initialize();
	void setId(int id) { m_id = id; }
	int getId() const { return m_id; }
	void renderWorld(World* world, CameraComponent* mainViewCamera, RenderView* mainRenderView);
	bool filterRenderObject(VisualComponent* renderObject);
	detail::OffscreenFilterInfo* updateRenderObjectFilterInfo(VisualComponent* renderObject);

private:
	RefPtr<RenderTargetTexture>	m_renderTarget;
	RefPtr<DrawList>			m_renderer;
	int							m_id;
};



class SkyComponent
	: public VisualComponent
{
public:

protected:
	virtual void onRender2(DrawList* renderer) override;

LN_INTERNAL_ACCESS:
	SkyComponent();
	virtual ~SkyComponent();
	void initialize();

private:

	RefPtr<Material>			m_skyMaterial;
};

class MirrorComponent
	: public VisualComponent
{
public:

protected:
	virtual void onRender2(DrawList* renderer) override;

LN_INTERNAL_ACCESS:
	MirrorComponent();
	virtual ~MirrorComponent();
	void initialize();

private:
	RefPtr<OffscreenWorldView>	m_offscreen;
	RefPtr<Material>			m_material;

};

LN_NAMESPACE_END
