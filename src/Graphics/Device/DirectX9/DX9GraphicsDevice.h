
#pragma once 
#include <Lumino/IO/FileManager.h>
#include "../GraphicsDeviceBase.h"
#include "DX9Renderer.h"

namespace Lumino
{
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
		Platform::Window*		MainWindow;
		Lumino::FileManager*	FileManager;
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


public:
	virtual void Finalize();
	virtual bool IsStandalone() const { return m_direct3D != NULL; }
	virtual GraphicsAPI GetGraphicsAPI() const { return GraphicsAPI::DirectX9; }
	virtual ISwapChain* GetDefaultSwapChain();
	virtual IRenderer* GetRenderer() { return m_renderer; }
	virtual IVertexBuffer* CreateVertexBuffer(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage);
	virtual IIndexBuffer* CreateIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage);
	virtual ITexture* CreateTexture(const Size& size, uint32_t mipLevels, TextureFormat format);
	virtual ITexture* CreateTexturePlatformLoading(Stream* stream, uint32_t mipLevels, TextureFormat format);
	virtual ITexture* CreateRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format);
	virtual ITexture* CreateDepthBuffer(uint32_t width, uint32_t height, TextureFormat format);
	virtual IShader* CreateShader(const void* textData, size_t size, ShaderCompileResult* result);
	virtual ISwapChain* CreateSwapChain(Platform::Window* window);
	virtual DeviceState GetDeviceState() { return m_deviceState; }
	virtual void ResetDevice();
	virtual void OnLostDevice();
	virtual void OnResetDevice();
	virtual void LockContext() {}
	virtual void UnlockContext() {}
	virtual void AttachRenderingThread() {}
	virtual void DetachRenderingThread() {}

private:
	void CheckDeviceInformation();
	void ResetDevice(bool fullscreen);
	void SetPresentParameters(const Size& backbufferSize, bool fullscreen);

private:
	FileManager*				m_fileManager;
	RefPtr<Platform::Window>	m_mainWindow;
	DX9Renderer*				m_renderer;
	DX9SwapChain*				m_defaultSwapChain;
	volatile DeviceState		m_deviceState;

	IDirect3D9*		            m_direct3D;
	IDirect3DDevice9*	        m_dxDevice;
	D3DPRESENT_PARAMETERS       m_presentParameters;	///< プレゼンテーションパラメータ
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
} // namespace Lumino
