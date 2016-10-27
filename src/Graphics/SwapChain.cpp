
#pragma once

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
	, m_isDefault(true)
{
}

//------------------------------------------------------------------------------
SwapChain::~SwapChain()
{
	// 前回発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
	m_waiting.Wait();

	m_backColorBuffer->DetachDefaultBackBuffer();

	LN_SAFE_RELEASE(m_commandList);
	LN_SAFE_RELEASE(m_backColorBuffer);
	LN_SAFE_RELEASE(m_backDepthBuffer);
	LN_SAFE_RELEASE(m_deviceObj);;
}

//------------------------------------------------------------------------------
void SwapChain::InitializeDefault(detail::GraphicsManager* manager)
{
	GraphicsResourceObject::Initialize(manager);

	m_deviceObj = m_manager->GetGraphicsDevice()->GetDefaultSwapChain();
	m_deviceObj->AddRef();
	PostInitialize();
}

//------------------------------------------------------------------------------
void SwapChain::InitializeSub(detail::GraphicsManager* manager, PlatformWindow* window)
{
	GraphicsResourceObject::Initialize(manager);

	m_deviceObj = m_manager->GetGraphicsDevice()->CreateSwapChain(window);
	PostInitialize();
}

//------------------------------------------------------------------------------
void SwapChain::PostInitialize()
{
	m_commandList = LN_NEW RenderingCommandList();

	// TODO: デフォルトのバックバッファという仕組みは入らない気がする。
	// こちら側でレンダリングターゲット作って、Present で全体転送してもらえばいいし。

	//Driver::IGraphicsDevice* device = m_manager->GetGraphicsDevice();
	//m_deviceObj->GetBackBuffer()->AddRef();	// ↓の set 用に+1しておく (TODO: ↓の中でやるのがいいのかもしれないが・・・。)
	m_backColorBuffer = LN_NEW RenderTarget();//Texture::CreateRenderTarget(m_manager, backbufferSize, 1, TextureFormat_R8G8B8X8);
	m_backColorBuffer->CreateCore(m_manager, true/*m_deviceObj->GetBackBuffer(), NULL*/);
	m_backColorBuffer->AttachDefaultBackBuffer(m_deviceObj->GetBackBuffer());

	// 独自管理できる深度バッファを作る。
	// これがないと、OpenGL のバックバッファはレンダリングターゲットと深度バッファを分離することが出来ないため、本Lib的に不都合が起こる。
	m_backDepthBuffer = LN_NEW DepthBuffer();
	m_backDepthBuffer->CreateImpl(m_manager, m_deviceObj->GetBackBuffer()->GetSize(), TextureFormat::D24S8);

	m_waiting.SetTrue();
}

//------------------------------------------------------------------------------
void SwapChain::Resize(const SizeI& newSize)
{
	m_deviceObj->Resize(newSize);
	// ※ここではまだ深度バッファはリサイズしない。Present を終えた後に行う。
}

//------------------------------------------------------------------------------
void SwapChain::Present()
{
	if (m_manager->GetRenderingType() == GraphicsRenderingType::Immediate)
	{
		m_deviceObj->Present(m_backColorBuffer->m_deviceObj);

		// 一時メモリの解放とかをやっておく
		m_manager->GetPrimaryRenderingCommandList()->PostExecute();


		// デバイスロストのチェック
		auto* device = m_manager->GetGraphicsDevice();
		if (device->GetDeviceState() == Driver::DeviceState_Lost)
		{
			device->GetRenderer()->LeaveRenderState();
			m_manager->PauseDevice();
			device->ResetDevice();
			m_manager->ResumeDevice();
			device->GetRenderer()->EnterRenderState();

			// 深度バッファのサイズを新しいバックバッファサイズに合わせる
			m_backDepthBuffer->Resize(m_deviceObj->GetBackBuffer()->GetSize());
		}
	}
	else
	{
		// 前回発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
		m_waiting.Wait();

		// デバイスロストのチェック
		bool reset = false;
		auto* device = m_manager->GetGraphicsDevice();
		if (device->GetDeviceState() == Driver::DeviceState_Lost)
		{
			auto* thread = m_manager->GetRenderingThread();

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
			m_backDepthBuffer->Resize(m_deviceObj->GetBackBuffer()->GetSize());

			reset = true;
		}



		if (reset)
		{
			// デバイスリセットされた直後はバックバッファのリソースが再構築されている。
			// リサイズが原因でリセットされた場合、おそらくほとんどの場合でコマンドに入っている深度バッファとサイズが異なる。
			// このままレンダリングすることはできないので、今回はスキップする。
			m_commandList->ClearCommands();
			m_manager->GetPrimaryRenderingCommandList()->ClearCommands();
			printf("--------\n");
		}
		else
		{
			// 実行状態にする。Present コマンドが実行された後、コマンドリストクラスから True がセットされる。
			// ※ PresentCommandList() の前に false にしておかないとダメ。
			//    後で false にすると、PresentCommandList() と同時に全部のコマンドが実行されて、描画スレッドから
			//    true がセットされるのに、その後 false をセットしてしまうことがある。
			m_waiting.SetFalse();

			// Primary コマンドリストの末尾に Present を追加し、キューへ追加する
			m_manager->GetRenderer()->PresentCommandList(this);
		}
	}
}

//------------------------------------------------------------------------------
void SwapChain::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		if (m_isDefault)
		{
			m_backColorBuffer->DetachDefaultBackBuffer();
		}
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		if (m_isDefault)
		{
			m_deviceObj = m_manager->GetGraphicsDevice()->GetDefaultSwapChain();
			m_deviceObj->AddRef();
			m_backColorBuffer->AttachDefaultBackBuffer(m_deviceObj->GetBackBuffer());
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
