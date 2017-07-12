
#pragma once
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIImage
	: public UIElement
{
	LN_OBJECT();
public:
	static RefPtr<UIImage> create();
	static RefPtr<UIImage> create(const StringRef& filePath);


	void setStretchType(StretchMode mode);
	StretchMode getStretchType();

protected:
	virtual Size measureOverride(const Size& availableSize) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(DrawingContext* g) override;

LN_CONSTRUCT_ACCESS:
	UIImage();
	virtual ~UIImage();
	void initialize();
	void initialize(const StringRef& filePath);

private:
	Size measureInternal(Size contentSize);

	//RefPtr<TextureBrush>	m_brush;
	RefPtr<Texture>			m_texture;
	StretchMode				m_stretchMode;
};

LN_NAMESPACE_END
