
#pragma once 
#include <Lumino/IO/FileManager.h>
#include <Lumino/Platform/PlatformWindow.h>
#include "../GraphicsDeviceBase.h"
#include "DX9Renderer.h"

LN_NAMESPACE_BEGIN
namespace Driver
{
class DX9SwapChain;

class DX9GraphicsDevice
	: public GraphicsDeviceBase
{
public:
	struct ConfigData
	{
		PlatformWindow*			MainWindow;
		FileManager*			FileManager;
		IDirect3DDevice9*		D3D9Device;
		SizeI					BackbufferSize;
		bool					EnableVSyncWait;
		bool					EnableFPUPreserve;

		ConfigData()
			: FileManager(NULL)
			, D3D9Device(NULL)
			, BackbufferSize(640, 480)
			, EnableVSyncWait(false)
			, EnableFPUPreserve(false)
		{}
	};

public:
	DX9GraphicsDevice();
	virtual ~DX9GraphicsDevice();

public:
	/// 描画を行うスレッドで初期化する必要がある
	void initialize(const ConfigData& configData);

	/// FileManager
	FileManager* getFileManager() { return m_fileManager; }

	/// IDirect3DDevice9 取得
	IDirect3DDevice9* getIDirect3DDevice9() { return m_dxDevice; }

	/// ID3DXEffectPool 取得
	ID3DXEffectPool* getD3DXEffectPool() { return m_d3dxEffectPool; }

	/// デバイスロスト状態を通知する
	void setDeviceLostFlag() { m_deviceState = DeviceState_Lost; }

	const D3DPRESENT_PARAMETERS& getPresentParameters() const { return m_presentParameters; }

	const List<IDirect3DTexture9*>& getDummyTextures() const { return m_dummyTextures; }

public:
	virtual void dispose() override;
	virtual bool isStandalone() const override { return m_direct3D != NULL; }
	virtual GraphicsAPI getGraphicsAPI() const override { return GraphicsAPI::DirectX9; }
	virtual ISwapChain* getDefaultSwapChain() override;
	virtual IRenderer* getRenderer() override { return m_renderer; }
	virtual Ref<IVertexDeclaration> createVertexDeclarationImplement(const VertexElement* elements, int elementsCount) override;
	virtual Ref<IVertexBuffer> createVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage) override;
	virtual Ref<IIndexBuffer> createIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage) override;
	virtual Ref<ITexture> createTextureImplement(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData) override;
	virtual Ref<ITexture> createTexturePlatformLoadingImplement(Stream* stream, bool mipmap, TextureFormat format) override;
	virtual Ref<ITexture> createTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData) override;
	virtual Ref<ITexture> ceateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual Ref<ITexture> createDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual Ref<IShader> createShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual Ref<ISwapChain> createSwapChainImplement(PlatformWindow* window) override;
	virtual DeviceState getDeviceState()  override { return m_deviceState; }
	virtual void resetDevice() override;
	virtual void onLostDevice() override;
	virtual void onResetDevice() override;
	virtual void flushResource() override {}

private:
	void checkDeviceInformation();
	void resetDevice(bool fullscreen);
	void setPresentParameters(const SizeI& backbufferSize, bool fullscreen);

private:
	FileManager*				m_fileManager;
	Ref<PlatformWindow>		m_mainWindow;
	DX9Renderer*				m_renderer;
	DX9SwapChain*				m_defaultSwapChain;
	volatile DeviceState		m_deviceState;
	
	IDirect3D9*		            m_direct3D;
	IDirect3DDevice9*	        m_dxDevice;
	D3DPRESENT_PARAMETERS       m_presentParameters;
	ID3DXEffectPool*			m_d3dxEffectPool;

	bool                        m_enableFPUPreserve;

	// 調査結果
	D3DCAPS9                    m_dxCaps;
	D3DDISPLAYMODE              m_dxDisplayMode;
	D3DTEXTUREFILTERTYPE        m_currentMinFilter;
	D3DTEXTUREFILTERTYPE        m_currentMagFilter;
	D3DTEXTUREFILTERTYPE        m_currentMipFilter;
	D3DTEXTUREFILTERTYPE        m_bestMinFilter;
	D3DTEXTUREFILTERTYPE        m_bestMagFilter;
	D3DTEXTUREFILTERTYPE        m_bestMipFilter;
	DWORD                       m_MSAAQualityWindowed;
	DWORD                       m_MSAAQualityFullScreen;
	D3DDEVTYPE				    m_deviceType;        ///< デバイスの種類。_checkDeviceInformation() でチェックして initialize() に伝えるために用意

	List<IDirect3DTexture9*>	m_dummyTextures;
};

} // namespace Driver
LN_NAMESPACE_END
