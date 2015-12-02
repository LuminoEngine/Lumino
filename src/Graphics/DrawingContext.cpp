/*
	[2015/12/2] タイリング描画について
		デフォルトでは有効にしない。シェーダは分ける。
		タイル配置するブラシが指定された場合、描画モードを切り替える。
		常に有効にするにはあまりにも複雑すぎる。


	[2015/12/2] コマンドのキャッシュについて
		同一ステート間の Primitive 描画コマンドは一括で実行できる。
		まとめてコマンドリストに送るといいのだが、バッファは誰が持つの？
		→ ステートの切り替えは割りと頻繁だと思われる。
		むしろ new の数が増えてしまうかもしれない。
		memcpy でコマンドリストの一時バッファへコピーするほうが全体的に見てパフォーマンス良いかも。
		sizeof(Matrix) を1コマンド分の平均サイズとして 10000 子 memcpy するときは 30us くらいかかる。
		1000 個なら 3us。
		実際にはこんなに使わないだろうし、memcpy で問題ないはず。


	[2015/12/2] クラス名
		しばらく間空いて、やっぱり DrawingContext のほうがいいと思う。
		このライブラリはかなり低レベルなAPIも公開する。
		そうなってくると DirectX11 の RenderingContext の意味が強くなってくる。
		高レベルってことで差別化するなら DrawingContext。
		


	名前の元ネタはこのあたりから。
	https://developer.mozilla.org/ja/docs/Web/API/RenderingContext

	このモジュールは Scene と GUI で共有されるが、
	DrawingContext は Scene よりも GUI 寄り。
	ライブラリとしては Scene の方が意味が大きい。Draw より Render かな、と。


	Scene では、あらかじめ SceneNode のシェーダをユーザーシェーダとして
	設定したものを OnRender() に渡す。



	ユーザーシェーダに要求する頂点宣言は、
	・Pos0
	・TexUV0
	・Color0

	ユーザーシェーダに要求するパラメータは、
	・VIEWPORTPIXELSIZE
	・LNRC_TONE
		UI_TONE、NODE_TONE とかのほうがいいかも？
	↑Tone の計算はユーザーシェーダで色が決まった後に行わなければならない。
	（乗算だけで表現できないので頂点宣言だけでは不足）
	#include "lumino.fx" とかして、ピクセルシェーダで LNUI_PostPixelShader みたいな関数を
	ユーザーに呼んでもらう必要がある。

	描画方式は3つ。
	・プリミティブのみ
	・組み込みシェーダを使う
	・ユーザーシェーダを使う

	RenderingContext
	BasicRenderingContext
	UIRenderingContext
	SceneNodeRenderingContext
*/

#include "Internal.h"
#include <algorithm>
#include "PainterEngine.h"	// CacheBuffer
#include "RenderingCommand.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/DrawingContext.h>

LN_NAMESPACE_BEGIN


//class DrawingPrimitiveCommandList
//{
//public:
//
//protected:
//	DrawingContext();
//	virtual ~DrawingContext();
//	void Initialize(GraphicsManager* manager);
//
//private:
//	GraphicsManager*	m_manager;
//};


//=============================================================================
// DrawingCommands
//=============================================================================
enum class DrawingCommandType : uint32_t
{
	DrawLine = 0,
};
struct DrawingCommands_DrawLine
{
	DrawingCommandType	type;
	Vector3				from;
	Vector3				to;
	ColorF				fromColor;
	ColorF				toColor;
};


//=============================================================================
// PrimitiveRenderer
//=============================================================================
class PrimitiveRenderer
{
public:
	PrimitiveRenderer();
	virtual ~PrimitiveRenderer();

	void Clear();
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);

private:
	struct PainterVertex
	{
	public:

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

	CacheBuffer<PainterVertex>		m_vertexCache;
	CacheBuffer<uint16_t>			m_indexCache;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRenderer::PrimitiveRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PrimitiveRenderer::~PrimitiveRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::Clear()
{
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PrimitiveRenderer::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	//PainterVertex v;
	//v.Position = from;
	//v.Color = fromColor;
	//m_vertexCache.Add(v);
	//v.Position = to;
	//v.Color = toColor;
	//m_vertexCache.Add(v);
}




//=============================================================================
// DrawingContextImpl
//=============================================================================
class DrawingContextImpl
	: public RefObject
{
public:
	DrawingContextImpl(GraphicsManager* manager);
	~DrawingContextImpl();

	void DoCommandList(const void* commandBuffer, size_t size);
	void Flush();

public:
	struct DoCommandListCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		DataHandle m_commandBuffer;
		size_t	m_size;
		void Create(DrawingContextImpl* engine, const void* commandBuffer, size_t size)
		{
			m_impl = engine;
			m_commandBuffer = AllocExtData(size, commandBuffer);
			m_size = size;
		}
		void Execute() { m_impl->DoCommandList(GetExtData(m_commandBuffer), m_size); }
	};

	struct FlushCommand : public RenderingCommand
	{
		DrawingContextImpl* m_impl;
		void Create(DrawingContextImpl* engine) { m_impl = engine; }
		void Execute() { m_impl->Flush(); }
	};

private:
	GraphicsManager* m_manager;
	PrimitiveRenderer	m_primitiveRenderer;

	// DrawingContext3D
	struct
	{
		RefPtr<Driver::IShader>		shader;
		Driver::IShaderVariable*    varWorldMatrix;
		Driver::IShaderVariable*    varViewProjMatrix;
		Driver::IShaderVariable*    varTexture;
		Driver::IShaderTechnique*   techMainDraw;
		Driver::IShaderPass*        passP0;

	} m_shader3D;
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::DrawingContextImpl(GraphicsManager* manager)
	: m_manager(manager)
{
	Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();

	//-----------------------------------------------------
	// シェーダ (DrawingContext3D)
	const unsigned char code[] =
	{
#include "Resource/DrawingContext3D.fx.h"
	};
	const size_t codeLen = LN_ARRAY_SIZE_OF(code);

	ShaderCompileResult result;
	m_shader3D.shader.Attach(device->CreateShader(code, codeLen, &result), false);
	if (result.Level != ShaderCompileResultLevel_Success) {
		printf(result.Message.c_str());	// TODO:仮
	}

	m_shader3D.varWorldMatrix = m_shader3D.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader3D.varViewProjMatrix = m_shader3D.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader3D.varTexture = m_shader3D.shader->GetVariableByName(_T("g_texture"));
	m_shader3D.techMainDraw = m_shader3D.shader->GetTechnique(0);
	m_shader3D.passP0 = m_shader3D.techMainDraw->GetPass(0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContextImpl::~DrawingContextImpl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::DoCommandList(const void* commandBuffer, size_t size)
{
	const byte_t* pos = (const byte_t*)commandBuffer;
	const byte_t* end = pos + size;
	detail::DrawingClass c = *((const detail::DrawingClass*)pos);
	pos += sizeof(detail::DrawingClass);

	while (pos < end)
	{
		DrawingCommandType type = *((const DrawingCommandType*)pos);
		switch (type)
		{
			case DrawingCommandType::DrawLine:
			{
				const DrawingCommands_DrawLine* cmd = (const DrawingCommands_DrawLine*)pos;
				m_primitiveRenderer.DrawLine(cmd->from, cmd->to, cmd->fromColor, cmd->toColor);
				pos += sizeof(DrawingCommands_DrawLine);
				break;
			}
			default:
				LN_THROW(0, InvalidOperationException);
				return;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContextImpl::Flush()
{

}


//=============================================================================
// DrawingContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::DrawingContext()
	: m_manager(nullptr)
	, m_commandsUsingByte(0)
	, m_internal(nullptr)
	, m_currentDrawingClass(detail::DrawingClass::Unknown)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
	LN_SAFE_RELEASE(m_internal);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_internal = LN_NEW	DrawingContextImpl();
	m_commandsBuffer.Resize(1024);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	DrawingCommands_DrawLine cmd;
	cmd.type = DrawingCommandType::DrawLine;
	cmd.from = from;
	cmd.to = to;
	cmd.fromColor = fromColor;
	cmd.toColor = toColor;
	AddCommand(&cmd, sizeof(cmd));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::DrawLine(const Vector3& from, const Vector3& to, const ColorF& color)
{
	CheckFlush(m_currentDrawingClass);
	DrawLine(from, to, color, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::Flush()
{
	FlushInternal();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::AddCommand(const void* command, size_t size)
{
	// バッファが足りなければ拡張する
	if (m_commandsUsingByte + size > m_commandsBuffer.GetSize()) {
		size_t newSize = m_commandsBuffer.GetSize() + std::max(m_commandsBuffer.GetSize(), size);	// 最低でも size 分を拡張する
		m_commandsBuffer.Resize(newSize, false);
	}

	// データコピー
	if (command != NULL)
	{
		byte_t* ptr = &(m_commandsBuffer.GetData()[m_commandsUsingByte]);
		size_t bufSize = m_commandsBuffer.GetSize() - m_commandsUsingByte;
		memcpy_s(ptr, bufSize, command, size);
	}

	// バッファ使用量を増やす
	m_commandsUsingByte += size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::FlushInternal()
{
	LN_CALL_COMMAND(Flush, DrawingContextImpl::FlushCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DrawingContext::CheckFlush(detail::DrawingClass newDrawingClass)
{
	if (m_currentDrawingClass != newDrawingClass &&
		m_currentDrawingClass != detail::DrawingClass::Unknown) {
		FlushInternal();
	}
	m_currentDrawingClass = newDrawingClass;

	// コマンドバッファの先頭に区分を入れておく
	AddCommand(&m_currentDrawingClass, sizeof(uint32_t));
}

LN_NAMESPACE_END
