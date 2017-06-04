﻿
#pragma once

LN_NAMESPACE_BEGIN
class CameraComponent;

/**
	@brief		
*/
class OffscreenWorldView
	: public Object
{
public:
	CameraComponent* GetCamera() const;
	RenderTargetTexture* GetRenderTarget() const;

LN_INTERNAL_ACCESS:
	OffscreenWorldView();
	virtual ~OffscreenWorldView();
	void Initialize();
	void RenderWorld(World* world);

private:
	// TODO: 視点を決めるための特殊な CameraComponent。World への追加はできないなどの制限をしておきたい。
	RefPtr<CameraComponent>		m_cameraInfo;
	RefPtr<RenderTargetTexture>	m_renderTarget;
};

class MirrorComponent
	: public Component
{
public:

LN_INTERNAL_ACCESS:
	MirrorComponent();
	virtual ~MirrorComponent();
	void Initialize();

private:
	RefPtr<OffscreenWorldView>	m_offscreen;
};

LN_NAMESPACE_END
