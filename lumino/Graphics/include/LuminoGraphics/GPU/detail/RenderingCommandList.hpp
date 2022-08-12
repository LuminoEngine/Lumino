#pragma once
#include <mutex>
//#include "LinearAllocator.hpp"

namespace ln {
class GraphicsCommandList;
namespace detail {
class LinearAllocatorPageManager;
class LinearAllocator;
class RenderingCommandList;
class RenderingCommand;
class RenderingQueue;

/*
	データバッファをレンダリングスレッドに転送するためのデータ構造。

	Immediate モードと Threaded モードでメモリコピーするかどうかが異なる。
	まず、モードに限らずデータは参照で持つ。
	Immediate モードの場合はこのデータがそのまま使われるのでメモリ確保は発生しない。
	Threaded モードの場合は、RenderBulkData をメンバに持つ RenderingCommand がキューに入った時点でメモリ確保されコピーされる。

	インスタンス自体はコピー可能。
	これはモードに関係なく一時メモリとして RenderingCommandList::allocateBulkData() でメモリ確保 → データ構築したいため。
*/
class RenderBulkData
{
public:
	RenderBulkData(const void* srcData, size_t size);
	RenderBulkData(const RenderBulkData& other);
	RenderBulkData& operator=(const RenderBulkData& other);

	const void* data() const { return (m_copyedData) ? m_copyedData : m_srcData; }
	size_t size() const { return m_size; }

	void* writableData() const { return m_copyedData; }

    void commitAllocation(RenderingCommandList* commandList);

private:
	const void* m_srcData;
	size_t m_size;
	void* m_copyedData;

	friend class RenderingCommandList;
};

/*
	描画コマンドとデータバッファをまとめたもの。

	主に Threaded モードで使われるが、RenderBulkData が参照するメモリの実体は Immediate でも使用される。
	このためモードにかかわらず、コマンド実行後に clear() を呼び出し、破棄する必要がある。
*/
class RenderingCommandList
	: public RefObject
{
public:
	RenderingCommandList(LinearAllocatorPageManager* manager);
	virtual ~RenderingCommandList();

	template<typename T, typename... TArgs>
	void enqueueCommand(TArgs... args)
	{
		void* buf = allocateBuffer(sizeof(T));
        T* command = new (buf)T(args...);
        //command->m_commandList = this;
        command->onEnqueued(this);
        //LN_RC_TRACE("RenderingCommandList::EnqueueCommand 0() s %p\n", this);
        m_commandList.add(command);
	}

	RenderBulkData allocateBulkData(size_t size);
    const Ref<LinearAllocator>& linearAllocator() const { return m_linearAllocator; }


	void clear();


	void waitForExecutionEnd() { m_running.wait(); }

private:
	void* allocateBuffer(size_t size);

    // すべてのコマンドを実行する (描画スレッドから呼ばれる)
    void execute();

	Ref<LinearAllocator> m_linearAllocator;
    List<RenderingCommand*> m_commandList;
	ConditionEvent m_running;

	friend class RenderingQueue;
};

class RenderingCommand
{
public:
	RenderingCommand();
	virtual ~RenderingCommand();

	virtual void execute() = 0;

	template<typename T>
    inline void commitBulkData(RenderingCommandList* commandList, T& value);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(RenderingCommand);
};


template<typename T>
inline void RenderingCommand::commitBulkData(RenderingCommandList* commandList, T& value)
{
}
template<>
inline void RenderingCommand::commitBulkData<RenderBulkData>(RenderingCommandList* commandList, RenderBulkData& value)
{
    value.commitAllocation(commandList);
}


class RenderingQueue
    : public RefObject
{
public:
    RenderingQueue();
    void dispose();

    //void pushRenderingCommand(RenderingCommandList* commandList);

	void submit(GraphicsCommandList* context);

    // 溜まっている list をすべて実行
    void execute();

private:
	Ref<RenderingCommandList> submitCommandList(RenderingCommandList* commandList);

	//Ref<RenderingCommandList> m_inFlightRenderingCommandList;
	//std::deque<Ref<RenderingCommandList>> m_freeCommandLists;
    std::deque<Ref<RenderingCommandList>> m_commandListQueue;
    std::mutex m_mutex;
};


#define LN_ENQUEUE_RENDER_COMMAND_PARAM(type, param) type param

#define LN_ENQUEUE_RENDER_COMMAND_CREATE(context, commandName, ...) \
	if (detail::GraphicsContextInternal::getRenderingType(context) == RenderingType::Threaded) { \
		detail::GraphicsContextInternal::getRenderingCommandList(context)->enqueueCommand<commandName>(__VA_ARGS__); \
	} \
	else { \
		commandName cmd(__VA_ARGS__); \
		cmd.execute(); \
	}

#define LN_ENQUEUE_RENDER_COMMAND_1(name, context, type1, param1, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1)) \
			: param1(in_##param1) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1);

#define LN_ENQUEUE_RENDER_COMMAND_2(name, context, type1, param1, type2, param2, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2);

#define LN_ENQUEUE_RENDER_COMMAND_3(name, context, type1, param1, type2, param2, type3, param3, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		type3 param3; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type3, in_##param3)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
			, param3(in_##param3) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
			RenderingCommand::commitBulkData(commandList, param3); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2, param3);

#define LN_ENQUEUE_RENDER_COMMAND_4(name, context, type1, param1, type2, param2, type3, param3, type4, param4, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		type3 param3; \
		type4 param4; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type3, in_##param3), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type4, in_##param4)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
			, param3(in_##param3) \
			, param4(in_##param4) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
			RenderingCommand::commitBulkData(commandList, param3); \
			RenderingCommand::commitBulkData(commandList, param4); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2, param3, param4);

#define LN_ENQUEUE_RENDER_COMMAND_5(name, context, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		type3 param3; \
		type4 param4; \
		type5 param5; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type3, in_##param3), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type4, in_##param4), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type5, in_##param5)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
			, param3(in_##param3) \
			, param4(in_##param4) \
			, param5(in_##param5) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
			RenderingCommand::commitBulkData(commandList, param3); \
			RenderingCommand::commitBulkData(commandList, param4); \
			RenderingCommand::commitBulkData(commandList, param5); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2, param3, param4, param5);

#define LN_ENQUEUE_RENDER_COMMAND_6(name, context, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, type6, param6, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		type3 param3; \
		type4 param4; \
		type5 param5; \
		type6 param6; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type3, in_##param3), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type4, in_##param4), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type5, in_##param5), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type6, in_##param6)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
			, param3(in_##param3) \
			, param4(in_##param4) \
			, param5(in_##param5) \
			, param6(in_##param6) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
			RenderingCommand::commitBulkData(commandList, param3); \
			RenderingCommand::commitBulkData(commandList, param4); \
			RenderingCommand::commitBulkData(commandList, param5); \
			RenderingCommand::commitBulkData(commandList, param6); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2, param3, param4, param5, param6);

#define LN_ENQUEUE_RENDER_COMMAND_7(name, context, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, type6, param6, type7, param7, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		type3 param3; \
		type4 param4; \
		type5 param5; \
		type6 param6; \
		type7 param7; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type3, in_##param3), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type4, in_##param4), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type5, in_##param5), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type6, in_##param6), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type7, in_##param7)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
			, param3(in_##param3) \
			, param4(in_##param4) \
			, param5(in_##param5) \
			, param6(in_##param6) \
			, param7(in_##param7) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
			RenderingCommand::commitBulkData(commandList, param3); \
			RenderingCommand::commitBulkData(commandList, param4); \
			RenderingCommand::commitBulkData(commandList, param5); \
			RenderingCommand::commitBulkData(commandList, param6); \
			RenderingCommand::commitBulkData(commandList, param7); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2, param3, param4, param5, param6, param7);

#define LN_ENQUEUE_RENDER_COMMAND_8(name, context, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, type6, param6, type7, param7, type8, param8, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		type2 param2; \
		type3 param3; \
		type4 param4; \
		type5 param5; \
		type6 param6; \
		type7 param7; \
		type8 param8; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type2, in_##param2), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type3, in_##param3), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type4, in_##param4), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type5, in_##param5), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type6, in_##param6), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type7, in_##param7), \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type8, in_##param8)) \
			: param1(in_##param1) \
			, param2(in_##param2) \
			, param3(in_##param3) \
			, param4(in_##param4) \
			, param5(in_##param5) \
			, param6(in_##param6) \
			, param7(in_##param7) \
			, param8(in_##param8) \
		{} \
		void onEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::commitBulkData(commandList, param1); \
			RenderingCommand::commitBulkData(commandList, param2); \
			RenderingCommand::commitBulkData(commandList, param3); \
			RenderingCommand::commitBulkData(commandList, param4); \
			RenderingCommand::commitBulkData(commandList, param5); \
			RenderingCommand::commitBulkData(commandList, param6); \
			RenderingCommand::commitBulkData(commandList, param7); \
			RenderingCommand::commitBulkData(commandList, param8); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(context, RenderCommand_##name, param1, param2, param3, param4, param5, param6, param7, param8);

} // namespace detail
} // namespace ln

