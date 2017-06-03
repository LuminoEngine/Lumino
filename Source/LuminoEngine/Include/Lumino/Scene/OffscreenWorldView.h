
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

LN_INTERNAL_ACCESS:
	OffscreenWorldView();
	virtual ~OffscreenWorldView();
	void Initialize();

private:
	// TODO: ‹“_‚ğŒˆ‚ß‚é‚½‚ß‚Ì“Áê‚È CameraComponentBWorld ‚Ö‚Ì’Ç‰Á‚Í‚Å‚«‚È‚¢‚È‚Ç‚Ì§ŒÀ‚ğ‚µ‚Ä‚¨‚«‚½‚¢B
	RefPtr<CameraComponent>	m_cameraInfo;
};

LN_NAMESPACE_END
