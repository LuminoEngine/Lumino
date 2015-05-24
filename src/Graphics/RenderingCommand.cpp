
#pragma once

#include "../Internal.h"
#include "Device/DeviceInterface.h"
#include <Lumino/Graphics/Shader.h>
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{


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
	: m_running(false)
	, m_idling(true)
{
	m_commandList.Reserve(DataBufferReserve);	// 適当に
	m_commandDataBuffer.Resize(DataBufferReserve, false);
	m_commandDataBufferUsed = 0;
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
void RenderingCommandList::Execute(Device::IRenderer* renderer)
{
	// この関数は描画スレッドから呼ばれる

	LN_FOREACH(size_t dataIdx, m_commandList)
	{
		((RenderingCommand*)GetBuffer(dataIdx))->Execute(this, renderer);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingCommandList::PostExecute()
{
	LN_FOREACH(size_t dataIdx, m_commandList)
	{
		((RenderingCommand*)GetBuffer(dataIdx))->Release(this);
		((RenderingCommand*)GetBuffer(dataIdx))->~RenderingCommand();
	}
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
size_t RenderingCommandList::Alloc(size_t byteCount, const void* copyData)
{
	// バッファが足りなければ拡張する
	if (m_commandDataBufferUsed + byteCount > m_commandDataBuffer.GetSize()) {
		size_t newSize = m_commandDataBuffer.GetSize() + std::max(m_commandDataBuffer.GetSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_commandDataBuffer.Resize(newSize, false);
	}

	if (copyData != NULL)
	{
		byte_t* ptr = &(m_commandDataBuffer.GetData()[m_commandDataBufferUsed]);
		memcpy(ptr, copyData, byteCount);
	}

	size_t dataIdx = m_commandDataBufferUsed;
	m_commandDataBufferUsed += byteCount;

	return dataIdx;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void* RenderingCommandList::GetBuffer(size_t bufferIndex)
{
	return &(m_commandDataBuffer.GetData()[bufferIndex]);
}

} // namespace Graphics
} // namespace Lumino
