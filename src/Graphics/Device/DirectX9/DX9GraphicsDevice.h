
#pragma once 
#include <Lumino/IO/FileManager.h>
#include <Lumino/Platform/PlatformWindow.h>
#include "../GraphicsDeviceBase.h"
#include "DX9Renderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
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
		Size					BackbufferSize;
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
	void Initialize(const ConfigData& configData);

	/// FileManager
	FileManager* GetFileManager() { return m_fileManager; }

	/// IDirect3DDevice9 取得
	IDirect3DDevice9* GetIDirect3DDevice9() { return m_dxDevice; }

	/// ID3DXEffectPool 取得
	ID3DXEffectPool* GetD3DXEffectPool() { return m_d3dxEffectPool; }

	/// デバイスロスト状態を通知する
	void SetDeviceLostFlag() { m_deviceState = DeviceState_Lost; }

	const D3DPRESENT_PARAMETERS& GetPresentParameters() const { return m_presentParameters; }

public:
	virtual void Finalize() override;
	virtual bool IsStandalone() const override { return m_direct3D != NULL; }
	virtual GraphicsAPI GetGraphicsAPI() const override { return GraphicsAPI::DirectX9; }
	virtual ISwapChain* GetDefaultSwapChain() override;
	virtual IRenderer* GetRenderer() override { return m_renderer; }
	virtual RefPtr<IVertexDeclaration> CreateVertexDeclarationImplement(const VertexElement* elements, int elementsCount) override;
	virtual RefPtr<IVertexBuffer> CreateVertexBufferImplement(size_t bufferSize, const void* data, DeviceResourceUsage usage) override;
	virtual RefPtr<IIndexBuffer> CreateIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage) override;
	virtual RefPtr<ITexture> CreateTextureImplement(const Size& size, uint32_t mipLevels, TextureFormat format, const void* initialData) override;
	virtual RefPtr<ITexture> CreateTexturePlatformLoadingImplement(Stream* stream, uint32_t mipLevels, TextureFormat format) override;
	virtual RefPtr<ITexture> CreateTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, const void* initialData);
	virtual RefPtr<ITexture> CreateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format) override;
	virtual RefPtr<ITexture> CreateDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format) override;
	virtual RefPtr<IShader> CreateShaderImplement(const void* textData, size_t size, ShaderCompileResult* result) override;
	virtual RefPtr<ISwapChain> CreateSwapChainImplement(PlatformWindow* window) override;
	virtual DeviceState GetDeviceState()  override { return m_deviceState; }
	virtual void ResetDevice() override;
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;
	virtual void LockContext() override {}
	virtual void UnlockContext() override {}

private:
	void CheckDeviceInformation();
	void ResetDevice(bool fullscreen);
	void SetPresentParameters(const Size& backbufferSize, bool fullscreen);

private:
	FileManager*				m_fileManager;
	RefPtr<PlatformWindow>		m_mainWindow;
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

};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
