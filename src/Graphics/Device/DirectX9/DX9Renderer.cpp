
#include "../../../Internal.h"
#include "DX9GraphicsDevice.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9Shader.h"
#include "DX9Renderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// DX9Renderer
//==============================================================================

//------------------------------------------------------------------------------
DX9Renderer::DX9Renderer(DX9GraphicsDevice* device)
	: m_owner(device)
	, m_dxDevice(NULL)
	, m_currentViewportRect()
	, m_currentVertexBuffer(NULL)
	, m_currentIndexBuffer(NULL)
	, m_currentDepthBuffer(NULL)
	, m_currentShaderPass(NULL)
	, m_sceneBegan(false)
	, m_restorationStates(true)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));

	m_dxDevice = device->GetIDirect3DDevice9();

	D3DVIEWPORT9 vp;
	m_dxDevice->GetViewport(&vp);
	SetViewport(Rect(0, 0, vp.Width, vp.Height));

	OnResetDevice();
}

//------------------------------------------------------------------------------
DX9Renderer::~DX9Renderer()
{
	LN_SAFE_RELEASE(m_currentVertexBuffer);
	LN_SAFE_RELEASE(m_currentIndexBuffer);
	LN_SAFE_RELEASE(m_currentDepthBuffer);
	for (int i = 0; i < MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
}

//------------------------------------------------------------------------------
void DX9Renderer::SetCurrentShaderPass(DX9ShaderPass* pass)
{
	//if (m_currentShaderPass != NULL && m_currentShaderPass != pass) {
	//	m_currentShaderPass->EndPass();
	//}
	LN_REFOBJ_SET(m_currentShaderPass, pass);	// TODO: DX9Shader の参照カウントも増やさないとまずいかも
}

//------------------------------------------------------------------------------
void DX9Renderer::OnLostDevice()
{
	//TryEndScene();


}

//------------------------------------------------------------------------------
void DX9Renderer::OnResetDevice()
{
}

//------------------------------------------------------------------------------
void DX9Renderer::EnterRenderState()
{
	// ステートを保存する
	if (m_restorationStates)
	{
		IDirect3DDevice9* device = m_owner->GetIDirect3DDevice9();
		device->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_state_D3DRS_ALPHABLENDENABLE);
		device->GetRenderState(D3DRS_BLENDOP, &m_state_D3DRS_BLENDOP);
		device->GetRenderState(D3DRS_DESTBLEND, &m_state_D3DRS_DESTBLEND);
		device->GetRenderState(D3DRS_SRCBLEND, &m_state_D3DRS_SRCBLEND);
		device->GetRenderState(D3DRS_ALPHAREF, &m_state_D3DRS_ALPHAREF);
		device->GetRenderState(D3DRS_ALPHAFUNC, &m_state_D3DCMP_GREATEREQUAL);

		device->GetRenderState(D3DRS_ZENABLE, &m_state_D3DRS_ZENABLE);
		device->GetRenderState(D3DRS_ZWRITEENABLE, &m_state_D3DRS_ZWRITEENABLE);
		device->GetRenderState(D3DRS_ALPHATESTENABLE, &m_state_D3DRS_ALPHATESTENABLE);
		device->GetRenderState(D3DRS_CULLMODE, &m_state_D3DRS_CULLMODE);

		device->GetRenderState(D3DRS_COLORVERTEX, &m_state_D3DRS_COLORVERTEX);
		device->GetRenderState(D3DRS_LIGHTING, &m_state_D3DRS_LIGHTING);
		device->GetRenderState(D3DRS_SHADEMODE, &m_state_D3DRS_SHADEMODE);

		device->GetVertexShader(&m_state_vertexShader);
		device->GetPixelShader(&m_state_pixelShader);
		device->GetVertexDeclaration(&m_state_vertexDeclaration);
		//device->GetStreamSource( 0, &m_state_streamData, &m_state_OffsetInBytes, &m_state_pStride );
		//device->GetIndices( &m_state_IndexData );

		device->GetTexture(0, &m_state_pTexture);
		device->GetFVF(&m_state_FVF);
	}

	RestoreStatus();

	OnUpdateRenderState(m_currentRenderState, m_currentRenderState, true);
	OnUpdateDepthStencilState(m_currentDepthStencilState, m_currentDepthStencilState, true);
	for (int i = 0; i < MaxMultiRenderTargets; ++i)
	{
		if (i != 0 || m_currentRenderTargets[i] != NULL) {	// 0 に NULL を指定することはできない。なのでやむを得ず何もしない
			InternalSetRenderTarget(i, m_currentRenderTargets[i], true);
		}
	}
	InternalSetDepthBuffer(m_currentDepthBuffer, true);
	InternalSetViewport(m_currentViewportRect, true);
	InternalSetVertexBuffer(m_currentVertexBuffer, true);
	InternalSetIndexBuffer(m_currentIndexBuffer, true);
}

//------------------------------------------------------------------------------
void DX9Renderer::LeaveRenderState()
{
	// アクティブなパスをここで終了しておく
	if (m_currentShaderPass != NULL) {
		m_currentShaderPass->EndPass();
	}
	LN_SAFE_RELEASE(m_currentShaderPass);





	////InternalSetRenderState(NULL, true);
	////InternalSetDepthStencilState(NULL, true);
	//for (int i = 0; i < MaxMultiRenderTargets; ++i)
	//{
	//	//if (i != 0 || m_currentRenderTargets[i] != NULL) {	// 0 に NULL を指定することはできない。なのでやむを得ず何もしない
	//		//InternalSetRenderTarget(i, NULL, true);
	//	//}
	//}
	//InternalSetDepthBuffer(NULL, true);
	////InternalSetViewport(NULL, true);
	//InternalSetVertexBuffer(NULL, true);
	//InternalSetIndexBuffer(NULL, true);




	// ステートを復元する
	if (m_restorationStates)
	{
		IDirect3DDevice9* device = m_owner->GetIDirect3DDevice9();
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, m_state_D3DRS_ALPHABLENDENABLE);
		device->SetRenderState(D3DRS_BLENDOP, m_state_D3DRS_BLENDOP);
		device->SetRenderState(D3DRS_DESTBLEND, m_state_D3DRS_DESTBLEND);
		device->SetRenderState(D3DRS_SRCBLEND, m_state_D3DRS_SRCBLEND);
		device->SetRenderState(D3DRS_ALPHAREF, m_state_D3DRS_ALPHAREF);
		device->SetRenderState(D3DRS_ALPHAFUNC, m_state_D3DCMP_GREATEREQUAL);

		device->SetRenderState(D3DRS_ZENABLE, m_state_D3DRS_ZENABLE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, m_state_D3DRS_ZWRITEENABLE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, m_state_D3DRS_ALPHATESTENABLE);
		device->SetRenderState(D3DRS_CULLMODE, m_state_D3DRS_CULLMODE);

		device->SetRenderState(D3DRS_COLORVERTEX, m_state_D3DRS_COLORVERTEX);
		device->SetRenderState(D3DRS_LIGHTING, m_state_D3DRS_LIGHTING);
		device->SetRenderState(D3DRS_SHADEMODE, m_state_D3DRS_SHADEMODE);

		device->SetVertexShader(m_state_vertexShader);
		LN_SAFE_RELEASE(m_state_vertexShader);

		device->SetPixelShader(m_state_pixelShader);
		LN_SAFE_RELEASE(m_state_pixelShader);

		device->SetVertexDeclaration(m_state_vertexDeclaration);
		LN_SAFE_RELEASE(m_state_vertexDeclaration);

		//device->SetStreamSource( 0, m_state_streamData, m_state_OffsetInBytes, m_state_pStride );
		//LN_SAFE_RELEASE( m_state_streamData );

		//device->SetIndices( m_state_IndexData );
		//LN_SAFE_RELEASE( m_state_IndexData );

		device->SetTexture(0, m_state_pTexture);
		LN_SAFE_RELEASE(m_state_pTexture);

		device->SetFVF(m_state_FVF);
	}
}

//------------------------------------------------------------------------------
void DX9Renderer::Begin()
{
	if (m_owner->IsStandalone()) {
		m_dxDevice->BeginScene();
	}
	m_sceneBegan = true;
}

//------------------------------------------------------------------------------
void DX9Renderer::End()
{
	if (m_sceneBegan)
	{
		if (m_owner->IsStandalone()) {
			m_dxDevice->EndScene();
		}
		m_sceneBegan = false;
	}
}

//------------------------------------------------------------------------------
void DX9Renderer::SetRenderTarget(int index, ITexture* texture)
{
	//TryBeginScene();
	InternalSetRenderTarget(index, texture, false);
}

//------------------------------------------------------------------------------
ITexture* DX9Renderer::GetRenderTarget(int index)
{
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void DX9Renderer::SetDepthBuffer(ITexture* texture)
{
	//TryBeginScene();
	InternalSetDepthBuffer(texture, false);
}

//------------------------------------------------------------------------------
//ITexture* DX9Renderer::GetDepthBuffer()
//{
//	return m_currentDepthBuffer;
//}

//------------------------------------------------------------------------------
void DX9Renderer::SetViewport(const Rect& rect)
{
	//TryBeginScene();
	InternalSetViewport(rect, false);
}

//------------------------------------------------------------------------------
//const Rect& DX9Renderer::GetViewport()
//{
//	return m_currentViewportRect;
//}

//------------------------------------------------------------------------------
void DX9Renderer::SetVertexBuffer(IVertexBuffer* vertexBuffer)
{
	//TryBeginScene();
	InternalSetVertexBuffer(vertexBuffer, false);
}

//------------------------------------------------------------------------------
void DX9Renderer::SetIndexBuffer(IIndexBuffer* indexBuffer)
{
	//TryBeginScene();
	InternalSetIndexBuffer(indexBuffer, false);
}

//------------------------------------------------------------------------------
void DX9Renderer::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	IRenderer::FlushStates();
	//TryBeginScene();

	// ※レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	//   していない場合、エラーとならないがクリアされない。

	DWORD flag = 0;
	if (flags.TestFlag(ClearFlags::Color)) { flag |= D3DCLEAR_TARGET; }
	if (m_currentDepthBuffer && flags.TestFlag(ClearFlags::Depth))	{ flag |= (D3DCLEAR_ZBUFFER); }
	if (m_currentDepthBuffer && flags.TestFlag(ClearFlags::Stencil)) { flag |= (D3DCLEAR_STENCIL); }
	if (flag == 0) { return; }

	Color c;
	c.r = static_cast<uint8_t>(color.r * 255);
	c.g = static_cast<uint8_t>(color.g * 255);
	c.b = static_cast<uint8_t>(color.b * 255);
	c.a = static_cast<uint8_t>(color.a * 255);
	D3DCOLOR dxc = D3DCOLOR_ARGB(c.a, c.r, c.g, c.b);
	LN_COMCALL(m_dxDevice->Clear(0, NULL, flag, dxc, z, stencil));

	IDirect3DSurface9* ff;
	m_dxDevice->GetDepthStencilSurface(&ff);
	ff->Release();
}

//------------------------------------------------------------------------------
void DX9Renderer::DrawPrimitive(IVertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	IRenderer::FlushStates();

	// TODO
	SetVertexBuffer(vertexBuffer);
	//TryBeginScene();

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

//------------------------------------------------------------------------------
void DX9Renderer::DrawPrimitiveIndexed(IVertexBuffer* vertexBuffer, IIndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	IRenderer::FlushStates();

	// TODO
	SetVertexBuffer(vertexBuffer);
	SetIndexBuffer(indexBuffer);
	//TryBeginScene();

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
		// ※ DrawIndexedPrimitiveUP の中で強制停止する場合、インデックスバッファの配列外参照しているかもしれない。
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

//------------------------------------------------------------------------------
void DX9Renderer::RestoreStatus()
{
	// プログラム実行中、特に変化しないステートはここで設定してしまう

	m_dxDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);			// D3D default
	m_dxDevice->SetRenderState(D3DRS_LIGHTING, TRUE);				// D3D default
	m_dxDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);	// D3D default

	// アルファテスト時の比較関数の指定 ( D3DCMPFUNC 列挙型  デフォルトは D3DCMP_ALWAYS )
	m_dxDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ステンシルテスト
	m_dxDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);				// ステンシルマスク
	m_dxDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);			// ステンシルマスク

	m_dxDevice->SetTexture(0, NULL);
	m_dxDevice->SetFVF(0);

	OnUpdateRenderState(m_currentRenderState, m_currentRenderState, true);
	OnUpdateDepthStencilState(m_currentDepthStencilState, m_currentDepthStencilState, true);
}

//------------------------------------------------------------------------------
void DX9Renderer::OnUpdateRenderState(const RenderState& newState, const RenderState& oldState, bool reset)
{
	// Blending
	DWORD blendOpTable[] =
	{
		D3DBLENDOP_ADD,
		D3DBLENDOP_SUBTRACT,
		D3DBLENDOP_REVSUBTRACT,
		D3DBLENDOP_MIN,
		D3DBLENDOP_MAX,
	};
	DWORD blendFactorTable[] =
	{
		D3DBLEND_ZERO,
		D3DBLEND_ONE,
		D3DBLEND_SRCCOLOR,
		D3DBLEND_INVSRCCOLOR,
		D3DBLEND_SRCALPHA,
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_INVDESTCOLOR,
		D3DBLEND_DESTALPHA,
		D3DBLEND_INVDESTALPHA,
	};
	if (newState.alphaBlendEnabled != oldState.alphaBlendEnabled || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, (newState.alphaBlendEnabled) ? TRUE : FALSE);
	}
	if (newState.blendOp != oldState.blendOp || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_BLENDOP, blendOpTable[(int)newState.blendOp]);
	}
	if (newState.sourceBlend != oldState.sourceBlend || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_SRCBLEND, blendFactorTable[(int)newState.sourceBlend]);
	}
	if (newState.destinationBlend != oldState.destinationBlend || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_DESTBLEND, blendFactorTable[(int)newState.destinationBlend]);
	}

	// カリング
	if (newState.Culling != oldState.Culling || reset)
	{
		const uint32_t tb[] = { D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW };
		m_dxDevice->SetRenderState(D3DRS_CULLMODE, tb[(int)newState.Culling]);
	}
	// 塗りつぶし方法
	if (newState.Fill != oldState.Fill || reset)
	{
		const uint32_t tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
		m_dxDevice->SetRenderState(D3DRS_FILLMODE, tb[newState.Fill]);
	}
	// アルファテスト
	if (newState.AlphaTest != oldState.AlphaTest || reset)
	{
		const uint32_t tb[] = { FALSE, TRUE };
		m_dxDevice->SetRenderState(D3DRS_ALPHATESTENABLE, tb[newState.AlphaTest]);
	}
}

//------------------------------------------------------------------------------
void DX9Renderer::OnUpdateDepthStencilState(const DepthStencilState& newState, const DepthStencilState& oldState, bool reset)
{
	DWORD cmpFuncTable[] =
	{
		D3DCMP_NEVER,			// Never
		D3DCMP_LESS,			// Less
		D3DCMP_LESSEQUAL,		// LessEqual
		D3DCMP_GREATER,			// Greater
		D3DCMP_GREATEREQUAL,	// GreaterEqual
		D3DCMP_EQUAL,			// Equal
		D3DCMP_NOTEQUAL,		// NotEqual
		D3DCMP_ALWAYS,			// Always
	};

	// 深度テスト
	if (newState.DepthTestEnabled != oldState.DepthTestEnabled || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_ZENABLE, (newState.DepthTestEnabled) ? TRUE : FALSE);
	}
	// 深度書き込み
	if (newState.DepthWriteEnabled != oldState.DepthWriteEnabled || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_ZWRITEENABLE, (newState.DepthWriteEnabled) ? TRUE : FALSE);
	}
	// 深度比較関数
	if (newState.DepthTestFunc != oldState.DepthTestFunc || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_ZFUNC, cmpFuncTable[newState.DepthTestFunc]);
	}

	// ステンシルテスト有無
	if (newState.StencilEnabled != oldState.StencilEnabled || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_STENCILENABLE, (newState.StencilEnabled) ? TRUE : FALSE);
	}

	// ステンシルテスト比較関数
	if (newState.StencilFunc != oldState.StencilFunc || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_STENCILFUNC, cmpFuncTable[newState.StencilFunc]);
	}

	// ステンシルテスト参照値
	if (newState.StencilReferenceValue != oldState.StencilReferenceValue || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_STENCILREF, newState.StencilReferenceValue);
	}

	// ステンシルテスト処理
	DWORD stencilOpTable[] = { D3DSTENCILOP_KEEP, D3DSTENCILOP_REPLACE };
	if (newState.StencilFailOp != oldState.StencilFailOp || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_STENCILFAIL, stencilOpTable[newState.StencilFailOp]);
	}
	if (newState.StencilDepthFailOp != oldState.StencilDepthFailOp || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_STENCILZFAIL, stencilOpTable[newState.StencilDepthFailOp]);
	}
	if (newState.StencilPassOp != oldState.StencilPassOp || reset)
	{
		m_dxDevice->SetRenderState(D3DRS_STENCILPASS, stencilOpTable[newState.StencilPassOp]);
	}
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void DX9Renderer::InternalSetViewport(const Rect& rect, bool reset)
{
	D3DVIEWPORT9 viewport;
	viewport.X = static_cast<DWORD>(rect.x);
	viewport.Y = static_cast<DWORD>(rect.y);
	viewport.Width = static_cast<DWORD>(rect.width);
	viewport.Height = static_cast<DWORD>(rect.height);
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	LN_COMCALL(m_dxDevice->SetViewport(&viewport));
	m_currentViewportRect = rect;



	//RECT rc = { 0, 0, rect.Width, rect.Height };
	//m_dxDevice->SetScissorRect(&rc);
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void DX9Renderer::InternalSetIndexBuffer(IIndexBuffer* indexBuffer, bool reset)
{
	if (m_currentIndexBuffer != indexBuffer || reset)
	{
		DX9IndexBuffer* ib = static_cast<DX9IndexBuffer*>(indexBuffer);
		LN_COMCALL(m_dxDevice->SetIndices((ib) ? ib->GetDxIndexBuffer() : NULL));
		LN_REFOBJ_SET(m_currentIndexBuffer, ib);
	}
}


} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
