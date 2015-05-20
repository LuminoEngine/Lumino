
#pragma once

#include <vector>
#include <Lumino/Base/Queue.h>
#include <Lumino/Threading/Thread.h>
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{

class RenderingThread
	: public Threading::Thread
{
public:
	RenderingThread();
	virtual ~RenderingThread();

public:
	void Initialize(Device::IGraphicsDevice* device);
	void Dispose();
	void PushRenderingCommand(RenderingCommandList* commandList);
	Exception* GetException() { return m_exception; }

	/// 描画スレッドへ一時停止を要求し、停止するまで待つ。
	/// 描画スレッドは、現在実行中のコマンドリストの実行が終わってから一時停止する。
	void RequestPauseAndWait();

	/// 描画スレッドを再開する
	void RequestResume();

protected:
	/// スレッド関数
	virtual void Execute();

private:
	Device::IGraphicsDevice*		m_device;
	Device::IRenderer*				m_renderer;
	Queue<RenderingCommandList*>	m_commandListQueue;
	Threading::Mutex				m_mutex;
	Threading::EventFlag			m_running;
	Threading::EventFlag			m_endRequested;
	Exception*						m_exception;
};

} // namespace Graphics
} // namespace Lumino
