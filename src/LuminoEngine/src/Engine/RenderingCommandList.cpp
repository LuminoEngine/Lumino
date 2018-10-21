
#include "Internal.hpp"
#include "RenderingCommandList.hpp"
#include "../Engine/LinearAllocator.hpp"

namespace ln {
namespace detail {

//=============================================================================
// RenderBulkData

RenderBulkData::RenderBulkData(const void* srcData, size_t size)
	: m_srcData(srcData)
	, m_size(size)
	, m_copyedData(nullptr)
{
}

RenderBulkData::RenderBulkData(const RenderBulkData& other)
{
	m_srcData = other.m_srcData;
	m_size = other.m_size;
	m_copyedData = other.m_copyedData;
}

RenderBulkData& RenderBulkData::operator=(const RenderBulkData& other)
{
	m_srcData = other.m_srcData;
	m_size = other.m_size;
	m_copyedData = other.m_copyedData;
	return *this;
}

//=============================================================================
// RenderingCommandList

RenderingCommandList::RenderingCommandList(LinearAllocatorPageManager* manager)
	: m_linearAllocator(makeRef<LinearAllocator>(manager))
{
}

RenderingCommandList::~RenderingCommandList()
{
}

RenderBulkData RenderingCommandList::allocateBulkData(size_t size)
{
	void* ptr = m_linearAllocator->allocate(size);
	RenderBulkData data(nullptr, size);
	data.m_copyedData = ptr;
	return data;
}

void RenderingCommandList::clear()
{
	m_linearAllocator->cleanup();
}

//=============================================================================
// RenderingCommand

RenderingCommand::RenderingCommand()
{
}

RenderingCommand::~RenderingCommand()
{
}

} // namespace detail
} // namespace ln

