
#pragma once
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class GlyphIconComponent;

/**
	@brief		Font Awesome アイコンの描画を行うクラスです。
	@details	http://fontawesome.io/icons/
*/
LN_CLASS()
class GlyphIcon2D
	: public VisualObject
{
	LN_OBJECT;
public:
	
	/**
		@brief		
	*/
	static Ref<GlyphIcon2D> create();
	
	/**
		@brief		
	*/
	static Ref<GlyphIcon2D> create(const StringRef& glyphName);

	/**
		@brief		
	*/
	static Ref<GlyphIcon2D> create(const StringRef& glyphName, int glyphSize);

	void setGlyphName(const StringRef& glyphName);
	void setGlyphSize(int glyphSize);

	//void setAnchorPoint(const Vector2& ratio);
	//void setAnchorPoint(float ratioX, float ratioY);		/**< @overload setAnchorPoint */

protected:
	virtual VisualComponent* getMainVisualComponent() const override;

LN_CONSTRUCT_ACCESS:
	GlyphIcon2D();
	virtual ~GlyphIcon2D();

	LN_METHOD()
	void initialize();

	LN_METHOD()
	void initialize(const StringRef& glyphName);

	LN_METHOD()
	void initialize(const StringRef& glyphName, int glyphSize);

private:
	Ref<GlyphIconComponent>	m_component;
};

/** */
class GlyphIconComponent
	: public VisualComponent
{
	LN_OBJECT;
public:
	void setGlyphName(const StringRef& glyphName);
	void setGlyphSize(int glyphSize);
	void setAnchorPoint(const Vector2& ratio);

protected:
	virtual void onRender2(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	GlyphIconComponent();
	virtual ~GlyphIconComponent();
	void initialize();

private:
	Vector2			m_anchor;
	Ref<Font>		m_font;
	uint32_t		m_codePoint;
	Size			m_renderSize;
};

LN_NAMESPACE_END
