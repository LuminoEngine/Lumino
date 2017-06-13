
#pragma once

#include "../Internal.h"
#include <Lumino/Profiler.h>
#include "Device/GraphicsDriverInterface.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//------------------------------------------------------------------------------
RenderingThread::RenderingThread()
	: m_exception(NULL)
{
	m_running.setFalse();
	m_endRequested.setFalse();
}

//------------------------------------------------------------------------------
RenderingThread::~RenderingThread()
{
	LN_SAFE_DELETE(m_exception);
}

//------------------------------------------------------------------------------
void RenderingThread::reset(Driver::IGraphicsDevice* device)
{
	m_device = device;
	m_renderer = m_device->getRenderer();
}

//------------------------------------------------------------------------------
void RenderingThread::Dispose()
{
	// 終了要求を出して待つ
	m_endRequested.setTrue();
	m_running.setTrue();
	wait();

	// 残っているコマンドはすべて破棄
	MutexScopedLock lock(m_mutex);
	for (RenderingCommandList* c : m_commandListQueue)
	{
		c->postExecute();		// TODO: デストラクタでやるべきかも？
		c->m_running.setFalse();
		c->m_idling.setTrue();
		c->release();
	}
	m_commandListQueue.clear();
}

//------------------------------------------------------------------------------
void RenderingThread::pushRenderingCommand(RenderingCommandList* commandList)
{
	MutexScopedLock lock(m_mutex);
	m_commandListQueue.push_back(commandList);
	commandList->m_running.setTrue();
	commandList->m_idling.setFalse();
	commandList->addRef();
	m_running.setTrue();

	// 既に描画スレッド例外していれば、それを再 throw する。
	// （↑受け取ったコマンドリストは Release を実行するため、とりあえずキューに入れておく）
	if (m_exception != NULL) {
		throw *m_exception;
	}
}

//------------------------------------------------------------------------------
void RenderingThread::requestPauseAndWait()
{
	// TODO: このスピンロックは何とかしたいが…
	while (m_running.isTrue()) { Thread::sleep(1); }
}

//------------------------------------------------------------------------------
void RenderingThread::requestResume()
{
}

//------------------------------------------------------------------------------
void RenderingThread::execute()
{
	// 描画スレッド初期化
	m_device->AttachRenderingThread();

	// 終了要求が来るまでループし続ける。ただし、実行するべきコマンドは全て実行してから終了する。
	while (true)
	{
		// キューからコマンドリストを1つ取り出してみる
		RenderingCommandList* commandList = nullptr;
		{
			MutexScopedLock lock(m_mutex);
			if (!m_commandListQueue.empty()) {
				commandList = m_commandListQueue.front();
				m_commandListQueue.pop_front();
			}
			else {
				// コマンドリストがなかった。待機状態にする。Mutex でロックされているのでこの直前に true になっていることはない
				m_running.setFalse();
			}
		}

		// コマンドリストのキューをチェックした後、キューが空で、かつ終了要求が来ている場合は終了する
		if (m_running.isFalse() && m_endRequested.isTrue())
		{
			break;
		}

		if (commandList != nullptr)
		{
			// 基本的に描画スレッドでの例外は、復帰不能なエラーと考える。(か、assert 的な、そもそも API の使い方が間違っている)
			// エラーはここで保持し、一度でも例外したら Failed 状態にする。
			//  Failed 状態の間はコマンドを実行しない。
			if (m_exception == nullptr)
			{
				try
				{
					ScopedProfilerSection sec(Profiler::Group_RenderThread, Profiler::Section_RenderThread_CommandExecute);

					// コマンドリスト実行
					commandList->execute(m_device);
				}
				catch (Exception& e)
				{
					m_exception = e.copy();
				}
			}
			commandList->postExecute();
			//commandList->m_running.setFalse();
			commandList->m_idling.setTrue();
			commandList->release();
		}
		else
		{
			// 処理するコマンドがない。true になるまで待機する
			m_running.wait();
		}
	}

	// 描画スレッドデタッチ (GL は MakeContext(NULL) が必要)
	// https://sites.google.com/site/monshonosuana/opengl/opengl_005
	m_device->DetachRenderingThread();
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
