
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/SwapChain.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"
#include "../../include/Lumino/Graphics/Renderer.h"
#include "../../include/Lumino/Graphics/Texture.h"
#include "RenderingThread.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// SwapChain
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SwapChain::SwapChain(GraphicsManager* manager, const Size& mainWindowSize)
	: m_manager(manager)
	, m_deviceObj(manager->GetGraphicsDevice()->GetDeviceObject()->GetDefaultSwapChain())
{
	m_deviceObj->AddRef();
	Initialize(mainWindowSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SwapChain::SwapChain(Platform::Window* targetWindow)
	: m_manager(Internal::Manager)
{
	Initialize(targetWindow->GetSize());
	m_deviceObj = m_manager->GetGraphicsDevice()->GetDeviceObject()->CreateSwapChain(targetWindow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SwapChain::SwapChain(GraphicsManager* manager, Platform::Window* targetWindow)
	: m_manager(manager)
{
	Initialize(targetWindow->GetSize());
	m_deviceObj = m_manager->GetGraphicsDevice()->GetDeviceObject()->CreateSwapChain(targetWindow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SwapChain::~SwapChain()
{
	// 前回発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
	m_waiting.Wait();

	LN_SAFE_RELEASE(m_commandList);
	LN_SAFE_RELEASE(m_backColorBuffer);
	LN_SAFE_RELEASE(m_backDepthBuffer);
	LN_SAFE_RELEASE(m_deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SwapChain::Initialize(const Size& backbufferSize)
{
	m_commandList = LN_NEW RenderingCommandList();

	Device::IGraphicsDevice* device = m_manager->GetGraphicsDevice()->GetDeviceObject();
	m_deviceObj->GetBackBuffer()->AddRef();	// ↓の set 用に+1しておく (TODO: ↓の中でやるのがいいのかもしれないが・・・。)
	m_backColorBuffer = LN_NEW Texture(m_manager, m_deviceObj->GetBackBuffer(), NULL);//Texture::CreateRenderTarget(m_manager, backbufferSize, 1, TextureFormat_R8G8B8X8);
	m_backDepthBuffer = Texture::CreateDepthBuffer(m_manager, backbufferSize, TextureFormat_D24S8);

	m_waiting.SetTrue();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SwapChain::Resize(const Size& newSize)
{
	m_deviceObj->Resize(newSize);

	// m_backColorBuffer は特殊なので Device 層でリサイズされるが、深度バッファはこちらで確保しなおす必要がある。
	LN_SAFE_RELEASE(m_backDepthBuffer);
	m_backDepthBuffer = Texture::CreateDepthBuffer(m_manager, newSize, TextureFormat_D24S8);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SwapChain::Present()
{
	// 前回発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
	m_waiting.Wait();


	// デバイスロストのチェック
	Device::IGraphicsDevice* device = m_manager->GetGraphicsDevice()->GetDeviceObject();
	if (device->GetDeviceState() == Device::DeviceState_Lost)
	{
		// 溜まっているコマンドを全て実行してレンダリングレッドを一時停止する
		m_manager->m_renderingThread->RequestPauseAndWait();
		try
		{
			m_manager->PauseDevice();
			device->ResetDevice();
			m_manager->ResumeDevice();
		}
		catch (...) {
			m_manager->m_renderingThread->RequestResume();
			throw;
		}
		m_manager->m_renderingThread->RequestResume();
	}
	



	// 実行状態にする。Present コマンドが実行された後、コマンドリストクラスから True がセットされる。
	// ※ PresentCommandList() の前に false にしておかないとダメ。
	//    後で false にすると、PresentCommandList() と同時に全部のコマンドが実行されて、描画スレッドから
	//    true がセットされるのに、その後 false をセットしてしまうことがある。
	m_waiting.SetFalse();

	// Primary コマンドリストの末尾に Present を追加し、キューへ追加する
	m_manager->m_renderer->PresentCommandList(this);
}

} // namespace Graphics
} // namespace Lumino
