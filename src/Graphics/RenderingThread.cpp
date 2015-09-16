
#pragma once

#include "../Internal.h"
#include <Lumino/Profiler.h>
#include "Device/DeviceInterface.h"
#include "RenderingThread.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingThread::RenderingThread()
	: m_exception(NULL)
{
	m_running.SetFalse();
	m_endRequested.SetFalse();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingThread::~RenderingThread()
{
	LN_SAFE_DELETE(m_exception);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingThread::Initialize(Device::IGraphicsDevice* device)
{
	m_device = device;
	m_renderer = m_device->GetRenderer();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingThread::Dispose()
{
	// 終了要求を出して待つ
	m_endRequested.SetTrue();
	m_running.SetTrue();
	Wait();

	// 残っているコマンドはすべて破棄
	Threading::MutexScopedLock lock(m_mutex);
	LN_FOREACH(RenderingCommandList* c, m_commandListQueue)
	{
		c->PostExecute();		// TODO: デストラクタでやるべきかも？
		c->m_running.SetFalse();
		c->m_idling.SetTrue();
		c->Release();
	}
	m_commandListQueue.Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingThread::PushRenderingCommand(RenderingCommandList* commandList)
{
	Threading::MutexScopedLock lock(m_mutex);
	m_commandListQueue.Enqueue(commandList);
	commandList->m_running.SetTrue();
	commandList->m_idling.SetFalse();
	commandList->AddRef();
	m_running.SetTrue();

	// 既に描画スレッド例外していれば、それを再 throw する。
	// （↑受け取ったコマンドリストは Release を実行するため、とりあえずキューに入れておく）
	if (m_exception != NULL) {
		throw *m_exception;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingThread::RequestPauseAndWait()
{
	// TODO: このスピンロックは何とかしたいが…
	while (m_running.IsTrue()) { Threading::Thread::Sleep(1); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingThread::RequestResume()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingThread::Execute()
{
	// 描画スレッド初期化
	m_device->AttachRenderingThread();

	while (!m_endRequested.IsTrue())
	{
		// キューからコマンドリストを1つ取り出してみる
		RenderingCommandList* commandList = NULL;
		{
			Threading::MutexScopedLock lock(m_mutex);
			if (!m_commandListQueue.IsEmpty()) {
				commandList = m_commandListQueue.GetHead();
				m_commandListQueue.Dequeue();
			}
			else {
				// コマンドリストがなかった。待機状態にする。Mutex でロックされているのでこの直前に true になっていることはない
				m_running.SetFalse();
			}
		}

		if (commandList != NULL)
		{
			// 基本的に描画スレッドでの例外は、復帰不能なエラーと考える。(か、assert 的な、そもそも API の使い方が間違っている)
			// エラーはここで保持し、一度でも例外したら Failed 状態にする。
			//  Failed 状態の間はコマンドを実行しない。
			if (m_exception == NULL)
			{
				try
				{
					ScopedProfilerSection sec(Profiler::Group_RenderThread, Profiler::Section_RenderThread_CommandExecute);
					// コマンドリスト実行
					commandList->Execute(m_renderer);
				}
				catch (Exception& e)
				{
					m_exception = e.Copy();
				}
			}
			commandList->PostExecute();
			//commandList->m_running.SetFalse();
			commandList->m_idling.SetTrue();
			commandList->Release();
		}
		else
		{
			// 処理するコマンドがない。true になるまで待機する
			m_running.Wait();
		}
	}

	// 描画スレッドデタッチ (GL は MakeContext(NULL) が必要)
	// https://sites.google.com/site/monshonosuana/opengl/opengl_005
	m_device->DetachRenderingThread();

#if 0
	// 描画スレッド初期化
	m_device->AttachRenderingThread();

	try
	{
		while (!m_endRequested.IsTrue())
		{
			// キューからコマンドリストを1つ取り出してみる
			RenderingCommandList* commandList = NULL;
			{
				Threading::MutexScopedLock lock(m_mutex);
				if (!m_commandListQueue.IsEmpty()) {
					commandList = m_commandListQueue.GetHead();
					m_commandListQueue.Dequeue();
				}
				else {
					// コマンドリストがなかった。待機状態にする。Mutex でロックされているのでこの直前に true になっていることはない
					m_running.SetFalse();
				}
			}

			if (commandList != NULL)
			{
				try
				{
					// コマンドリスト実行
					commandList->Execute(m_renderer);
					commandList->m_running.SetFalse();
					commandList->m_idling.SetTrue();
					commandList->Release();
				}
				catch (...)
				{
					commandList->m_running.SetFalse();
					commandList->m_idling.SetTrue();
					commandList->Release();
					throw;
				}
			}
			else
			{
				// 処理するコマンドがない。true になるまで待機する
				m_running.Wait();
			}
		}
	}
	catch (Exception& /*e*/)
	{
		m_device->DetachRenderingThread();
		throw;
	}

	// 描画スレッドデタッチ (GL は MakeContext(NULL) が必要)
	// https://sites.google.com/site/monshonosuana/opengl/opengl_005
	m_device->DetachRenderingThread();
#endif
}

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
