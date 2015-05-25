
#include "../../../Internal.h"
#include "DX9GraphicsDevice.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9Shader.h"
#include "DX9Renderer.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// DX9Renderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9Renderer::DX9Renderer(DX9GraphicsDevice* device)
	: m_dxDevice(NULL)
	, m_currentRenderState()
	, m_currentViewportRect()
	, m_currentVertexBuffer(NULL)
	, m_currentIndexBuffer(NULL)
	, m_currentDepthBuffer(NULL)
	, m_currentShaderPass(NULL)
	, m_sceneBegan(false)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));

	m_dxDevice = device->GetIDirect3DDevice9();

	D3DVIEWPORT9 vp;
	m_dxDevice->GetViewport(&vp);
	SetViewport(Rect(0, 0, vp.Width, vp.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9Renderer::~DX9Renderer()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetCurrentShaderPass(DX9ShaderPass* pass)
{
	if (m_currentShaderPass != NULL && m_currentShaderPass != pass) {
		m_currentShaderPass->EndPass();
	}
	LN_REFOBJ_SET(m_currentShaderPass, pass);	// TODO: DX9Shader の参照カウントも増やさないとまずいかも
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::OnLostDevice()
{
	TryEndScene();

	LN_SAFE_RELEASE(m_currentVertexBuffer);
	LN_SAFE_RELEASE(m_currentIndexBuffer);
	LN_SAFE_RELEASE(m_currentDepthBuffer);
	for (int i = 0; i < MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::OnResetDevice()
{
	InternalRenderStaet(m_currentRenderState, true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::TryBeginScene()
{
	if (!m_sceneBegan) {
		m_dxDevice->BeginScene();
		m_sceneBegan = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::TryEndScene()
{
	if (m_sceneBegan)
	{
		// アクティブなパスもここで終了しておく
		if (m_currentShaderPass != NULL) {
			m_currentShaderPass->EndPass();
		}
		LN_SAFE_RELEASE(m_currentShaderPass);

		m_dxDevice->EndScene();
		m_sceneBegan = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetRenderState(const RenderState& state)
{
	TryBeginScene();
	InternalRenderStaet(state, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const RenderState& DX9Renderer::GetRenderState()
{
	return m_currentRenderState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetRenderTarget(int index, ITexture* texture)
{
	TryBeginScene();
	InternalSetRenderTarget(index, texture, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* DX9Renderer::GetRenderTarget(int index)
{
	return m_currentRenderTargets[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetDepthBuffer(ITexture* texture)
{
	TryBeginScene();
	InternalSetDepthBuffer(texture, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* DX9Renderer::GetDepthBuffer()
{
	return m_currentDepthBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetViewport(const Rect& rect)
{
	TryBeginScene();
	InternalSetViewport(rect, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Rect& DX9Renderer::GetViewport()
{
	return m_currentViewportRect;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetVertexBuffer(IVertexBuffer* vertexBuffer)
{
	TryBeginScene();
	InternalSetVertexBuffer(vertexBuffer, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::SetIndexBuffer(IIndexBuffer* indexBuffer)
{
	TryBeginScene();
	InternalSetIndexBuffer(indexBuffer, false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::Clear(bool target, bool depth, const ColorF& color, float z)
{
	TryBeginScene();

	// ※レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	//   していない場合、エラーとならないがクリアされない。

	DWORD flag = 0;
	if (target) flag |= D3DCLEAR_TARGET;
	if (m_currentDepthBuffer && depth) flag |= (D3DCLEAR_ZBUFFER);// | D3DCLEAR_STENCIL
	if (flag == 0) return;

	Color c(color);
	D3DCOLOR dxc = D3DCOLOR_ARGB(c.A, c.R, c.G, c.B);
	LN_COMCALL(m_dxDevice->Clear(0, NULL, flag, dxc, z, 0x00000000));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	TryBeginScene();

	D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
	switch (primitive)
	{
		//case LN_PRIMITIVE_TRIANGLELIST:   dx_prim = D3DPT_TRIANGLELIST; break;
	case PrimitiveType_TriangleStrip:
		dx_prim = D3DPT_TRIANGLESTRIP; break;
	case PrimitiveType_LineList:
		dx_prim = D3DPT_LINELIST; break;
	case PrimitiveType_LineStrip:
		dx_prim = D3DPT_LINESTRIP; break;
	case PrimitiveType_PointList:
		dx_prim = D3DPT_POINTLIST; break;
	}

	// 動的な頂点バッファとして作成している場合は DrawPrimitiveUP で描画する
	if (m_currentVertexBuffer->GetUsage() == DeviceResourceUsage_Dynamic)
	{
		const byte_t* vertices = (const byte_t*)m_currentVertexBuffer->Lock();
		int stride = m_currentVertexBuffer->GetVertexStride();
		vertices += stride * startVertex;

		LN_COMCALL(m_dxDevice->DrawPrimitiveUP(dx_prim, primitiveCount, vertices, stride));
	}
	else
	{
		LN_COMCALL(m_dxDevice->DrawPrimitive(dx_prim, startVertex, primitiveCount));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	TryBeginScene();

	D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
	switch (primitive)
	{
		//case LN_PRIMITIVE_TRIANGLELIST:     dx_prim = D3DPT_TRIANGLELIST; break;
	case PrimitiveType_TriangleStrip:
		dx_prim = D3DPT_TRIANGLESTRIP; break;
	case PrimitiveType_LineList:
		dx_prim = D3DPT_LINELIST; break;
	case PrimitiveType_LineStrip:
		dx_prim = D3DPT_LINESTRIP; break;
	case PrimitiveType_PointList:
		dx_prim = D3DPT_POINTLIST; break;
	}

	bool vd = (m_currentVertexBuffer->GetUsage() == DeviceResourceUsage_Dynamic);
	bool id = (m_currentIndexBuffer->GetUsage() == DeviceResourceUsage_Dynamic);

	// 両方が dynamic
	if (vd && id)
	{
		D3DFORMAT ib_fmt;
		const void* index_data;
		if (m_currentIndexBuffer->GetFormat() == IndexBufferFormat_UInt16)
		{
			ib_fmt = D3DFMT_INDEX16;
			index_data = &((uint16_t*)m_currentIndexBuffer->GetDynamicData())[startIndex];
		}
		else
		{
			ib_fmt = D3DFMT_INDEX32;
			index_data = &((uint32_t*)m_currentIndexBuffer->GetDynamicData())[startIndex];
		}

		LN_COMCALL(
			m_dxDevice->DrawIndexedPrimitiveUP(
			dx_prim,
			0,
			m_currentVertexBuffer->GetVertexCount(),
			primitiveCount,
			index_data,
			ib_fmt,
			m_currentVertexBuffer->Lock(),
			m_currentVertexBuffer->GetVertexStride()));
	}
	// 両方が static
	else if (!vd && !id)
	{
		LN_COMCALL(
			m_dxDevice->DrawIndexedPrimitive(
			dx_prim,
			0,
			0,
			m_currentVertexBuffer->GetVertexCount(),
			startIndex,
			primitiveCount));
	}
	else {
		LN_THROW(0, InvalidOperationException)
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::InternalRenderStaet(const RenderState& state, bool reset)
{
	// 合成方法
	if (state.Blend != m_currentRenderState.Blend || reset)
	{
		switch (state.Blend)
		{
		case BlendMode_Normal:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 127);
			break;
		case BlendMode_Alpha:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			break;
		case BlendMode_Add:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			break;
		case BlendMode_AddAlphaDisable:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			break;
		case BlendMode_Sub:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			break;
		case BlendMode_SubAlphaDisable:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			break;
		case BlendMode_Mul:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			break;
		case BlendMode_Screen:
			m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
			m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
			m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
			break;
		//case BlendMode_Reverse:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		//	break;
		}
	}

	// カリング
	if (state.Culling != m_currentRenderState.Culling || reset)
	{
		const uint32_t tb[] = { D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE };
		m_dxDevice->SetRenderState(D3DRS_CULLMODE, tb[state.Culling]);
	}
	// 塗りつぶし方法
	if (state.Fill != m_currentRenderState.Fill || reset)
	{
		const uint32_t tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
		m_dxDevice->SetRenderState(D3DRS_FILLMODE, tb[state.Fill]);
	}
	// アルファテスト
	if (state.AlphaTest != m_currentRenderState.AlphaTest || reset)
	{
		const uint32_t tb[] = { FALSE, TRUE };
		m_dxDevice->SetRenderState(D3DRS_ALPHATESTENABLE, tb[state.AlphaTest]);
	}
	// 深度テスト
	if (state.DepthTest != m_currentRenderState.DepthTest || reset)
	{
		const uint32_t tb[] = { FALSE, TRUE };
		m_dxDevice->SetRenderState(D3DRS_ZENABLE, tb[state.DepthTest]);
	}
	// 深度書き込み
	if (state.DepthWrite != m_currentRenderState.DepthWrite || reset)
	{
		const uint32_t tb[] = { FALSE, TRUE };
		m_dxDevice->SetRenderState(D3DRS_ZWRITEENABLE, tb[state.DepthWrite]);
	}
#if 0	// ポイントスプライトは API 毎の依存が強いのでここでは設定しない。実装に任せる。
	// ポイントスプライト
	if (state.PointSprite != m_currentRenderState.PointSprite || reset)
	{
		const uint32_t tb[] = { FALSE, TRUE };
		m_dxDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, tb[state.PointSprite]);
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::InternalSetRenderTarget(int index, ITexture* texture, bool reset)
{
	if (m_currentRenderTargets[index] != texture || reset)
	{
		if (texture) {
			LN_THROW((texture->GetTextureType() == TextureType_RenderTarget), ArgumentException);
			LN_COMCALL(m_dxDevice->SetRenderTarget(index, ((DX9TextureBase*)texture)->GetIDirect3DSurface9()));
		}
		else {
			LN_COMCALL(m_dxDevice->SetRenderTarget(index, NULL));
		}
		LN_REFOBJ_SET(m_currentRenderTargets[index], static_cast<DX9RenderTargetTexture*>(texture));

		// index 0 の場合はビューポートを再設定
		if (index == 0)
		{
			SetViewport(Rect(Point(0, 0), m_currentRenderTargets[0]->GetSize()));
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::InternalSetDepthBuffer(ITexture* texture, bool reset)
{

	if (m_currentDepthBuffer != texture || reset)
	{
		if (texture) {
			LN_THROW((texture->GetTextureType() == TextureType_DepthBuffer), ArgumentException);
			m_dxDevice->SetDepthStencilSurface(((DX9TextureBase*)texture)->GetIDirect3DSurface9());
		}
		else {
			m_dxDevice->SetDepthStencilSurface(NULL);
		}
		LN_REFOBJ_SET(m_currentDepthBuffer, static_cast<DX9DepthBuffer*>(texture));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::InternalSetViewport(const Rect& rect, bool reset)
{
	D3DVIEWPORT9 viewport;
	viewport.X = static_cast<DWORD>(rect.X);
	viewport.Y = static_cast<DWORD>(rect.Y);
	viewport.Width = static_cast<DWORD>(rect.Width);
	viewport.Height = static_cast<DWORD>(rect.Height);
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	LN_COMCALL(m_dxDevice->SetViewport(&viewport));
	m_currentViewportRect = rect;



	//RECT rc = { 0, 0, rect.Width, rect.Height };
	//m_dxDevice->SetScissorRect(&rc);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::InternalSetVertexBuffer(IVertexBuffer* vertexBuffer, bool reset)
{
	if (m_currentVertexBuffer != vertexBuffer || reset)
	{
		DX9VertexBuffer* vb = static_cast<DX9VertexBuffer*>(vertexBuffer);
		if (vb != NULL) {
			LN_COMCALL(m_dxDevice->SetVertexDeclaration(vb->GetDxVertexDeclaration()));
			LN_COMCALL(m_dxDevice->SetStreamSource(0, vb->GetDxVertexBuffer(), 0, vb->GetVertexStride()));
		}
		else {
			LN_COMCALL(m_dxDevice->SetVertexDeclaration(NULL));
			LN_COMCALL(m_dxDevice->SetStreamSource(0, NULL, 0, 0));
		}
		LN_REFOBJ_SET(m_currentVertexBuffer, vb);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Renderer::InternalSetIndexBuffer(IIndexBuffer* indexBuffer, bool reset)
{
	if (m_currentIndexBuffer != indexBuffer || reset)
	{
		DX9IndexBuffer* ib = static_cast<DX9IndexBuffer*>(indexBuffer);
		LN_COMCALL(m_dxDevice->SetIndices((ib) ? ib->GetDxIndexBuffer() : NULL));
		LN_REFOBJ_SET(m_currentIndexBuffer, ib);
	}
}


} // namespace Device
} // namespace Graphics
} // namespace Lumino
