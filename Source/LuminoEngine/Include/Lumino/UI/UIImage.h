
#pragma once
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIImage
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIImage> Create();
	static RefPtr<UIImage> Create(const StringRef& filePath);


	virtual void OnRender(DrawList* g) override
	{
		UIElement::OnRender(g);
	}

LN_CONSTRUCT_ACCESS:
	UIImage();
	virtual ~UIImage();
	void Initialize();
	void Initialize(const StringRef& filePath);

private:
	RefPtr<TextureBrush>	m_brush;
};

LN_NAMESPACE_END
