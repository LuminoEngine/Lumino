
#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/detail/RenderingCommandList.hpp>
#include <LuminoGraphics/GraphicsContext.hpp>

namespace ln {
namespace detail {

//==============================================================================
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

void RenderBulkData::commitAllocation(RenderingCommandList* commandList)
{
    if (!m_copyedData) {
        m_copyedData = commandList->linearAllocator()->allocate(m_size);
        memcpy(m_copyedData, m_srcData, m_size);
    }
}

//==============================================================================
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
    for (RenderingCommand* cmd : m_commandList) {
        cmd->~RenderingCommand();
    }
    m_commandList.clear();
	m_linearAllocator->cleanup();
}

void* RenderingCommandList::allocateBuffer(size_t size)
{
	return m_linearAllocator->allocate(size);
}

void RenderingCommandList::execute()
{
    for (RenderingCommand* cmd : m_commandList) {
        cmd->execute();
    }
}

//==============================================================================
// RenderingCommand

RenderingCommand::RenderingCommand()
{
}

RenderingCommand::~RenderingCommand()
{
}

//==============================================================================
// RenderingQueue

RenderingQueue::RenderingQueue()
{
}

void RenderingQueue::dispose()
{

}

//// TODO: submit をここに。
//void RenderingQueue::pushRenderingCommand(RenderingCommandList* commandList)
//{
//    std::lock_guard<std::mutex> lock(m_mutex);
//
//    m_commandListQueue.push_back(commandList);
//    //commandList->m_running.setTrue();
//    //commandList->m_idling.setFalse();
//    //commandList->m_publisher = publisher;
//    //commandList->addRef();
//    //m_running.setTrue();
//}
//
//Ref<RenderingCommandList> RenderingQueue::submitCommandList(RenderingCommandList* commandList)
//{
//	std::lock_guard<std::mutex> lock(m_mutex);
//	m_commandListQueue.push_back(commandList);
//}

void RenderingQueue::submit(GraphicsContext* context)
{
	if (LN_REQUIRE(context)) return;

#if 0
    if (context->renderingType() == RenderingType::Immediate) {
        // 一時メモリのクリアはしておく
        context->m_executingCommandList->clear();
		std::swap(context->m_recordingCommandList, context->m_executingCommandList);
		//context->m_recordingCommandList->clear();
    }
    else {
        // もし前回発行した CommandList がまだ実行中であればここで待つ
        context->m_executingCommandList->waitForExecutionEnd();

        // m_executingCommandList は次に m_recordingCommandList となるので、ここで clear
        context->m_executingCommandList->clear();

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_commandListQueue.push_back(context->m_recordingCommandList);
            std::swap(context->m_recordingCommandList, context->m_executingCommandList);
        }

        //execute(); // TODO: test
    }
#endif

}

void RenderingQueue::execute()
{
    while (true)
    {
        // キューからコマンドリストを1つ取り出してみる
        Ref<RenderingCommandList> commandList;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (!m_commandListQueue.empty()) {
                commandList = m_commandListQueue.front();
                m_commandListQueue.pop_front();
				commandList->m_running.lock();	// ここで実行開始にする。(submit() から抜けたら必ず実行中になっているようにする)
            }
            else {
                // 終了
                break;
            }
        }

        // コマンドリストのキューをチェックした後、キューが空で、かつ終了要求が来ている場合は終了する
        //if (m_running.isFalse() && m_endRequested.isTrue())
        //{
        //    break;
        //}

        if (commandList)
        {
            commandList->execute();
			commandList->m_running.unlock();
            //// 基本的に描画スレッドでの例外は、復帰不能なエラーと考える。(か、assert 的な、そもそも API の使い方が間違っている)
            //// エラーはここで保持し、一度でも例外したら Failed 状態にする。
            ////  Failed 状態の間はコマンドを実行しない。
            //if (m_exception == nullptr)
            //{
            //    try
            //    {
            //        //ScopedProfilerSection sec(Profiler::Group_RenderThread, Profiler::Section_RenderThread_CommandExecute);

            //        ScopedProfilingSection2 section2(ProfilingKeys::Rendering_PresentDevice);


            //        // コマンドリスト実行
            //        commandList->execute(m_device);
            //    }
            //    catch (Exception& e)
            //    {
            //        m_exception = e.copy();
            //    }
            //}
            ////commandList->postExecute();
            ////commandList->m_running.setFalse();
            //commandList->m_idling.setTrue();
            //commandList->release();
        }
        else
        {
            // 処理するコマンドがない。true になるまで待機する
            //m_running.wait();
        }
    }
}

} // namespace detail
} // namespace ln

