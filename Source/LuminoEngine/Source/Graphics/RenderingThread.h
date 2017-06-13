
#pragma once
#include <deque>
#include <vector>
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
	void reset(Driver::IGraphicsDevice* device);
	void Dispose();

	// 指定したコマンドリストの実行を直ちに開始する
	void pushRenderingCommand(RenderingCommandList* commandList);
	Exception* getException() { return m_exception; }

	bool isRunning() { return m_running.isTrue(); }

	/// 描画スレッドへ一時停止を要求し、停止するまで待つ。
	/// 描画スレッドは、現在実行中のコマンドリストの実行が終わってから一時停止する。
	void requestPauseAndWait();

	/// 描画スレッドを再開する
	void requestResume();

protected:
	/// スレッド関数
	virtual void execute();

private:
	Driver::IGraphicsDevice*			m_device;
	Driver::IRenderer*					m_renderer;
	std::deque<RenderingCommandList*>	m_commandListQueue;
	Mutex								m_mutex;
	ConditionFlag						m_running;
	ConditionFlag						m_endRequested;
	Exception*							m_exception;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
