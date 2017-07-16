
#include "../../Internal.h"
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

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// DX9GraphicsDevice
//==============================================================================

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
DX9GraphicsDevice::~DX9GraphicsDevice()
{
	LN_COM_SAFE_RELEASE(m_d3dxEffectPool);
	LN_COM_SAFE_RELEASE(m_dxDevice);
	LN_COM_SAFE_RELEASE(m_direct3D);
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::initialize(const ConfigData& configData)
{
	m_mainWindow.attach(configData.MainWindow, true);
	m_enableFPUPreserve = configData.EnableFPUPreserve;

	// DLL 読み込み
	DX9Module::initialize(); 

	// Direct3D 作成
	m_direct3D = DX9Module::Direct3DCreate9(D3D_SDK_VERSION);
	LN_THROW(m_direct3D != NULL, InvalidOperationException);

	// デバイスの性能チェック
	checkDeviceInformation();

	if (configData.D3D9Device == NULL)
	{
		// デフォルトの SwapChain
		m_defaultSwapChain = LN_NEW DX9SwapChain();
		m_defaultSwapChain->initializeDefault(this, m_mainWindow, configData.BackbufferSize);

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
		m_presentParameters.hDeviceWindow = PlatformSupport::getWindowHandle(m_mainWindow);
		//m_presentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		// デバイス作成
		resetDevice(false/*configData.FullScreen*//*, mSystemManager->getMainWindow()->getSize()*/);
	}
	else
	{
		m_dxDevice = configData.D3D9Device;
		m_dxDevice->AddRef();

		LN_LOG_INFO << "user IDirect3DDevice9 specified.";
	}

	// ID3DXEffectPool
	LN_COMCALL(DX9Module::D3DXCreateEffectPool(&m_d3dxEffectPool));

	// 描画クラスの作成
	m_renderer = LN_NEW DX9Renderer(this);

	if (m_defaultSwapChain != NULL) {
		m_defaultSwapChain->postInitialize();
	}


	for (int i = 0; i < m_dxCaps.MaxSimultaneousTextures; ++i)
	{
		IDirect3DTexture9* tex = NULL;
		LN_COMCALL(m_dxDevice->CreateTexture(2, 2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL));
		m_dummyTextures.add(tex);
	}

	
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::dispose()
{
	for (IDirect3DTexture9* tex : m_dummyTextures)
	{
		LN_COM_SAFE_RELEASE(tex);
	}
	m_dummyTextures.clear();

	GraphicsDeviceBase::dispose();
	LN_SAFE_RELEASE(m_renderer);
	LN_SAFE_RELEASE(m_defaultSwapChain);
}

//------------------------------------------------------------------------------
ISwapChain* DX9GraphicsDevice::getDefaultSwapChain()
{
	return m_defaultSwapChain;
}

//------------------------------------------------------------------------------
RefPtr<IVertexDeclaration> DX9GraphicsDevice::createVertexDeclarationImplement(const VertexElement* elements, int elementsCount)
{
	RefPtr<DX9VertexDeclaration> obj(LN_NEW DX9VertexDeclaration(), false);
	obj->initialize(this, elements, elementsCount);
	return RefPtr<IVertexDeclaration>::staticCast(obj);
}

//------------------------------------------------------------------------------
RefPtr<IVertexBuffer> DX9GraphicsDevice::createVertexBufferImplement(size_t bufferSize, const void* data, ResourceUsage usage)
{
	RefPtr<DX9VertexBuffer> obj(LN_NEW DX9VertexBuffer(), false);
	obj->create(this, bufferSize, data, usage);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<IIndexBuffer> DX9GraphicsDevice::createIndexBufferImplement(int indexCount, const void* initialData, IndexBufferFormat format, ResourceUsage usage)
{
	RefPtr<DX9IndexBuffer> obj(LN_NEW DX9IndexBuffer(), false);
	obj->create(this, indexCount, initialData, format, usage);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ITexture> DX9GraphicsDevice::createTextureImplement(const SizeI& size, bool mipmap, TextureFormat format, const void* initialData)
{
	RefPtr<DX9Texture> obj(LN_NEW DX9Texture(this, size, format, mipmap), false);
	if (initialData != nullptr) {
		obj->setSubData(PointI(0, 0), initialData, Utils::getTextureFormatByteCount(format) * size.width * size.height, size);
	}
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ITexture> DX9GraphicsDevice::createTexturePlatformLoadingImplement(Stream* stream, bool mipmap, TextureFormat format)
{
	ByteBuffer buffer;
	buffer.resize((size_t)stream->getLength(), false);
	stream->read(buffer.getData(), buffer.getSize());

	RefPtr<DX9Texture> obj(LN_NEW DX9Texture(this, buffer.getData(), buffer.getSize(), Color32::Transparency, mipmap, format), false);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ITexture> DX9GraphicsDevice::createTexture3DImplement(int width, int height, int depth, uint32_t mipLevels, TextureFormat format, ResourceUsage usage, const void* initialData)
{
	RefPtr<DX9Texture3D> obj(LN_NEW DX9Texture3D(this), false);
	obj->initialize(width, height, depth, format, mipLevels);
	if (initialData != nullptr) {
		obj->setSubData3D(Box32::Zero, initialData, Utils::getTextureFormatByteCount(format) * width * height * depth);
	}
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ITexture> DX9GraphicsDevice::ceateRenderTargetImplement(uint32_t width, uint32_t height, uint32_t mipLevels, TextureFormat format)
{
	RefPtr<DX9RenderTargetTexture> obj(LN_NEW DX9RenderTargetTexture(this, SizeI(width, height), format, mipLevels), false);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ITexture> DX9GraphicsDevice::createDepthBufferImplement(uint32_t width, uint32_t height, TextureFormat format)
{
	RefPtr<DX9DepthBuffer> obj(LN_NEW DX9DepthBuffer(this, SizeI(width, height), format), false);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<IShader> DX9GraphicsDevice::createShaderImplement(const void* textData, size_t size, ShaderCompileResult* result)
{
	DX9Shader* shader = NULL;
	result->Level = DX9Shader::create(this, (const char*)textData, size, &shader, &result->Message);
	if (shader != NULL) {
		addDeviceResource(shader);
	}
	RefPtr<IShader> obj(shader, false);
	return obj;
}

//------------------------------------------------------------------------------
RefPtr<ISwapChain> DX9GraphicsDevice::createSwapChainImplement(PlatformWindow* window)
{
	RefPtr<DX9SwapChain> obj(LN_NEW DX9SwapChain(), false);
	obj->initializeSub(this, window, window->getSize());
	return obj;
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::resetDevice()
{
	// 先に onLostDevice() を呼ぶこと
	LN_THROW(m_deviceState == DeviceState_Pausing, InvalidOperationException);

	resetDevice(false);
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::onLostDevice()
{
	m_deviceState = DeviceState_Pausing;

	m_renderer->onLostDevice();
	m_defaultSwapChain->onLostDevice();
	for (int i = 0; i < m_deviceObjectList.getCount(); i++) {
		m_deviceObjectList[i]->onLostDevice();
	}

	// TODO: DepthBuffer がどんどん増え続けている
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::onResetDevice()
{
	for (int i = m_deviceObjectList.getCount() - 1; i >= 0; i--) {
		m_deviceObjectList[i]->onResetDevice();
	}
	m_defaultSwapChain->onResetDevice();
	m_renderer->onResetDevice();

	m_deviceState = DeviceState_Enabled;
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::checkDeviceInformation()
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
	Logger::writeLine("グラフィックデバイスの情報を取得します...");
	Logger::writeLine("    スクリーンの幅             : %u", m_dxDisplayMode.Width);
	Logger::writeLine("    スクリーンの高さ           : %u", m_dxDisplayMode.Height);
	Logger::writeLine("    リフレッシュレート         : %u", m_dxDisplayMode.RefreshRate);
	Logger::writeLine("    ディスプレイフォーマット   : %s", DX9Module::GetDxFormatString(m_dxDisplayMode.Format));
	Logger::writeLine("    テクスチャ");
	Logger::writeLine("        最大横幅               : %u", m_dxCaps.MaxTextureWidth);
	Logger::writeLine("        最大縦幅               : %u", m_dxCaps.MaxTextureHeight);
	Logger::writeLine("        2 の累乗制限           : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ? "あり" : "なし");
	Logger::writeLine("        2 の累乗制限の限定解除 : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL ? "あり" : "なし");
	Logger::writeLine("        正方形制限             : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ? "あり" : "なし");
	Logger::writeLine("        動的テクスチャ         : %s", m_dxCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES ? "○" : "×");
	Logger::writeLine("    キューブテクスチャ         : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP ? "○" : "×");
	Logger::writeLine("        2 の累乗制限           : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2 ? "あり" : "なし");
	Logger::writeLine("    ボリュームテクスチャ       : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP ? "○" : "×");
	Logger::writeLine("        2 の累乗制限           : %s", m_dxCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2 ? "あり" : "なし");
	Logger::writeLine("        ﾃﾞｨﾒﾝｼﾞｮﾝ最大値        : %u", m_dxCaps.MaxVolumeExtent);
	Logger::writeLine("    異方性の次数               : %u", m_dxCaps.MaxAnisotropy);
	Logger::writeLine("    異方性フィルタ拡大         : %s", m_dxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? "○" : "×");
	Logger::writeLine("    異方性フィルタ縮小         : %s", m_dxCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? "○" : "×");
	Logger::writeLine("    最大プリミティブ数         : %u", m_dxCaps.MaxPrimitiveCount);
	Logger::writeLine("    最大インデックス数         : %u", m_dxCaps.MaxVertexIndex);
	Logger::writeLine("    頂点シェーダバージョン     : %u.%u", D3DSHADER_VERSION_MAJOR(m_dxCaps.VertexShaderVersion), D3DSHADER_VERSION_MINOR(m_dxCaps.VertexShaderVersion));
	Logger::writeLine("    ピクセルシェーダバージョン : %u.%u", D3DSHADER_VERSION_MAJOR(m_dxCaps.PixelShaderVersion), D3DSHADER_VERSION_MINOR(m_dxCaps.PixelShaderVersion));
	Logger::writeLine("    マルチレンダーターゲット数 : %u", m_dxCaps.NumSimultaneousRTs);
	Logger::writeLine("    MSAA レベル(Window)        : %u", m_MSAAQualityWindowed);
	Logger::writeLine("    MSAA レベル(FullScreen)    : %u", m_MSAAQualityFullScreen);

	// シェーダのバージョンチェック
	LN_THROW((m_dxCaps.VertexShaderVersion >= D3DVS_VERSION(2, 0)), InvalidOperationException, "Invalid vertex shader version.");
	LN_THROW((m_dxCaps.PixelShaderVersion >= D3DPS_VERSION(2, 0)), InvalidOperationException, "Invalid vertex shader version.");
}

//------------------------------------------------------------------------------
void DX9GraphicsDevice::resetDevice(bool fullscreen)
{
	// プレゼンテーションパラメータ設定
	//mIsFullScreen = (canFullScreen()) ? fullscreen : false;
	//if (mDefaultCanvas->getBackbufferResizeMode() == BackbufferResizeMode_Scaling)
	//	_setPresentParameters(mDefaultCanvas->getRequestedBackbufferSize());
	//else
	//	_setPresentParameters(mSystemManager->getMainWindow()->getSize());
	setPresentParameters(m_defaultSwapChain->getBackBufferSize(), fullscreen);

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

//------------------------------------------------------------------------------
void DX9GraphicsDevice::setPresentParameters(const SizeI& backbufferSize, bool fullscreen)
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
		m_presentParameters.BackBufferWidth = std::max(1, backbufferSize.width);	// 0 だとデバイス作成時にエラーとなる、最低 1
		m_presentParameters.BackBufferHeight = std::max(1, backbufferSize.height);	// 0 だとデバイス作成時にエラーとなるため、最低 1
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
LN_NAMESPACE_END
