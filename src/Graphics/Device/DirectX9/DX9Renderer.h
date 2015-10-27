
#pragma once 

#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/RenderState.h>
#include "../GraphicsDriverInterface.h"
#include "DX9Texture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9GraphicsDevice;
class DX9VertexBuffer;
class DX9IndexBuffer;
class DX9ShaderPass;

/// DirectX9 用の IRenderer の実装
class DX9Renderer
	: public IRenderer
{
public:
	DX9Renderer(DX9GraphicsDevice* device);
	virtual ~DX9Renderer();

public:
	DX9ShaderPass* GetCurrentShaderPass() { return m_currentShaderPass; }
	void SetCurrentShaderPass(DX9ShaderPass* pass);
	void OnLostDevice();
	void OnResetDevice();
	//void TryBeginScene();
	//void TryEndScene();

public:
	virtual void EnterRenderState();
	virtual void LeaveRenderState();
	virtual void Begin();
	virtual void End();
	virtual void SetRenderState(const RenderState& state);
	//virtual const RenderState& GetRenderState();
	virtual void SetDepthStencilState(const DepthStencilState& state);
	//virtual const DepthStencilState& GetDepthStencilState();
	virtual void SetRenderTarget(int index, ITexture* texture);
	//virtual ITexture* GetRenderTarget(int index);
	virtual void SetDepthBuffer(ITexture* texture);
	//virtual ITexture* GetDepthBuffer();
	virtual void SetViewport(const Rect& rect);
	//virtual const Rect& GetViewport();
	virtual void SetVertexBuffer(IVertexBuffer* vertexBuffer);
	virtual void SetIndexBuffer(IIndexBuffer* indexBuffer);
	virtual void Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil);
	virtual void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);
	virtual void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

private:
	void RestoreStatus();
	void InternalSetRenderState(const RenderState& state, bool reset);
	void InternalSetDepthStencilState(const DepthStencilState& newState, bool reset);
	void InternalSetRenderTarget(int index, ITexture* texture, bool reset);
	void InternalSetDepthBuffer(ITexture* texture, bool reset);
	void InternalSetViewport(const Rect& rect, bool reset);
	void InternalSetVertexBuffer(IVertexBuffer* vertexBuffer, bool reset);
	void InternalSetIndexBuffer(IIndexBuffer* indexBuffer, bool reset);

private:
	DX9GraphicsDevice*		m_owner;
	IDirect3DDevice9*		m_dxDevice;
	RenderState				m_currentRenderState;
	DepthStencilState		m_currentDepthStencilState;
	Rect					m_currentViewportRect;
	DX9VertexBuffer*		m_currentVertexBuffer;
	DX9IndexBuffer*			m_currentIndexBuffer;
	DX9RenderTargetTexture*	m_currentRenderTargets[MaxMultiRenderTargets];
	DX9DepthBuffer*			m_currentDepthBuffer;
	DX9ShaderPass*			m_currentShaderPass;

	DWORD	m_state_FVF;
	DWORD	m_state_D3DRS_ALPHABLENDENABLE;
	DWORD	m_state_D3DRS_BLENDOP;
	DWORD	m_state_D3DRS_DESTBLEND;
	DWORD	m_state_D3DRS_SRCBLEND;
	DWORD	m_state_D3DRS_ALPHAREF;
	DWORD	m_state_D3DRS_ZENABLE;
	DWORD	m_state_D3DRS_ZWRITEENABLE;
	DWORD	m_state_D3DRS_ALPHATESTENABLE;
	DWORD	m_state_D3DRS_CULLMODE;
	DWORD	m_state_D3DRS_COLORVERTEX;
	DWORD	m_state_D3DRS_LIGHTING;
	DWORD	m_state_D3DRS_SHADEMODE;
	IDirect3DVertexShader9*			m_state_vertexShader;
	IDirect3DPixelShader9*			m_state_pixelShader;
	IDirect3DVertexDeclaration9*	m_state_vertexDeclaration;
	IDirect3DVertexBuffer9* m_state_streamData;
	UINT m_state_OffsetInBytes;
	UINT m_state_pStride;

	IDirect3DIndexBuffer9*	m_state_IndexData;

	IDirect3DBaseTexture9*	m_state_pTexture;

	bool					m_sceneBegan;
	bool					m_restorationStates;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
