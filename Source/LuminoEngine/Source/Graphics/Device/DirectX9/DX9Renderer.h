
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

	DX9ShaderPass* getCurrentShaderPass() { return m_currentShaderPass; }
	void setCurrentShaderPass(DX9ShaderPass* pass);
	void onLostDevice();
	void onResetDevice();

private:
	void restoreStatus();
	virtual void onEnterRenderState() override;
	virtual void onLeaveRenderState() override;
	virtual void onBeginRendering() override;
	virtual void onEndRendering() override;
	virtual void onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, ITexture* depthBuffer) override;
	virtual	void onUpdateRenderState(const RenderState& newState, const RenderState& oldState, bool reset) override;
	virtual	void onUpdateDepthStencilState(const DepthStencilState& newState, const DepthStencilState& oldState, bool reset) override;
	virtual void onUpdatePrimitiveData(IVertexDeclaration* decls, const List<Ref<IVertexBuffer>>& vertexBuufers, IIndexBuffer* indexBuffer) override;
	virtual void onClear(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
	virtual void onDrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) override;
	virtual void onDrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) override;

private:
	DX9GraphicsDevice*		m_owner;
	IDirect3DDevice9*		m_dxDevice;
	//RectI					m_currentViewportRect;
	DX9ShaderPass*			m_currentShaderPass;

	// onEnterRenderState() 時点で IDirect3DDevice9 から取り出すフレームバッファ。
	// setRenderTarget() で nullptr が指定された場合、これを使用する。
	// これはライブラリ外部で用意されたフレームバッファに書き込むための機能。
	IDirect3DSurface9*		m_defaultRenderTargets[Graphics::MaxMultiRenderTargets];
	IDirect3DSurface9*		m_defaultDepthBuffer;

	DWORD	m_state_FVF;
	DWORD	m_state_D3DRS_ALPHABLENDENABLE;
	DWORD	m_state_D3DRS_BLENDOP;
	DWORD	m_state_D3DRS_DESTBLEND;
	DWORD	m_state_D3DRS_SRCBLEND;
	DWORD	m_state_D3DRS_ALPHAREF;
	DWORD	m_state_D3DCMP_GREATEREQUAL;
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

	bool					m_restorationStates;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
