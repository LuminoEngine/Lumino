
#include "../Internal.h"
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Texture.h>
#include "GraphicsManager.h"
#include "RendererImpl.h"
#include "RenderingThread.h"
#include "RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// SwapChain
//==============================================================================

//------------------------------------------------------------------------------
SwapChain::SwapChain()
	: m_deviceObj(nullptr)
{
}

//------------------------------------------------------------------------------
SwapChain::~SwapChain()
{
}

//------------------------------------------------------------------------------
void SwapChain::InitializeDefault(detail::GraphicsManager* manager)
{
	GraphicsResourceObject::Initialize();

	m_deviceObj = m_manager->GetGraphicsDevice()->GetDefaultSwapChain();
	m_deviceObj->AddRef();
	PostInitialize();
}

//------------------------------------------------------------------------------
void SwapChain::InitializeSub(detail::GraphicsManager* manager, PlatformWindow* window)
{
	GraphicsResourceObject::Initialize();

	m_deviceObj = m_manager->GetGraphicsDevice()->CreateSwapChain(window);
	PostInitialize();
}

//------------------------------------------------------------------------------
void SwapChain::Dispose()
{
	if (m_deviceObj != nullptr)
	{
		// 前回発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
		m_waiting.Wait();

		m_backColorBuffer->DetachDefaultBackBuffer();

		LN_SAFE_RELEASE(m_commandList);
		LN_SAFE_RELEASE(m_backColorBuffer);
		LN_SAFE_RELEASE(m_deviceObj);
	}

	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
void SwapChain::PostInitialize()
{
	m_commandList = LN_NEW RenderingCommandList(m_manager);

	// TODO: デフォルトのバックバッファという仕組みは入らない気がする。
	// こちら側でレンダリングターゲット作って、Present で全体転送してもらえばいいし。

	//Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();
	//m_deviceObj->GetBackBuffer()->AddRef();	// ↓の set 用に+1しておく (TODO: ↓の中でやるのがいいのかもしれないが・・・。)
	m_backColorBuffer = LN_NEW RenderTargetTexture();//Texture::CreateRenderTarget(m_manager, backbufferSize, 1, TextureFormat_R8G8B8X8);
	m_backColorBuffer->CreateCore(m_manager, true/*m_deviceObj->GetBackBuffer(), NULL*/);
	m_backColorBuffer->AttachDefaultBackBuffer(m_deviceObj->GetBackBuffer());

	// 独自管理できる深度バッファを作る。
	// これがないと、OpenGL のバックバッファはレンダリングターゲットと深度バッファを分離することが出来ないため、本Lib的に不都合が起こる。
	//m_backDepthBuffer = LN_NEW DepthBuffer();
	//m_backDepthBuffer->CreateImpl(m_manager, m_deviceObj->GetBackBuffer()->GetSize(), TextureFormat::D24S8);

	m_waiting.SetTrue();
}

//------------------------------------------------------------------------------
RenderTargetTexture* SwapChain::GetBackBuffer()
{
	if (m_backColorBuffer->GetDeviceObjectConst() != m_deviceObj->GetBackBuffer())
	{
		m_backColorBuffer->DetachDefaultBackBuffer();
		m_backColorBuffer->AttachDefaultBackBuffer(m_deviceObj->GetBackBuffer());
	}
	return m_backColorBuffer;
}

//------------------------------------------------------------------------------
void SwapChain::Present()
{
	m_manager->PresentSwapChain(this);
}

//------------------------------------------------------------------------------
void SwapChain::MightResizeAndDeviceReset(const SizeI& newSize)
{
	// Resize
	if (GetBackBuffer()->GetSize() != newSize)
	{
		m_deviceObj->Resize(newSize);
		m_backColorBuffer->AttachDefaultBackBuffer(m_deviceObj->GetBackBuffer());
		// ※ここではまだ深度バッファはリサイズしない。Present を終えた後に行う。
	}



	/*
		デバイスロストの確認は、描画コマンドを作り始める前に行わなければならない。
		作り終わった後では、例えばバックバッファをリサイズしてリセットしたとき、
		その時点で作られている描画コマンドは古いサイズのバックバッファへの描画を前提としていることになる。
	*/

	auto* device = m_manager->GetGraphicsDevice();

	if (m_manager->GetRenderingType() == GraphicsRenderingType::Immediate)
	{
		// デバイスロストのチェック
		if (device->GetDeviceState() == Driver::DeviceState_Lost)
		{
			device->GetRenderer()->LeaveRenderState();
			m_manager->PauseDevice();
			device->ResetDevice();
			m_manager->ResumeDevice();
			device->GetRenderer()->EnterRenderState();

			// 深度バッファのサイズを新しいバックバッファサイズに合わせる
			//m_backDepthBuffer->Resize(m_deviceObj->GetBackBuffer()->GetSize());
		}
	}
	else
	{
		RenderingThread* thread = m_manager->GetRenderingThread();

		// デバイスロストのチェック
		if (device->GetDeviceState() == Driver::DeviceState_Lost)
		{
			// 溜まっているコマンドを全て実行してレンダリングレッドを一時停止する
			thread->RequestPauseAndWait();

			// リセット
			try
			{
				m_manager->PauseDevice();
				device->ResetDevice();
				m_manager->ResumeDevice();
			}
			catch (...) {
				thread->RequestResume();
				throw;
			}
			thread->RequestResume();

			// 深度バッファのサイズを新しいバックバッファサイズに合わせる
			//m_backDepthBuffer->Resize(m_deviceObj->GetBackBuffer()->GetSize());

		}
	}
}

//------------------------------------------------------------------------------
void SwapChain::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		m_deviceObj = m_manager->GetGraphicsDevice()->GetDefaultSwapChain();
		m_deviceObj->AddRef();
	}
}

//------------------------------------------------------------------------------
void SwapChain::PresentInternal()
{
	m_deviceObj->Present(m_backColorBuffer->ResolveDeviceObject());

	// 実行完了。m_waiting を ture にすることで、メインスレッドからはこのスワップチェインをキューに追加できるようになる。
	// コマンドの成否にかかわらず true にしないと、例外した後にデッドロックが発生する。

	// TODO: ポインタが fefefefe とかなってたことがあった。メモリバリア張っておくこと。
	m_waiting.SetTrue();
}

//------------------------------------------------------------------------------
void SwapChain::WaitForPresent()
{
	m_waiting.Wait();
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
