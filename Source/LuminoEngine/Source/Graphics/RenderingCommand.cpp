/*
	[2015/7/31] コマンドの実行は仮想関数？関数ポインタ？
		Release ビルド 100000000 回の単純呼び出しで、
		仮想関数 : 450ms
		関数ポインタ : 400ms
		微々たる差だが、関数ポインタが少しだけ有利。
*/

#include "../Internal.h"
#include "Device/GraphicsDriverInterface.h"
#include <Lumino/Graphics/Shader.h>
#include "RenderingCommand.h"
#include "GraphicsManager.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// RenderBulkData
//==============================================================================

//------------------------------------------------------------------------------
RenderingCommand::~RenderingCommand()
{
}

//==============================================================================
// RenderBulkData
//==============================================================================

//------------------------------------------------------------------------------
RenderBulkData::RenderBulkData()
	: m_srcData(nullptr)
	, m_size(0)
	, m_commandList(nullptr)
	, m_dataHandle(0)
{
}

//------------------------------------------------------------------------------
RenderBulkData::RenderBulkData(const void* srcData, size_t size)
	: m_srcData(srcData)
	, m_size(size)
	, m_commandList(nullptr)
	, m_dataHandle(0)
{
}

//------------------------------------------------------------------------------
const void* RenderBulkData::getData() const
{
	if (m_dataHandle)
	{
		return m_commandList->getExtData(m_dataHandle);
	}
	else
	{
		return m_srcData;
	}
}

//------------------------------------------------------------------------------
void* RenderBulkData::alloc(RenderingCommandList* commandList, size_t size)
{
	m_size = size;
	return alloc(commandList);
}

//------------------------------------------------------------------------------
void* RenderBulkData::alloc(RenderingCommandList* commandList)
{
	if (m_dataHandle == 0)
	{
		m_commandList = commandList;
		m_dataHandle = m_commandList->allocExtData(m_size, m_srcData);
	}
	return m_commandList->getExtData(m_dataHandle);
}


//==============================================================================
// RenderingCommandList
//==============================================================================

//------------------------------------------------------------------------------
RenderingCommandList::RenderingCommandList(detail::GraphicsManager* manager, intptr_t mainThreadId)
	: m_manager(manager)
	, m_mainThreadId(mainThreadId)
	, m_commandList()
	, m_commandDataBuffer()
	, m_commandDataBufferUsed(0)
	, m_extDataBuffer()
	, m_extDataBufferUsed(0)
	, m_markGCList()
	, m_currentDevice(nullptr)
	, m_currentRenderer(nullptr)
	, m_running(false)
	, m_idling(true)
{
	m_commandList.reserve(DataBufferReserve);	// 適当に
	m_commandDataBuffer.resize(DataBufferReserve, false);
	m_commandDataBufferUsed = 0;
	m_extDataBuffer.resize(DataBufferReserve, false);
	m_extDataBufferUsed = sizeof(intptr_t);	// ポインタサイズ分予約済みにしておく (null チェックで 0 を使いたい)
}

//------------------------------------------------------------------------------
RenderingCommandList::~RenderingCommandList()
{
}

//------------------------------------------------------------------------------
void RenderingCommandList::clearCommands()
{
	// コマンド(とリソース)のクリアはメインスレッドからのみ許可する。
	// レンダリングスレッドでもできないことは無いが複雑になる。
	// そもそも、特に OpenGL リソースはマルチスレッドでの振る舞いが怪しい。確保と解放のスレッドは統一しておく。
	if (LN_REQUIRE(Thread::getCurrentThreadId() == m_mainThreadId)) return;

	m_commandList.clear();
	m_commandDataBufferUsed = 0;
	m_extDataBufferUsed = sizeof(intptr_t);	// ポインタサイズ分予約済みにしておく (null チェックで 0 を使いたい)

	// コマンド内のリソースの参照をクリア
	for (RefObject* obj : m_markGCList)
	{
		obj->release();
	}
	m_markGCList.clear();
	m_publisher = nullptr;
}

//------------------------------------------------------------------------------
void RenderingCommandList::execute(Driver::IGraphicsDevice* device/*Driver::IRenderer* renderer*/)
{
	LN_RC_TRACE("RenderingCommandList::Execute() s %p %d\n", this, m_commandList.getCount());
	// この関数は描画スレッドから呼ばれる

	m_currentDevice = device;
	m_currentRenderer = device->getRenderer();
	for (size_t dataIdx : m_commandList)
	{
		RenderingCommand* cmd = ((RenderingCommand*)getCommand(dataIdx));
		cmd->execute();
		cmd->~RenderingCommand();
	}

	if (m_publisher != nullptr)
	{
		m_publisher->PresentInternal();
	}

	LN_RC_TRACE("RenderingCommandList::Execute() e %p\n", this);
}

//------------------------------------------------------------------------------
//void RenderingCommandList::postExecute()
//{
//	clearCommands();
//}

//------------------------------------------------------------------------------
RenderingCommandList::DataHandle RenderingCommandList::allocCommand(size_t byteCount, const void* copyData)
{
	// バッファが足りなければ拡張する
	if (m_commandDataBufferUsed + byteCount > m_commandDataBuffer.getSize())
	{
		size_t newSize = m_commandDataBuffer.getSize() + std::max(m_commandDataBuffer.getSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_commandDataBuffer.resize(newSize, false);
	}

	if (copyData != nullptr)
	{
		byte_t* ptr = &(m_commandDataBuffer.getData()[m_commandDataBufferUsed]);
		size_t size = m_commandDataBuffer.getSize() - m_commandDataBufferUsed;
		memcpy_s(ptr, size, copyData, byteCount);
	}

	size_t dataIdx = m_commandDataBufferUsed;
	m_commandDataBufferUsed += byteCount;

	return dataIdx;
}

//------------------------------------------------------------------------------
RenderingCommandList::DataHandle RenderingCommandList::allocExtData(size_t byteCount, const void* copyData)
{
	// バッファが足りなければ拡張する
	if (m_extDataBufferUsed + byteCount > m_extDataBuffer.getSize())
	{
		size_t newSize = m_extDataBuffer.getSize() + std::max(m_extDataBuffer.getSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_extDataBuffer.resize(newSize, false);
	}

	if (copyData != nullptr)
	{
		byte_t* ptr = &(m_extDataBuffer.getData()[m_extDataBufferUsed]);
		size_t size = m_extDataBuffer.getSize() - m_extDataBufferUsed;
		memcpy_s(ptr, size, copyData, byteCount);
	}

	size_t dataIdx = m_extDataBufferUsed;
	m_extDataBufferUsed += byteCount;

	return dataIdx;
}

//------------------------------------------------------------------------------
void* RenderingCommandList::getExtData(DataHandle bufferIndex)
{
	return &(m_extDataBuffer.getData()[bufferIndex]);
}

//------------------------------------------------------------------------------
bool RenderingCommandList::checkOnStandaloneRenderingThread()
{
	RenderingThread* rt = m_manager->getRenderingThread();
	if (rt != nullptr)
	{
		if (Thread::getCurrentThreadId() == rt->getThreadId())
			return true;
	}
	return false;
}

} // namespace detail
LN_NAMESPACE_END
