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

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN



//=============================================================================
// RenderBulkData
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderBulkData::RenderBulkData(void* srcData, size_t size)
	: m_srcData(srcData)
	, m_size(size)
	, m_commandList(nullptr)
	, m_dataHandle(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* RenderBulkData::GetData() const
{
	return m_commandList->GetExtData(m_dataHandle);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderBulkData::Alloc(RenderingCommandList* commandList)
{
	m_commandList = commandList;
	m_dataHandle = m_commandList->AllocExtData(m_size, m_srcData);
}


//=============================================================================
// RenderingCommand
//=============================================================================
//void* RenderingCommand::operator new(size_t size, RenderingCommandList* cmmandList)
//{
//	size_t dataIdx = cmmandList->Alloc(size, NULL);
//	RenderingCommand* cmd = (RenderingCommand*)cmmandList->GetBuffer(dataIdx);
//	cmd->m_commandList = cmmandList;
//	cmmandList->m_commandList.Add(dataIdx);
//	return cmd;
//}
//
//void RenderingCommand::operator delete(void* ptr, RenderingCommandList* cmmandList)
//{
//}

//=============================================================================
// RenderingCommandList
//=============================================================================

#define CREATE_COMMAND(name) \
	size_t dataIdx = Alloc(sizeof(name##Command), NULL); \
	name##Command* cmd = (name##Command*)&(m_commandDataBuffer.GetData()[dataIdx]); \
	cmd->Type = CommandType_##name; \
	m_commandList.Add(dataIdx);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingCommandList::RenderingCommandList()
	: m_commandList()
	, m_commandDataBuffer()
	, m_commandDataBufferUsed(0)
	, m_extDataBuffer()
	, m_extDataBufferUsed(0)
	, m_markGCList()
	, m_currentDevice(NULL)
	, m_currentRenderer(NULL)
	, m_running(false)
	, m_idling(true)
{
	m_commandList.Reserve(DataBufferReserve);	// 適当に
	m_commandDataBuffer.Resize(DataBufferReserve, false);
	m_commandDataBufferUsed = 0;
	m_extDataBuffer.Resize(DataBufferReserve, false);
	m_extDataBufferUsed = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingCommandList::~RenderingCommandList()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingCommandList::Execute(Driver::IGraphicsDevice* device/*Driver::IRenderer* renderer*/)
{
	LN_RC_TRACE("RenderingCommandList::Execute() s %p %d\n", this, m_commandList.GetCount());
	// この関数は描画スレッドから呼ばれる

	m_currentDevice = device;
	m_currentRenderer = device->GetRenderer();
	LN_FOREACH(size_t dataIdx, m_commandList)
	{
		RenderingCommand* cmd = ((RenderingCommand*)GetCommand(dataIdx));
		cmd->Execute();
		cmd->~RenderingCommand();
	}
	LN_RC_TRACE("RenderingCommandList::Execute() e %p\n", this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingCommandList::PostExecute()
{
	//LN_RC_TRACE("RenderingCommandList::PostExecute() s %p\n", this);
	//LN_FOREACH(size_t dataIdx, m_commandList)
	//{
	//	((RenderingCommand*)GetCommand(dataIdx))->Release(this);
	//	((RenderingCommand*)GetCommand(dataIdx))->~RenderingCommand();
	//}
	//LN_RC_TRACE("RenderingCommandList::PostExecute() e %p\n", this);
	m_commandList.Clear();
	m_commandDataBufferUsed = 0;


	LN_FOREACH(RefObject* obj, m_markGCList)
	{
		obj->Release();
	}
	m_markGCList.Clear();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingCommandList::DataHandle RenderingCommandList::AllocCommand(size_t byteCount, const void* copyData)
{
	// バッファが足りなければ拡張する
	if (m_commandDataBufferUsed + byteCount > m_commandDataBuffer.GetSize()) {
		size_t newSize = m_commandDataBuffer.GetSize() + std::max(m_commandDataBuffer.GetSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_commandDataBuffer.Resize(newSize, false);
	}

	if (copyData != NULL)
	{
		byte_t* ptr = &(m_commandDataBuffer.GetData()[m_commandDataBufferUsed]);
		size_t size = m_commandDataBuffer.GetSize() - m_commandDataBufferUsed;
		memcpy_s(ptr, size, copyData, byteCount);
	}

	size_t dataIdx = m_commandDataBufferUsed;
	m_commandDataBufferUsed += byteCount;

	return dataIdx;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingCommandList::DataHandle RenderingCommandList::AllocExtData(size_t byteCount, const void* copyData)
{
	// TODO: Bug: 確保メモリ量が増え続けている。

	// バッファが足りなければ拡張する
	if (m_extDataBufferUsed + byteCount > m_extDataBuffer.GetSize()) {
		size_t newSize = m_extDataBuffer.GetSize() + std::max(m_extDataBuffer.GetSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_extDataBuffer.Resize(newSize, false);
	}

	if (copyData != NULL)
	{
		byte_t* ptr = &(m_extDataBuffer.GetData()[m_extDataBufferUsed]);
		size_t size = m_extDataBuffer.GetSize() - m_extDataBufferUsed;
		memcpy_s(ptr, size, copyData, byteCount);
	}

	size_t dataIdx = m_extDataBufferUsed;
	m_extDataBufferUsed += byteCount;

	return dataIdx;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* RenderingCommandList::GetExtData(DataHandle bufferIndex)
{
	return &(m_extDataBuffer.GetData()[bufferIndex]);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
