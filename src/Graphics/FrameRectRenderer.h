
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/ContextInterface.h>
#include "PainterEngine.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

struct FrameRectRendererState
{
	Matrix						worldTransform;
	Matrix						viewProjTransform;
	BrushImageDrawMode			imageDrawMode;
	ThicknessF					borderThickness;
	Rect						srcRect;
	BrushWrapMode				wrapMode;
	RefPtr<Driver::ITexture>	texture;
};

class FrameRectRendererCore
	: public RefObject
{
public:
	FrameRectRendererCore();
	~FrameRectRendererCore();
	void Initialize(GraphicsManager* manager);

	void SetState(const FrameRectRendererState& state);
	void Draw(const RectF& rect);

private:
	void RequestBuffers(int faceCount);

	void PutRectangleStretch(const RectF& rect, const RectF& srcUVRect);
	void PutRectangleTiling(const RectF& rect, const Rect& srcPixelRect, const RectF& srcUVRect, Driver::ITexture* srcTexture);
	void PutFrameRectangle(const RectF& rect, const ThicknessF& borderThickness, Driver::ITexture* srcTexture, Rect srcRect);

	// TODO 頂点宣言とかは外部からもらうようにしたい
	struct Vertex
	{
		Vector3	Position;			///< 位置
		Vector4	Color;				///< 頂点カラー
		Vector4	UVOffset;		///< テクスチャUV (転送元のUV情報)
		Vector2	UVTileUnit;		///< テクスチャUV (タイリング空間のどこにいるか)

								/// 頂点レイアウト
		static VertexElement* Elements()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_TexCoord, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 1 },
			};
			return elements;
		}
		static const int ElementCount = 4;
	};

	GraphicsManager*			m_manager;
	Driver::IRenderer*			m_renderer;
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
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

class FrameRectRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	FrameRectRenderer();
	~FrameRectRenderer();
	void Initialize(GraphicsManager* manager);

	void SetState(TextureBrush* brush, const Matrix& world, const Matrix& viewProj);

	void Draw(const RectF& rect);

	virtual void Flush() override;
	virtual void OnActivated() { }
	virtual void OnDeactivated() { Flush(); }

private:
	GraphicsManager*		m_manager;
	FrameRectRendererCore*	m_core;
	//FrameRectRendererState	m_state;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
