
#pragma once

#include <vector>
#include <Lumino/Base/Queue.h>
#include <Lumino/Threading/Thread.h>
#include "RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class RenderingThread
	: public Thread
{
public:
	RenderingThread();
	virtual ~RenderingThread();

public:
	void Reset(Driver::IGraphicsDevice* device);
	void Dispose();

	// 指定したコマンドリストの実行を直ちに開始する
	void PushRenderingCommand(RenderingCommandList* commandList);
	Exception* GetException() { return m_exception; }

	bool IsRunning() { return m_running.IsTrue(); }

	/// 描画スレッドへ一時停止を要求し、停止するまで待つ。
	/// 描画スレッドは、現在実行中のコマンドリストの実行が終わってから一時停止する。
	void RequestPauseAndWait();

	/// 描画スレッドを再開する
	void RequestResume();

protected:
	/// スレッド関数
	virtual void Execute();

private:
	Driver::IGraphicsDevice*		m_device;
	Driver::IRenderer*				m_renderer;
	Queue<RenderingCommandList*>	m_commandListQueue;
	Mutex							m_mutex;
	ConditionFlag					m_running;
	ConditionFlag					m_endRequested;
	Exception*						m_exception;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
