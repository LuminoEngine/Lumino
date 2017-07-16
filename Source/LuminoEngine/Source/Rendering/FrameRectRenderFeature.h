
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Graphics/Brush.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

struct FrameRectRendererState
{
	//Matrix						worldTransform;
	Matrix						viewProjTransform;
	BrushImageDrawMode			imageDrawMode;
	ThicknessF					borderThickness;
	RectI						srcRect;
	BrushWrapMode				wrapMode;
	Ref<Driver::ITexture>	texture;
};

class FrameRectRendererCore
	: public RefObject
{
public:
	FrameRectRendererCore();
	~FrameRectRendererCore();
	void initialize(GraphicsManager* manager);

	void setState(const FrameRectRendererState& state);
	void draw(const Matrix& transform, const Rect& rect);

private:
	void requestBuffers(int faceCount);

	void putRectangleStretch(const Rect& rect, const Rect& srcUVRect);
	void putRectangleTiling(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture);
	void putRectangle(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture, BrushWrapMode wrapMode);
	void putFrameRectangle(const Rect& rect, const ThicknessF& borderThickness, Driver::ITexture* srcTexture, RectI srcRect, BrushWrapMode wrapMode);

	//// TODO 頂点宣言とかは外部からもらうようにしたい
	//struct Vertex
	//{
	//	Vector3	Position;			///< 位置
	//	Vector4	Color;				///< 頂点カラー
	//	Vector4	UVOffset;		///< テクスチャUV (転送元のUV情報)
	//	Vector2	UVTileUnit;		///< テクスチャUV (タイリング空間のどこにいるか)

	//							/// 頂点レイアウト
	//	static VertexElement* Elements()
	//	{
	//		static VertexElement elements[] =
	//		{
	//			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
	//			{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
	//			{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 0 },
	//			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 1 },
	//		};
	//		return elements;
	//	}
	//	static const int ElementCount = 4;
	//};

	GraphicsManager*			m_manager;
	Driver::IRenderer*			m_renderer;
	//Ref<Driver::IVertexDeclaration>	m_vertexDeclaration;
	Driver::IVertexBuffer*		m_vertexBuffer;
	Driver::IIndexBuffer*		m_indexBuffer;
	CacheBuffer<Vertex>			m_vertexCache;
	CacheBuffer<uint16_t>		m_indexCache;
	FrameRectRendererState		m_state;

	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderTechnique*	technique;
		Driver::IShaderPass*		pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varTone;
		Driver::IShaderVariable*	varTexture;
		Driver::IShaderVariable*	varGlyphMaskSampler;
		Driver::IShaderVariable*	varViewportSize;	///< DX9 HLSL 用のピクセルオフセット計算用。GLSL では最適化により消えることもある

	} m_shader;
};

class FrameRectRenderFeature
	: public RefObject
	, public detail::IRenderFeature
{
public:
	FrameRectRenderFeature();
	~FrameRectRenderFeature();
	void initialize(GraphicsManager* manager);

	void setViewInfo(const Matrix& viewProj);
	void setState(Brush* brush);

	void draw(const Matrix& transform, const Rect& rect);

	virtual bool isStandaloneShader() const { return false; }
	virtual void flush() override;
	virtual void onActivated() { }
	virtual void onDeactivated() { flush(); }
	virtual void onSetState(const DrawElementBatch* state);

private:
	void setState(Brush* brush, const Matrix& world, const Matrix& viewProj);

	GraphicsManager*		m_manager;
	FrameRectRendererCore*	m_core;

	Matrix					m_viewProj;
	Ref<Brush>	m_brush;

	//FrameRectRendererState	m_state;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
