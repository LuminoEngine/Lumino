
#include "../../../Internal.h"
#include <algorithm>
#include <Lumino/Platform/PlatformSupport.h>
#include <Lumino/Graphics/Utils.h>
#include "DX9Module.h"
#include "DX9VertexBuffer.h"
#include "DX9IndexBuffer.h"
#include "DX9Texture.h"
#include "DX9Shader.h"
#include "DX9SwapChain.h"
#include "DX9GraphicsDevice.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//=============================================================================
// DX9GraphicsDevice
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9GraphicsDevice::DX9GraphicsDevice()
	: m_fileManager(NULL)
	, m_mainWindow()
	, m_renderer(NULL)
	, m_defaultSwapChain(NULL)
	, m_deviceState(DeviceState_Lost)
	, m_direct3D(NULL)
	, m_dxDevice(NULL)
	, m_d3dxEffectPool(NULL)
	, m_enableFPUPreserve(false)
	, m_currentMinFilter(D3DTEXF_POINT)
	, m_currentMagFilter(D3DTEXF_POINT)
	, m_currentMipFilter(D3DTEXF_NONE)
	, m_bestMinFilter(D3DTEXF_POINT)
	, m_bestMagFilter(D3DTEXF_POINT)
	, m_bestMipFilter(D3DTEXF_NONE)
	, m_MSAAQualityWindowed(0)
	, m_MSAAQualityFullScreen(0)
	, m_deviceType(D3DDEVTYPE_HAL)
{
	memset(&m_presentParameters, 0, sizeof(m_presentParameters));
	memset(&m_dxCaps, 0, sizeof(m_dxCaps));
	memset(&m_dxDisplayMode, 0, sizeof(m_dxDisplayMode));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9GraphicsDevice::~DX9GraphicsDevice()
{
	LN_SAFE_RELEASE(m_d3dxEffectPool);
	LN_SAFE_RELEASE(m_dxDevice);
	LN_SAFE_RELEASE(m_direct3D);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::Initialize(const ConfigData& configData)
{
	m_mainWindow.Attach(configData.MainWindow, true);
	m_enableFPUPreserve = configData.EnableFPUPreserve;

	// DLL 読み込み
	DX9Module::Initialize(); 

	if (configData.D3D9Device == NULL)
	{
		// Direct3D 作成
		m_direct3D = DX9Module::Direct3DCreate9(D3D_SDK_VERSION);
		LN_THROW(m_direct3D != NULL, InvalidOperationException);

		// デバイスの性能チェック
		CheckDeviceInformation();

		// デフォルトの SwapChain
		m_defaultSwapChain = LN_NEW DX9SwapChain(this, m_mainWindow, configData.BackbufferSize);

		// 基本的に変化のないプレゼンテーションパラメータの設定
		// ( PresentationInterval は D3DPRESENT_INTERVAL_IMMEDIATE 以外の場合、
		//   DirectShow を使って動画再生した時に画面が表示されなくなることがある )
		ZeroMemory(&m_presentParameters, sizeof(D3DPRESENT_PARAMETERS));
		m_presentParameters.BackBufferCount = 1;
		m_presentParameters.EnableAutoDepthStencil = FALSE;//TRUE;//					// Canvas で作るのでこれはいらない
		m_presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;//D3DFMT_UNKNOWN;//
		m_presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;	// マルチサンプリングするにはコレ
		m_presentParameters.PresentationInterval = (configData.EnableVSyncWait) ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_IMMEDIATE(アダプタの更新間隔無視); // 	//D3DPRESENT_INTERVAL_DEFAULT(アダプタの更新間隔に合わせる);	// 画面の更新間隔
		m_presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_presentParameters.hDeviceWindow = Platform::PlatformSupport::GetWindowHandle(m_mainWindow);
		//m_presentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		// デバイス作成
		ResetDevice(false/*configData.FullScreen*//*, mSystemManager->getMainWindow()->getSize()*/);
	}
	else
	{
		m_dxDevice = configData.D3D9Device;
		m_dxDevice->AddRef();
	}

	// ID3DXEffectPool
	LN_COMCALL(DX9Module::D3DXCreateEffectPool(&m_d3dxEffectPool));

	// 描画クラスの作成
	m_renderer = LN_NEW DX9Renderer(this);

	if (m_defaultSwapChain != NULL) {
		m_defaultSwapChain->PostInitialize();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::Finalize()
{
	GraphicsDeviceBase::Finalize();
	LN_SAFE_RELEASE(m_renderer);
	LN_SAFE_RELEASE(m_defaultSwapChain);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* DX9GraphicsDevice::GetDefaultSwapChain()
{
	return m_defaultSwapChain;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IVertexBuffer* DX9GraphicsDevice::CreateVertexBuffer(const VertexElement* vertexElements, int elementsCount, int vertexCount, const void* data, DeviceResourceUsage usage)
{
	RefPtr<DX9VertexBuffer> obj(LN_NEW DX9VertexBuffer(), false);
	obj->Create(this, vertexElements, elementsCount, vertexCount, data, usage);
	AddDeviceResource(obj);		// GC 対象
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IIndexBuffer* DX9GraphicsDevice::CreateIndexBuffer(int indexCount, const void* initialData, IndexBufferFormat format, DeviceResourceUsage usage)
{
	RefPtr<DX9IndexBuffer> obj(LN_NEW DX9IndexBuffer(), false);
	obj->Create(this, indexCount, initialData, format, usage);
	AddDeviceResource(obj);		// GC 対象
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* DX9GraphicsDevice::CreateTexture(const Size& size, uint32_t mipLevels, TextureFormat format, const void* initialData)
{
	RefPtr<DX9Texture> obj(LN_NEW DX9Texture(this, size, format, mipLevels), false);
	if (initialData != NULL) {
		obj->SetSubData(Point(0, 0), initialData, Utils::GetTextureFormatByteCount(format) * size.Width * size.Height, size);
	}
	AddDeviceResource(obj);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* DX9GraphicsDevice::CreateTexturePlatformLoading(Stream* stream, uint32_t mipLevels, TextureFormat format)
{
	ByteBuffer buffer;
	buffer.Resize((size_t)stream->GetLength(), false);
	stream->Read(buffer.GetData(), buffer.GetSize());

	RefPtr<DX9Texture> obj(LN_NEW DX9Texture(this, buffer.GetData(), buffer.GetSize(), Color::Transparency, mipLevels, format), false);
	AddDeviceResource(obj);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* DX9GraphicsDevice::CreateRenderTarget(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format)
{
	RefPtr<DX9RenderTargetTexture> obj(LN_NEW DX9RenderTargetTexture(this, Size(width, height), format, mipLevels), false);
	AddDeviceResource(obj);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ITexture* DX9GraphicsDevice::CreateDepthBuffer(uint32_t width, uint32_t height, TextureFormat format)
{
	RefPtr<DX9DepthBuffer> obj(LN_NEW DX9DepthBuffer(this, Size(width, height), format), false);
	AddDeviceResource(obj);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IShader* DX9GraphicsDevice::CreateShader(const void* textData, size_t size, ShaderCompileResult* result)
{
	DX9Shader* shader = NULL;
	result->Level = DX9Shader::Create(this, (const char*)textData, size, &shader, &result->Message);
	if (shader != NULL) {
		AddDeviceResource(shader);
	}
	return shader;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ISwapChain* DX9GraphicsDevice::CreateSwapChain(Platform::Window* window)
{
	LN_THROW(0, NotImplementedException);
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::ResetDevice()
{
	// 先に OnLostDevice() を呼ぶこと
	LN_THROW(m_deviceState == DeviceState_Pausing, InvalidOperationException);

	ResetDevice(false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::OnLostDevice()
{
	m_deviceState = DeviceState_Pausing;

	m_renderer->OnLostDevice();
	m_defaultSwapChain->OnLostDevice();
	for (int i = 0; i < m_deviceObjectList.GetCount(); i++) {
		m_deviceObjectList[i]->OnLostDevice();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::OnResetDevice()
{
	for (int i = m_deviceObjectList.GetCount() - 1; i >= 0; i--) {
		m_deviceObjectList[i]->OnResetDevice();
	}
	m_defaultSwapChain->OnResetDevice();
	m_renderer->OnResetDevice();

	m_deviceState = DeviceState_Enabled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::CheckDeviceInformation()
{
	// 使えるデバイスの種類を調べる
	HRESULT hr = m_direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_dxCaps);
	if (SUCCEEDED(hr)) {
		m_deviceType = D3DDEVTYPE_HAL;
	}
	else {
		hr = m_direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &m_dxCaps);
		if (SUCCEEDED(hr)) {
			m_deviceType = D3DDEVTYPE_REF;
		}
		else {
			hr = m_direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_SW, &m_dxCaps);
			if (SUCCEEDED(hr)) {
				m_deviceType = D3DDEVTYPE_SW;
			}
			else {
				LN_THROW(hr, COMException, hr);
			}
		}
	}

	// ディスプレイの情報を取得
	LN_COMCALL(m_direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_dxDisplayMode));

	// アンチエイリアスする場合の品質
	// ウィンドウモードのときは？
	hr = m_direct3D->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT, m_deviceType,
		m_dxDisplayMode.Format, TRUE,
		D3DMULTISAMPLE_NONMASKABLE,
		&m_MSAAQualityWindowed);
	if (FAILED(hr))
	{
		m_MSAAQualityWindowed = 0;
	}
	// フルスクリーンのときは？
	hr = m_direct3D->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT, m_deviceType,
		m_dxDisplayMode.Format, FALSE,
		D3DMULTISAMPLE_NONMASKABLE,
		&m_MSAAQualityFullScreen);
	if (FAILED(hr))
	{
		m_MSAAQualityFullScreen = 0;
	}

	// 一番いいっぽいテクスチャフィルタ取得 ( あとでもっと追加しておく )
	DWORD tfcaps = m_dxCaps.TextureFilterCaps;
	// 縮小
	if (tfcaps & D3DPTFILTERCAPS_MIPFLINEAR)
	{
		m_bestMipFilter = D3DTEXF_LINEAR;
	}
	else if (tfcaps & D3DPTFILTERCAPS_MIPFPOINT)
	{
		m_bestMipFilter = D3DTEXF_POINT;
	}
	// 拡大
	if (tfcaps & D3DPTFILTERCAPS_MAGFANISOTROPIC)
	{
		m_bestMagFilter = D3DTEXF_ANISOTROPIC;
	}
	else if (tfcaps & D3DPTFILTERCAPS_MAGFLINEAR)
	{
		m_bestMagFilter = D3DTEXF_LINEAR;
	}
	// ミップマップ
	if (tfcaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
	{
		m_bestMinFilter = D3DTEXF_ANISOTROPIC;
	}
	else if (tfcaps & D3DPTFILTERCAPS_MINFLINEAR)
	{
		m_bestMinFilter = D3DTEXF_LINEAR;
	}

	// TODO:
	Logger::WriteLine("グラフィックデバイスの情報を取得します...");
	Logger::WriteLine("    スクリーンの幅             : %u", m_dxDisplayMode.Width);
	Logger::WriteLine("    スクリーンの高さ           : %u", m_dxDisplayMode.Height);
	Logger::WriteLine("    リフレッシュレート         : %u", m_dxDisplayMode.RefreshRate);
	Logger::WriteLine("    ディスプレイフォーマット   : %s", DX9Module::GetDxFormatString(m_dxDisplayMode.Format));
	Logger::WriteLine("    テクスチャ");
	Logger::WriteLine("        最大横幅               : %u", m_dxCaps.MaxTextureWidth);
	Logger::WriteLine("        最大縦幅               : %u", m_dxCaps.MaxTextureHeight);
	Logger::WriteLine("        2 の累乗制限           : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ? "あり" : "なし");
	Logger::WriteLine("        2 の累乗制限の限定解除 : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ? "あり" : "なし");
	Logger::WriteLine("        正方形制限             : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ? "あり" : "なし");
	Logger::WriteLine("        動的テクスチャ         : %s", m_dxCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES ? "○" : "×");
	Logger::WriteLine("    キューブテクスチャ         : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP ? "○" : "×");
	Logger::WriteLine("        2 の累乗制限           : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2 ? "あり" : "なし");
	Logger::WriteLine("    ボリュームテクスチャ       : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP ? "○" : "×");
	Logger::WriteLine("        2 の累乗制限           : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2 ? "あり" : "なし");
	Logger::WriteLine("        ﾃﾞｨﾒﾝｼﾞｮﾝ最大値        : %u", m_dxCaps.MaxVolumeExtent);
	Logger::WriteLine("    異方性の次数               : %u", m_dxCaps.MaxAnisotropy);
	Logger::WriteLine("    異方性フィルタ拡大         : %s", m_dxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? "○" : "×");
	Logger::WriteLine("    異方性フィルタ縮小         : %s", m_dxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? "○" : "×");
	Logger::WriteLine("    最大プリミティブ数         : %u", m_dxCaps.MaxPrimitiveCount);
	Logger::WriteLine("    最大インデックス数         : %u", m_dxCaps.MaxVertexIndex);
	Logger::WriteLine("    頂点シェーダバージョン     : %u.%u", D3DSHADER_VERSION_MAJOR(m_dxCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(m_dxCaps.VertexShaderVersion));
	Logger::WriteLine("    ピクセルシェーダバージョン : %u.%u", D3DSHADER_VERSION_MAJOR(m_dxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(m_dxCaps.PixelShaderVersion));
	Logger::WriteLine("    マルチレンダーターゲット数 : %u", m_dxCaps.NumSimultaneousRTs);
	Logger::WriteLine("    MSAA レベル(Window)        : %u", m_MSAAQualityWindowed);
	Logger::WriteLine("    MSAA レベル(FullScreen)    : %u", m_MSAAQualityFullScreen);

	// シェーダのバージョンチェック
	LN_THROW((m_dxCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0)), InvalidOperationException, "Invalid vertex shader version.");
	LN_THROW((m_dxCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0)), InvalidOperationException, "Invalid vertex shader version.");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::ResetDevice(bool fullscreen)
{
	// プレゼンテーションパラメータ設定
	//mIsFullScreen = (canFullScreen()) ? fullscreen : false;
	//if (mDefaultCanvas->getBackbufferResizeMode() == BackbufferResizeMode_Scaling)
	//	_setPresentParameters(mDefaultCanvas->getRequestedBackbufferSize());
	//else
	//	_setPresentParameters(mSystemManager->getMainWindow()->getSize());
	SetPresentParameters(m_defaultSwapChain->GetBackBufferSize(), fullscreen);

	// まだデバイスが作成されていなければ新規作成
	if (!m_dxDevice)
	{
		// D3Dデバイスの生成
		DWORD fpu_precision = (m_enableFPUPreserve) ? D3DCREATE_FPU_PRESERVE : 0;
		LN_COMCALL(
			m_direct3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			m_deviceType,
			m_presentParameters.hDeviceWindow,
			fpu_precision | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//D3DCREATE_SOFTWARE_VERTEXPROCESSING, | D3DCREATE_MULTITHREADED
			&m_presentParameters,
			&m_dxDevice));
	}
	// 既に作成されている場合はリセット
	else
	{
		// デバイスのリセット (作成したスレッド以外でリセットしようとすると失敗する)
		LN_COMCALL(m_dxDevice->Reset(&m_presentParameters));
	}

	m_deviceState = DeviceState_Enabled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9GraphicsDevice::SetPresentParameters(const Size& backbufferSize, bool fullscreen)
{
	// フルスクリーンモードの場合
	if (fullscreen == true)
	{
		m_presentParameters.Windowed = TRUE;
		m_presentParameters.BackBufferWidth = m_dxDisplayMode.Width;
		m_presentParameters.BackBufferHeight = m_dxDisplayMode.Height;
		m_presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;//D3DFMT_X8B8G8R8;//
		if (false)//( mMSAAQualityWindowed > 0 )
		{
			m_presentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
			m_presentParameters.MultiSampleQuality = m_MSAAQualityWindowed - 1;
		}
		else
		{
			m_presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			m_presentParameters.MultiSampleQuality = 0;
		}
		/*
		m_presentParameters.Windowed = FALSE;
		m_presentParameters.BackBufferWidth  = mDxDisplayMode.Width;
		m_presentParameters.BackBufferHeight = mDxDisplayMode.Height;
		m_presentParameters.BackBufferFormat = mDxDisplayMode.Format;
		// 品質レベルが 0 より大きい場合
		if ( false )//( mMSAAQualityFullScreen > 0 )
		{
		m_presentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
		m_presentParameters.MultiSampleQuality = mMSAAQualityFullScreen - 1;
		}
		else
		{
		m_presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_presentParameters.MultiSampleQuality = 0;
		}
		*/
	}
	// ウィンドウモードの場合
	else
	{
		m_presentParameters.Windowed = TRUE;
		m_presentParameters.BackBufferWidth = std::max(1, backbufferSize.Width);	// 0 だとデバイス作成時にエラーとなる、最低 1
		m_presentParameters.BackBufferHeight = std::max(1, backbufferSize.Height);	// 0 だとデバイス作成時にエラーとなるため、最低 1
		m_presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_UNKNOWN;//D3DFMT_X8R8G8B8;//
		if (false)//( mMSAAQualityWindowed > 0 )
		{
			m_presentParameters.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
			m_presentParameters.MultiSampleQuality = m_MSAAQualityWindowed - 1;
		}
		else
		{
			m_presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			m_presentParameters.MultiSampleQuality = 0;
		}
	}
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
