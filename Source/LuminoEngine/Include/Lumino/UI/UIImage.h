
#pragma once
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIImage
	: public UIElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static RefPtr<UIImage> Create();
	static RefPtr<UIImage> Create(const StringRef& filePath);


	void SetStretchType(StretchMode mode);
	StretchMode GetStretchType();

protected:
	virtual Size MeasureOverride(const Size& availableSize) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRender(DrawingContext* g) override;

LN_CONSTRUCT_ACCESS:
	UIImage();
	virtual ~UIImage();
	void initialize();
	void initialize(const StringRef& filePath);

private:
	Size MeasureInternal(Size contentSize);

	//RefPtr<TextureBrush>	m_brush;
	RefPtr<Texture>			m_texture;
	StretchMode				m_stretchMode;
};

LN_NAMESPACE_END
