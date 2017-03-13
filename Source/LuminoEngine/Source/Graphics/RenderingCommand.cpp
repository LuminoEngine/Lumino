/*
	[2015/7/31] コマンドの実行は仮想関数？関数ポインタ？
		Release ビルド 100000000 回の単純呼び出しで、
		仮想関数 : 450ms
		関数ポインタ : 400ms
		微々たる差だが、関数ポインタが少しだけ有利。
*/
#pragma once

#include "../Internal.h"
#include "Device/GraphicsDriverInterface.h"
#include <Lumino/Graphics/Shader.h>
#include "RenderingCommand.h"
#include "GraphicsManager.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

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
const void* RenderBulkData::GetData() const
{
	if (m_dataHandle)
	{
		return m_commandList->GetExtData(m_dataHandle);
	}
	else
	{
		return m_srcData;
	}
}

//------------------------------------------------------------------------------
void* RenderBulkData::Alloc(RenderingCommandList* commandList, size_t size)
{
	m_size = size;
	return Alloc(commandList);
}

//------------------------------------------------------------------------------
void* RenderBulkData::Alloc(RenderingCommandList* commandList)
{
	if (m_dataHandle == 0)
	{
		m_commandList = commandList;
		m_dataHandle = m_commandList->AllocExtData(m_size, m_srcData);
	}
	return m_commandList->GetExtData(m_dataHandle);
}


//==============================================================================
// RenderingCommandList
//==============================================================================

//------------------------------------------------------------------------------
RenderingCommandList::RenderingCommandList(detail::GraphicsManager* manager)
	: m_manager(manager)
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
	m_commandList.Reserve(DataBufferReserve);	// 適当に
	m_commandDataBuffer.Resize(DataBufferReserve, false);
	m_commandDataBufferUsed = 0;
	m_extDataBuffer.Resize(DataBufferReserve, false);
	m_extDataBufferUsed = sizeof(intptr_t);	// ポインタサイズ分予約済みにしておく (null チェックで 0 を使いたい)
}

//------------------------------------------------------------------------------
RenderingCommandList::~RenderingCommandList()
{
}

//------------------------------------------------------------------------------
void RenderingCommandList::ClearCommands()
{
	m_commandList.Clear();
	m_commandDataBufferUsed = 0;
	m_extDataBufferUsed = sizeof(intptr_t);	// ポインタサイズ分予約済みにしておく (null チェックで 0 を使いたい)

	// コマンド内のリソースの参照をクリア
	for (RefObject* obj : m_markGCList)
	{
		obj->Release();
	}
	m_markGCList.Clear();
}

//------------------------------------------------------------------------------
void RenderingCommandList::Execute(Driver::IGraphicsDevice* device/*Driver::IRenderer* renderer*/)
{
	LN_RC_TRACE("RenderingCommandList::Execute() s %p %d\n", this, m_commandList.GetCount());
	// この関数は描画スレッドから呼ばれる

	m_currentDevice = device;
	m_currentRenderer = device->GetRenderer();
	for (size_t dataIdx : m_commandList)
	{        
		/*
		-        cmd    0x14e21ea0 {m_commandList=0xfeeefeee {m_manager=??? m_commandList={m_data=??? } m_commandDataBuffer=...} }    ln::RenderingCommand *
		+        __vfptr    0xfeeefeee {???, ???}    void * *
		+        m_commandList    0xfeeefeee {m_manager=??? m_commandList={m_data=??? } m_commandDataBuffer={m_core=??? m_capacity=??? ...} ...}    ln::RenderingCommandList *
		dataIdx    2024    unsigned int
		※m_commandList は正しい。cmd->m_commandList がおかしい。

		※ウィンドウがアクティブになったときに起こりやすい？
		*/
		RenderingCommand* cmd = ((RenderingCommand*)GetCommand(dataIdx));
		cmd->Execute();
		cmd->~RenderingCommand();
	}
	LN_RC_TRACE("RenderingCommandList::Execute() e %p\n", this);
}

//------------------------------------------------------------------------------
void RenderingCommandList::PostExecute()
{
	ClearCommands();
}

//------------------------------------------------------------------------------
RenderingCommandList::DataHandle RenderingCommandList::AllocCommand(size_t byteCount, const void* copyData)
{
	// バッファが足りなければ拡張する
	if (m_commandDataBufferUsed + byteCount > m_commandDataBuffer.GetSize())
	{
		size_t newSize = m_commandDataBuffer.GetSize() + std::max(m_commandDataBuffer.GetSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_commandDataBuffer.Resize(newSize, false);
	}

	if (copyData != nullptr)
	{
		byte_t* ptr = &(m_commandDataBuffer.GetData()[m_commandDataBufferUsed]);
		size_t size = m_commandDataBuffer.GetSize() - m_commandDataBufferUsed;
		memcpy_s(ptr, size, copyData, byteCount);
	}

	size_t dataIdx = m_commandDataBufferUsed;
	m_commandDataBufferUsed += byteCount;

	return dataIdx;
}

//------------------------------------------------------------------------------
RenderingCommandList::DataHandle RenderingCommandList::AllocExtData(size_t byteCount, const void* copyData)
{
	// バッファが足りなければ拡張する
	if (m_extDataBufferUsed + byteCount > m_extDataBuffer.GetSize())
	{
		size_t newSize = m_extDataBuffer.GetSize() + std::max(m_extDataBuffer.GetSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_extDataBuffer.Resize(newSize, false);
	}

	if (copyData != nullptr)
	{
		byte_t* ptr = &(m_extDataBuffer.GetData()[m_extDataBufferUsed]);
		size_t size = m_extDataBuffer.GetSize() - m_extDataBufferUsed;
		memcpy_s(ptr, size, copyData, byteCount);
	}

	size_t dataIdx = m_extDataBufferUsed;
	m_extDataBufferUsed += byteCount;

	return dataIdx;
}

//------------------------------------------------------------------------------
void* RenderingCommandList::GetExtData(DataHandle bufferIndex)
{
	return &(m_extDataBuffer.GetData()[bufferIndex]);
}

//------------------------------------------------------------------------------
bool RenderingCommandList::CheckOnStandaloneRenderingThread()
{
	RenderingThread* rt = m_manager->GetRenderingThread();
	if (rt != nullptr)
	{
		if (Thread::GetCurrentThreadId() == rt->GetThreadId())
			return true;
	}
	return false;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
