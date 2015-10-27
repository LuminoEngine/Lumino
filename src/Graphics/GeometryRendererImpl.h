
#pragma once

#include <Lumino/Graphics/GeometryRenderer.h>
#include "Device/GraphicsDriverInterface.h"
#include "RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/// GeometryRenderer の実装クラス
class GeometryRendererImpl
	: public GeometryRenderer
{
public:
	GeometryRendererImpl(GraphicsManager* manager);
	virtual ~GeometryRendererImpl();
};

/// GeometryRenderer のコマンド発行クラス
class GeometryRendererProxy
{
public:
	GeometryRendererProxy(GraphicsManager* manager);
	virtual ~GeometryRendererProxy();
};

/// GeometryRenderer の描画クラス
///		このクラスは描画スレッドで生成され、各機能が呼ばれる。
///		他のスレッドからはアクセスできない。
class GeometryRendererCore
	: public GraphicsResourceObject
{
public:
	GeometryRendererCore(GraphicsManager* manager, Driver::IGraphicsDevice* device);
	virtual ~GeometryRendererCore();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);
	void CreateInternal();

public:

	/// 描画パスの開始 (呼び出しは任意。呼び出さない場合、Draw～ 系はただ頂点バッファを DrawPrimitive するだけになる)
	void BeginPass();

	/// 描画パスの終了
	void EndPass();

	/// 座標変換行列の設定
	void SetTransform(const Matrix& matrix);

	/// 座標変換行列の設定
	void SetViewProjTransform(const Matrix& matrix);

	/// テクスチャの設定 (NULL でダミーテクスチャを使用する)
	void SetTexture(Driver::ITexture* texture);

	/// 線分の描画
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);

	/// 四角形の描画
	void DrawSquare(
		float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
		float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
		float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
		float x4, float y4, float z4, float u4, float v4, const ColorF& c4);

	/// 矩形の描画     
	void DrawRect(const RectF& destRect, const RectF& texUVRect, const ColorF& color);

public:

	/// 描画用の頂点
	struct Vertex
	{
		Vector3		Position;   ///< 座標
		ColorF		Color;      ///< 頂点カラー
		Vector2		TexUV;      ///< テクスチャ座標

		static VertexElement* GetLayout()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
	};

private:
	GraphicsManager*				m_manager;
	RefPtr<Driver::IGraphicsDevice>	m_device;
	RefPtr<Driver::IRenderer>		m_renderer;
	RefPtr<Driver::ITexture>		m_dummyTexture;
	RefPtr<Driver::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Driver::IIndexBuffer>	m_indexBuffer;
	Driver::IShaderPass*			m_currentShaderPass;

	/// シェーダ関係の変数をまとめた構造体
	struct
	{
		RefPtr<Driver::IShader>		Shader;
		Driver::IShaderVariable*    varWorldMatrix;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varTexture;
		Driver::IShaderTechnique*   techMainDraw;
		Driver::IShaderPass*        passP0;

	} m_shaderParam;

public:

	struct BeginPassCommand : public RenderingCommand
	{
		GeometryRendererCore* m_renderer;
		void Create(GeometryRendererCore* renderer) { m_renderer = renderer; MarkGC(m_renderer); }
		void Execute() { m_renderer->BeginPass(); }
	};

	struct EndPassCommand : public RenderingCommand
	{
		GeometryRendererCore* m_renderer;
		void Create(GeometryRendererCore* renderer) { m_renderer = renderer; MarkGC(m_renderer); }
		void Execute() { m_renderer->EndPass(); }
	};

	struct SetTransformCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Matrix					m_transform;
		void Create(GeometryRendererCore* renderer, const Matrix& transform)
		{
			m_renderer = renderer;
			m_transform = transform;
			MarkGC(m_renderer);
		}
		void Execute() { m_renderer->SetTransform(m_transform); }
	};

	struct SetViewProjTransformCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Matrix					m_transform;
		void Create(GeometryRendererCore* renderer, const Matrix& transform)
		{
			m_renderer = renderer;
			m_transform = transform;
			MarkGC(m_renderer);
		}
		void Execute(){ m_renderer->SetViewProjTransform(m_transform); }
	};

	struct SetTextureCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Driver::ITexture*		m_texture;
		void Create(GeometryRendererCore* renderer, Driver::ITexture* texture)
		{
			m_renderer = renderer;
			m_texture = texture;
			MarkGC(m_renderer);
			MarkGC(m_texture);
		}
		void Execute() { m_renderer->SetTexture(m_texture); }
	};

	struct DrawLineCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Vector3					m_from, m_to;
		ColorF					m_fromColor, m_toColor;
		void Create(GeometryRendererCore* renderer, const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
		{
			m_renderer = renderer;
			m_from = from;
			m_to = to;
			m_fromColor = fromColor;
			m_toColor = toColor;
			MarkGC(m_renderer);
		}
		virtual void Execute(){ m_renderer->DrawLine(m_from, m_to, m_fromColor, m_toColor); }
	};

	struct DrawSquareCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Vertex m_vertices[4];

		void Create(GeometryRendererCore* renderer,
			const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
		{
			m_renderer = renderer;
			m_vertices[0] = v1;
			m_vertices[1] = v2;
			m_vertices[2] = v3;
			m_vertices[3] = v4;
			MarkGC(m_renderer);
		}
		void Execute()
		{
			m_renderer->DrawSquare(
				m_vertices[0].Position.X, m_vertices[0].Position.Y, m_vertices[0].Position.Z, m_vertices[0].TexUV.X, m_vertices[0].TexUV.Y, m_vertices[0].Color,
				m_vertices[1].Position.X, m_vertices[1].Position.Y, m_vertices[1].Position.Z, m_vertices[1].TexUV.X, m_vertices[1].TexUV.Y, m_vertices[1].Color,
				m_vertices[2].Position.X, m_vertices[2].Position.Y, m_vertices[2].Position.Z, m_vertices[2].TexUV.X, m_vertices[2].TexUV.Y, m_vertices[2].Color,
				m_vertices[3].Position.X, m_vertices[3].Position.Y, m_vertices[3].Position.Z, m_vertices[3].TexUV.X, m_vertices[3].TexUV.Y, m_vertices[3].Color);
		}
	};

	struct DrawRectCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		RectF					m_destRect;
		RectF					m_texUVRect;
		ColorF					m_color;

		void Create(GeometryRendererCore* renderer, const RectF& destRect, const RectF& texUVRect, const ColorF& color)
		{
			m_renderer = renderer;
			m_destRect = destRect;
			m_texUVRect = texUVRect;
			m_color = color;
			MarkGC(m_renderer);
		}
		void Execute()
		{
			m_renderer->DrawRect(m_destRect, m_texUVRect, m_color);
		}
	};
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
