
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
void SwapChain::initializeDefault(detail::GraphicsManager* manager)
{
	GraphicsResourceObject::initialize();

	m_deviceObj = m_manager->getGraphicsDevice()->getDefaultSwapChain();
	m_deviceObj->addRef();
	postInitialize();
}

//------------------------------------------------------------------------------
void SwapChain::initializeSub(detail::GraphicsManager* manager, PlatformWindow* window)
{
	GraphicsResourceObject::initialize();

	m_deviceObj = m_manager->getGraphicsDevice()->createSwapChain(window);
	postInitialize();
}

//------------------------------------------------------------------------------
void SwapChain::Dispose()
{
	if (m_deviceObj != nullptr)
	{
		// 前回発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
		m_waiting.wait();

		m_backColorBuffer->detachDefaultBackBuffer();

		LN_SAFE_RELEASE(m_commandList);
		LN_SAFE_RELEASE(m_backColorBuffer);
		LN_SAFE_RELEASE(m_deviceObj);
	}

	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
void SwapChain::postInitialize()
{
	m_commandList = LN_NEW RenderingCommandList(m_manager);

	// TODO: デフォルトのバックバッファという仕組みは入らない気がする。
	// こちら側でレンダリングターゲット作って、present で全体転送してもらえばいいし。

	//Driver::IGraphicsDevice* device = m_manager->getGraphicsDevice();
	//m_deviceObj->getBackBuffer()->AddRef();	// ↓の set 用に+1しておく (TODO: ↓の中でやるのがいいのかもしれないが・・・。)
	m_backColorBuffer = LN_NEW RenderTargetTexture();//Texture::createRenderTarget(m_manager, backbufferSize, 1, TextureFormat_R8G8B8X8);
	m_backColorBuffer->createCore(m_manager, true/*m_deviceObj->getBackBuffer(), NULL*/);
	m_backColorBuffer->attachDefaultBackBuffer(m_deviceObj->getBackBuffer());

	// 独自管理できる深度バッファを作る。
	// これがないと、OpenGL のバックバッファはレンダリングターゲットと深度バッファを分離することが出来ないため、本Lib的に不都合が起こる。
	//m_backDepthBuffer = LN_NEW DepthBuffer();
	//m_backDepthBuffer->createImpl(m_manager, m_deviceObj->getBackBuffer()->GetSize(), TextureFormat::D24S8);

	m_waiting.setTrue();
}

//------------------------------------------------------------------------------
RenderTargetTexture* SwapChain::getBackBuffer()
{
	if (m_backColorBuffer->getDeviceObjectConst() != m_deviceObj->getBackBuffer())
	{
		m_backColorBuffer->detachDefaultBackBuffer();
		m_backColorBuffer->attachDefaultBackBuffer(m_deviceObj->getBackBuffer());
	}
	return m_backColorBuffer;
}

//------------------------------------------------------------------------------
void SwapChain::present()
{
	m_manager->presentSwapChain(this);
}

//------------------------------------------------------------------------------
void SwapChain::MightResizeAndDeviceReset(const SizeI& newSize)
{
	// Resize
	if (getBackBuffer()->getSize() != newSize)
	{
		m_deviceObj->resize(newSize);
		m_backColorBuffer->attachDefaultBackBuffer(m_deviceObj->getBackBuffer());
		// ※ここではまだ深度バッファはリサイズしない。present を終えた後に行う。
	}



	/*
		デバイスロストの確認は、描画コマンドを作り始める前に行わなければならない。
		作り終わった後では、例えばバックバッファをリサイズしてリセットしたとき、
		その時点で作られている描画コマンドは古いサイズのバックバッファへの描画を前提としていることになる。
	*/

	auto* device = m_manager->getGraphicsDevice();

	if (m_manager->getRenderingType() == GraphicsRenderingType::Immediate)
	{
		// デバイスロストのチェック
		if (device->getDeviceState() == Driver::DeviceState_Lost)
		{
			device->getRenderer()->leaveRenderState();
			m_manager->pauseDevice();
			device->resetDevice();
			m_manager->resumeDevice();
			device->getRenderer()->enterRenderState();

			// 深度バッファのサイズを新しいバックバッファサイズに合わせる
			//m_backDepthBuffer->Resize(m_deviceObj->getBackBuffer()->GetSize());
		}
	}
	else
	{
		RenderingThread* thread = m_manager->getRenderingThread();

		// デバイスロストのチェック
		if (device->getDeviceState() == Driver::DeviceState_Lost)
		{
			// 溜まっているコマンドを全て実行してレンダリングレッドを一時停止する
			thread->requestPauseAndWait();

			// リセット
			try
			{
				m_manager->pauseDevice();
				device->resetDevice();
				m_manager->resumeDevice();
			}
			catch (...) {
				thread->requestResume();
				throw;
			}
			thread->requestResume();

			// 深度バッファのサイズを新しいバックバッファサイズに合わせる
			//m_backDepthBuffer->Resize(m_deviceObj->getBackBuffer()->GetSize());

		}
	}
}

//------------------------------------------------------------------------------
void SwapChain::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		m_deviceObj = m_manager->getGraphicsDevice()->getDefaultSwapChain();
		m_deviceObj->addRef();
	}
}

//------------------------------------------------------------------------------
void SwapChain::PresentInternal()
{
	m_deviceObj->present(m_backColorBuffer->resolveDeviceObject());

	// 実行完了。m_waiting を ture にすることで、メインスレッドからはこのスワップチェインをキューに追加できるようになる。
	// コマンドの成否にかかわらず true にしないと、例外した後にデッドロックが発生する。

	// TODO: ポインタが fefefefe とかなってたことがあった。メモリバリア張っておくこと。
	m_waiting.setTrue();
}

//------------------------------------------------------------------------------
void SwapChain::WaitForPresent()
{
	m_waiting.wait();
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
