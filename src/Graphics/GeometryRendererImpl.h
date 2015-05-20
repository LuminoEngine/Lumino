
#pragma once

#include <Lumino/Graphics/GeometryRenderer.h>
#include "Device/DeviceInterface.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{
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
	: public RefObject
{
public:
	GeometryRendererCore(Device::IGraphicsDevice* device);
	virtual ~GeometryRendererCore();

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
	void SetTexture(Device::ITexture* texture);

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
	RefPtr<Device::IGraphicsDevice>	m_device;
	RefPtr<Device::IRenderer>		m_renderer;
	RefPtr<Device::ITexture>		m_dummyTexture;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
	Device::IShaderPass*			m_currentShaderPass;

	/// シェーダ関係の変数をまとめた構造体
	struct
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderVariable*    varWorldMatrix;
		Device::IShaderVariable*    varViewProjMatrix;
		Device::IShaderVariable*    varTexture;
		Device::IShaderTechnique*   techMainDraw;
		Device::IShaderPass*        passP0;

	} m_shaderParam;

public:

	class BeginPassCommand : public RenderingCommand
	{
		GeometryRendererCore* m_renderer;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer)
		{
			HandleCast<BeginPassCommand>(cmd)->m_renderer = renderer;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->BeginPass();
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class EndPassCommand : public RenderingCommand
	{
		GeometryRendererCore* m_renderer;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer)
		{
			HandleCast<EndPassCommand>(cmd)->m_renderer = renderer;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->EndPass();
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class SetTransformCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Matrix					m_transform;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer, const Matrix& transform)
		{
			HandleCast<SetTransformCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetTransformCommand>(cmd)->m_transform = transform;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetTransform(m_transform);
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class SetViewProjTransformCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Matrix					m_transform;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer, const Matrix& transform)
		{
			HandleCast<SetViewProjTransformCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetViewProjTransformCommand>(cmd)->m_transform = transform;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetViewProjTransform(m_transform);
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class SetTextureCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Device::ITexture*		m_texture;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer, Device::ITexture* texture)
		{
			HandleCast<SetTextureCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetTextureCommand>(cmd)->m_texture = texture;
			LN_SAFE_ADDREF(renderer);
			LN_SAFE_ADDREF(texture);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetTexture(m_texture);
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
			LN_SAFE_RELEASE(m_texture);
		}
	};

	class DrawLineCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Vector3					m_from, m_to;
		ColorF					m_fromColor, m_toColor;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer, const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
		{
			HandleCast<DrawLineCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawLineCommand>(cmd)->m_from = from;
			HandleCast<DrawLineCommand>(cmd)->m_to = to;
			HandleCast<DrawLineCommand>(cmd)->m_fromColor = fromColor;
			HandleCast<DrawLineCommand>(cmd)->m_toColor = toColor;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawLine(m_from, m_to, m_fromColor, m_toColor);
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class DrawSquareCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		Vertex m_vertices[4];

	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer,
			const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4)
			//const Vector3& pos1, const Vector2& uv1, const ColorF& color1,
			//const Vector3& pos2, const Vector2& uv2, const ColorF& color2,
			//const Vector3& pos3, const Vector2& uv3, const ColorF& color3,
			//const Vector3& pos4, const Vector2& uv4, const ColorF& color4)
		{
			HandleCast<DrawSquareCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawSquareCommand>(cmd)->m_vertices[0] = v1;
			HandleCast<DrawSquareCommand>(cmd)->m_vertices[1] = v2;
			HandleCast<DrawSquareCommand>(cmd)->m_vertices[2] = v3;
			HandleCast<DrawSquareCommand>(cmd)->m_vertices[3] = v4;
			//m_pos1 = pos1; m_uv1 = uv1; m_color1 = color1;
			//m_pos2 = pos2; m_uv2 = uv2; m_color2 = color2;
			//m_pos3 = pos3; m_uv3 = uv3; m_color3 = color3;
			//m_pos4 = pos4; m_uv4 = uv4; m_color4 = color4;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawSquare(
				m_vertices[0].Position.X, m_vertices[0].Position.Y, m_vertices[0].Position.Z, m_vertices[0].TexUV.X, m_vertices[0].TexUV.Y, m_vertices[0].Color,
				m_vertices[1].Position.X, m_vertices[1].Position.Y, m_vertices[1].Position.Z, m_vertices[1].TexUV.X, m_vertices[1].TexUV.Y, m_vertices[1].Color,
				m_vertices[2].Position.X, m_vertices[2].Position.Y, m_vertices[2].Position.Z, m_vertices[2].TexUV.X, m_vertices[2].TexUV.Y, m_vertices[2].Color,
				m_vertices[3].Position.X, m_vertices[3].Position.Y, m_vertices[3].Position.Z, m_vertices[3].TexUV.X, m_vertices[3].TexUV.Y, m_vertices[3].Color);
				//m_pos2.X, m_pos2.Y, m_pos2.Z, m_uv2.X, m_uv2.Y, m_color2, 
				//m_pos3.X, m_pos3.Y, m_pos3.Z, m_uv3.X, m_uv3.Y, m_color3, 
				//m_pos4.X, m_pos4.Y, m_pos4.Z, m_uv4.X, m_uv4.Y, m_color4);
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class DrawRectCommand : public RenderingCommand
	{
		GeometryRendererCore*	m_renderer;
		RectF					m_destRect;
		RectF					m_texUVRect;
		ColorF					m_color;
	public:
		static void Create(CmdInfo& cmd, GeometryRendererCore* renderer, const RectF& destRect, const RectF& texUVRect, const ColorF& color)
		{
			HandleCast<DrawRectCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawRectCommand>(cmd)->m_destRect = destRect;
			HandleCast<DrawRectCommand>(cmd)->m_texUVRect = texUVRect;
			HandleCast<DrawRectCommand>(cmd)->m_color = color;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawRect(m_destRect, m_texUVRect, m_color);
		}
		virtual void Release(RenderingCommandList* commandList)
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};
};

} // namespace Graphics
} // namespace Lumino
