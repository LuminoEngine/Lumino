
#pragma once
#include <vector>
#include <Lumino/Base/Delegate.h>
#include <Lumino/Base/GeometryStructs.h>
#include "Device/GraphicsDriverInterface.h"
#include <Lumino/Graphics/RenderState.h>
#include <Lumino/Graphics/SamplerState.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Utils.h>

#define LN_RC_TRACE /*printf*/

LN_NAMESPACE_BEGIN
class SwapChain;
class Texture;
class VertexBuffer;
class IndexBuffer;
class ShaderPass;

namespace detail {
class RenderingCommandList;
class RenderBulkData;

/// 
///		×必ずメインスレッド (描画スレッド以外) で生成されて、描画スレッドで実行される。
///		×operator new 等、C++ のコアな機能を使っているが、これはサブクラスとして定義するコマンドの実装を簡単にするため。
///		×メモリ確保と m_commandList の初期化を operator new で行うことで、サブクラスにアロケータ処理を書く必要が無くなる。
///
///		Alloc は Create 内でのみ可
/**
	描画コマンドの基底クラス

	以前は operator new をオーバーロードし、コンストラクタで初期化を行えるようにしていた。
	しかし、コマンドは RenderingCommandList::Alloc() が呼ばれると再配置される可能性がある。
	そのため、コンストラクタ内で Alloc() すると this が不正なポインタになることがある。

	対策として、初期化は static 関数である create() で行うようにした。
	ユーティリティ関数を用意したものの、キャストなどで冗長になってしまうのはやむなし。
	(急いで作ったから、もう少し改善の余地はあるかも)

	必ず守らなければならないのは、HandleCast() と Alloc() を同一式の中に書いてはならないこと。
	例えば以下のようにしてはならない。

		HandleCast<Command>(cmd)->m_data = Alloc(cmd, 10, ...);

	Alloc() で再配置の可能性があるので、格納先である m_data が不正なメモリ位置になってしまう。
*/
struct RenderingCommand
{
public:
	virtual ~RenderingCommand();

	typedef size_t	DataHandle;

	virtual void execute() = 0;


protected:
	inline Driver::IGraphicsDevice* getDevice() const;
	inline Driver::IRenderer* getRenderer() const;	// TODO: いらないかも
	inline DataHandle allocExtData(size_t byteCount, const void* copyData);
	inline void* getExtData(DataHandle handle);
	inline void markGC(RefObject* obj);

	template<typename T>
	inline void markBulkData(RenderingCommandList* commandList, T& value);
	//template<>
	//inline void markBulkData<RenderBulkData>(RenderingCommandList* commandList, RenderBulkData& value);

private:
	friend class RenderingCommandList;
	RenderingCommandList*	m_commandList;
	//size_t					m_dataHandle;

	//struct CmdInfo
	//{
	//	RenderingCommandList*	m_commandList;
	//	size_t					m_dataHandle;
	//};



	//RenderingCommand() {}
	//virtual ~RenderingCommand() {}

	//virtual void Execute() = 0;
	//virtual void Release(RenderingCommandList* commandList) {}	// 削除予定。デストラクタに任せる

	//template<class CommandT>
	//static CommandT* HandleCast(CmdInfo& cmd)
	//{
	//	return reinterpret_cast<CommandT*>(cmd.m_commandList->getExtData(cmd.m_dataHandle));
	//}

	//inline static size_t allocExtData(size_t byteCount, const void* copyData);

	//inline void markGC(RefObject* obj);

//private:
	//friend class RenderingCommandList;
	//static void* operator new(size_t size, RenderingCommandList* cmmandList);
	//static void operator delete(void* ptr, RenderingCommandList* cmmandList);	// 一応書いているが、直接 delete しようとするとコンパイルエラー「非置換 delete が定義されていません」になるはず
};

//#define LN_RENDERING_COMMAND_IMPLEMENT(cmdClass) \
	
//	static void ExecuteInternal(RenderingCommand* cmd) { Execute(static_cast<cmdClass*>(cmd)); }

class RenderBulkData
{
public:
	RenderBulkData();
	RenderBulkData(const void* srcData, size_t size);
	const void* getData() const;
	size_t getSize() const { return m_size; }

	// 確保したメモリは書き込み可能ポインタとして返される。
	void* alloc(RenderingCommandList* commandList, size_t size);

	// TODO: internal
	void* alloc(RenderingCommandList* commandList);

private:
	const void*				m_srcData;
	size_t					m_size;
	RenderingCommandList*	m_commandList;
	size_t					m_dataHandle;
};


class RenderingCommandList
	: public RefObject
{
public:
	typedef size_t	DataHandle;

	RenderingCommandList(detail::GraphicsManager* manager, intptr_t mainThreadId);
	virtual ~RenderingCommandList();

public:
	void clearCommands();

	/// すべてのコマンドを実行する (描画スレッドから呼ばれる)
	void execute(Driver::IGraphicsDevice* device/*, Device::IRenderer* renderer*/);

	/// 後処理 (描画スレッドから呼ばれる)
	//void postExecute();

	/// 描画キューに入っているか
	bool isRunning() { return m_running.isTrue(); }

	/// アイドル状態になるまで待つ
	void waitForIdle() { m_idling.wait(); }


private:
	static const size_t DataBufferReserve = 20;	// TODO: デバッグ用。もっと大きくて良い

	void presentRHIAndEndExecute();

private:
	DataHandle allocCommand(size_t byteCount, const void* copyData);

	RenderingCommand* getCommand(DataHandle bufferIndex) { return (RenderingCommand*)&(m_commandDataBuffer.getData()[bufferIndex]); }

	template<typename T>
	DataHandle createCommand()
	{
		size_t dataHandle = allocCommand(sizeof(T), NULL);
		T* t = new (getCommand(dataHandle))T();
		t->m_commandList = this;
		//t->m_dataHandle = dataHandle;
		return dataHandle;
	}

	bool checkOnStandaloneRenderingThread();

public:

	template<typename T, typename... TArgs>
	void addCommand(TArgs... args)
	{
		DataHandle h = createCommand<T>();
		T* cmd = static_cast<T*>(getCommand(h));
		//RenderingCommand::CmdInfo cmd;
		//cmd->m_commandList = this;
		//cmd.m_dataHandle = t->m_dataHandle;
		cmd->create(args...);
		LN_RC_TRACE("RenderingCommandList::AddCommand 0() s %p\n", this);
		m_commandList.add(h);
	}

	template<typename T, typename... TArgs>
	void enqueueCommand(TArgs... args)
	{
		if (LN_CHECK_STATE(!checkOnStandaloneRenderingThread())) return;

		size_t dataHandle = allocCommand(sizeof(T), NULL);
		T* t = new (getCommand(dataHandle))T(args...);
		t->m_commandList = this;
		t->OnEnqueued(this);
		LN_RC_TRACE("RenderingCommandList::EnqueueCommand 0() s %p\n", this);
		m_commandList.add(dataHandle);
	}

public:
	DataHandle allocExtData(size_t byteCount, const void* copyData);
	void* getExtData(DataHandle bufferIndex);
	void markGC(RefObject* obj)
	{
		if (obj != NULL)	// テクスチャを解除したりするときは NULL が渡されてくる
		{
			obj->addRef();
			m_markGCList.add(obj);
		}
	}

private:
	detail::GraphicsManager*	m_manager;
	intptr_t					m_mainThreadId;		// for inspection
	List<size_t>			m_commandList;
	ByteBuffer				m_commandDataBuffer;
	size_t					m_commandDataBufferUsed;
	ByteBuffer				m_extDataBuffer;
	size_t					m_extDataBufferUsed;
	List<RefObject*>		m_markGCList;
	Driver::IGraphicsDevice*	m_currentDevice;
	Driver::IRenderer*			m_currentRenderer;	///< 描画実行中の IRenderer

	friend class RenderingThread;
	friend class UserRenderingCommand;
	friend struct RenderingCommand;
	ConditionFlag	m_running;	///< 描画キューに入っているか
	ConditionFlag	m_idling;
	Ref<SwapChain> m_publisher;
};


inline Driver::IGraphicsDevice* RenderingCommand::getDevice() const
{
	return m_commandList->m_currentDevice;
}
inline Driver::IRenderer* RenderingCommand::getRenderer() const
{
	return m_commandList->m_currentRenderer;
}
inline RenderingCommand::DataHandle RenderingCommand::allocExtData(size_t byteCount, const void* copyData)
{
	return m_commandList->allocExtData(byteCount, copyData);
}
inline void* RenderingCommand::getExtData(DataHandle handle)
{
	return m_commandList->getExtData(handle);
}
inline void RenderingCommand::markGC(RefObject* obj)
{
	m_commandList->markGC(obj);
}
template<typename T>
inline void RenderingCommand::markBulkData(RenderingCommandList* commandList, T& value)
{
}
template<>
inline void RenderingCommand::markBulkData<RenderBulkData>(RenderingCommandList* commandList, RenderBulkData& value)
{
	value.alloc(commandList);
}

} // namespace detail

#define LN_ENQUEUE_RENDER_COMMAND_PARAM(type, param) type param

#define LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, commandName, ...) \
	if (manager->getRenderingType() == GraphicsRenderingType::Threaded) { \
		manager->getPrimaryRenderingCommandList()->enqueueCommand<commandName>(__VA_ARGS__); \
	} \
	else { \
		commandName cmd(__VA_ARGS__); \
		cmd.execute(); \
	}

#define LN_ENQUEUE_RENDER_COMMAND_1(name, manager, type1, param1, code) \
	class RenderCommand_##name : public ln::detail::RenderingCommand \
	{ \
	public: \
		type1 param1; \
		RenderCommand_##name( \
			LN_ENQUEUE_RENDER_COMMAND_PARAM(type1, in_##param1)) \
			: param1(in_##param1) \
		{} \
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1);

#define LN_ENQUEUE_RENDER_COMMAND_2(name, manager, type1, param1, type2, param2, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2);

#define LN_ENQUEUE_RENDER_COMMAND_3(name, manager, type1, param1, type2, param2, type3, param3, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
			RenderingCommand::markBulkData(commandList, param3); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2, param3);

#define LN_ENQUEUE_RENDER_COMMAND_4(name, manager, type1, param1, type2, param2, type3, param3, type4, param4, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
			RenderingCommand::markBulkData(commandList, param3); \
			RenderingCommand::markBulkData(commandList, param4); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2, param3, param4);

#define LN_ENQUEUE_RENDER_COMMAND_5(name, manager, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
			RenderingCommand::markBulkData(commandList, param3); \
			RenderingCommand::markBulkData(commandList, param4); \
			RenderingCommand::markBulkData(commandList, param5); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2, param3, param4, param5);

#define LN_ENQUEUE_RENDER_COMMAND_6(name, manager, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, type6, param6, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
			RenderingCommand::markBulkData(commandList, param3); \
			RenderingCommand::markBulkData(commandList, param4); \
			RenderingCommand::markBulkData(commandList, param5); \
			RenderingCommand::markBulkData(commandList, param6); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2, param3, param4, param5, param6);

#define LN_ENQUEUE_RENDER_COMMAND_7(name, manager, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, type6, param6, type7, param7, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
			RenderingCommand::markBulkData(commandList, param3); \
			RenderingCommand::markBulkData(commandList, param4); \
			RenderingCommand::markBulkData(commandList, param5); \
			RenderingCommand::markBulkData(commandList, param6); \
			RenderingCommand::markBulkData(commandList, param7); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2, param3, param4, param5, param6, param7);

#define LN_ENQUEUE_RENDER_COMMAND_8(name, manager, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5, type6, param6, type7, param7, type8, param8, code) \
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
		void OnEnqueued(ln::detail::RenderingCommandList* commandList) \
		{ \
			RenderingCommand::markBulkData(commandList, param1); \
			RenderingCommand::markBulkData(commandList, param2); \
			RenderingCommand::markBulkData(commandList, param3); \
			RenderingCommand::markBulkData(commandList, param4); \
			RenderingCommand::markBulkData(commandList, param5); \
			RenderingCommand::markBulkData(commandList, param6); \
			RenderingCommand::markBulkData(commandList, param7); \
			RenderingCommand::markBulkData(commandList, param8); \
		} \
		virtual void execute() override \
		{ \
			code; \
		} \
	}; \
	LN_ENQUEUE_RENDER_COMMAND_CREATE(manager, RenderCommand_##name, param1, param2, param3, param4, param5, param6, param7, param8);


//==============================================================================
struct SetSamplerStateCommand : public ln::detail::RenderingCommand
{
	Driver::ITexture* m_targetTexture;
	SamplerState m_state;
	void create(Driver::ITexture* texture, const SamplerState& state)
	{
		m_targetTexture = texture;
		m_state = state;
		markGC(m_targetTexture);
	}
	void execute()
	{
		m_targetTexture->setSamplerState(m_state);
	}
};

//==============================================================================
struct SetShaderVariableCommand : public ln::detail::RenderingCommand
{
	union
	{
		bool				BoolVal;	// X11 では "Bool" が定義済みマクロなので Val を付けて逃げる
		int					Int;
		float				Float;
		size_t				VectorsBufferIndex;
		//Vector4*			Vector;		///< 単一 Vector または VectorArray
		//ln::Matrix*		Matrix;		///< 単一 Matrix または MatrixArray
		Driver::ITexture*	Texture;
	};
	DataHandle					m_arrayLength;
	ShaderVariableType			m_variableType;
	Driver::IShaderVariable*	m_target;

	void create(Driver::IShaderVariable* target, bool value)
	{
		m_target = target;
		m_variableType = ShaderVariableType_Bool;
		BoolVal = value;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, int value)
	{
		m_target = target;
		m_variableType = ShaderVariableType_Int;
		Int = value;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, float value)
	{
		m_target = target;
		m_variableType = ShaderVariableType_Float;
		Float = value;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, const Vector4& value)
	{
		size_t tmpData = allocExtData(sizeof(Vector4), &value);
		m_target = target;
		m_variableType = ShaderVariableType_Vector;
		VectorsBufferIndex = tmpData;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, const Vector4* vectors, size_t count)
	{
		size_t tmpData = allocExtData(sizeof(Vector4) * count, vectors);
		m_target = target;
		m_arrayLength = count;
		m_variableType = ShaderVariableType_VectorArray;
		VectorsBufferIndex = tmpData;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, const Matrix* matrices, size_t count)
	{
		size_t tmpData = allocExtData(sizeof(Matrix) * count, matrices);
		m_target = target;
		m_arrayLength = count;
		m_variableType = ShaderVariableType_MatrixArray;
		VectorsBufferIndex = tmpData;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, const Matrix& value)
	{
		size_t tmpData = allocExtData(sizeof(Matrix), &value);
		m_target = target;
		m_variableType = ShaderVariableType_Matrix;
		VectorsBufferIndex = tmpData;
		markGC(target);
	}
	void create(Driver::IShaderVariable* target, Driver::ITexture* value)
	{
		m_target = target;
		m_variableType = ShaderVariableType_DeviceTexture;
		Texture = value;
		markGC(target);
		markGC(value);
	}

	void execute()
	{
		switch (m_variableType)
		{
		case ShaderVariableType_Bool:
			m_target->setBool(BoolVal);
			break;
		case ShaderVariableType_Int:
			m_target->setInt(Int);
			break;
		case ShaderVariableType_Float:
			m_target->setFloat(Float);
			break;
		case ShaderVariableType_Vector:
			m_target->setVector(*((Vector4*)getExtData(VectorsBufferIndex)));
			break;
		case ShaderVariableType_VectorArray:
			m_target->setVectorArray((Vector4*)getExtData(VectorsBufferIndex), m_arrayLength);
			break;
		case ShaderVariableType_Matrix:
			m_target->setMatrix(*((Matrix*)getExtData(VectorsBufferIndex)));
			break;
		case ShaderVariableType_MatrixArray:
			m_target->setMatrixArray((Matrix*)getExtData(VectorsBufferIndex), m_arrayLength);
			break;
		case ShaderVariableType_DeviceTexture:
			m_target->setTexture(Texture);
			break;
			//case ShaderVariableType_String:
		default:
			break;
		}
	}
};

//==============================================================================
struct ApplyShaderPassCommand : public ln::detail::RenderingCommand
{
	Driver::IShaderPass* m_pass;
	void create(Driver::IShaderPass* pass)
	{
		m_pass = pass;
		markGC(pass);
	}
	void execute() { getRenderer()->setShaderPass(m_pass); /*m_pass->apply();*/ }
};

//==============================================================================
//struct PresentCommand : public ln::detail::RenderingCommand
//{
//	SwapChain* m_targetSwapChain;
//
//	void create(SwapChain* swapChain)
//	{
//		m_targetSwapChain = swapChain;
//		markGC(swapChain);
//	}
//
//	void execute()
//	{
//		m_targetSwapChain->PresentInternal();
//	}
//};

//==============================================================================
struct SetSubDataTextureCommand : public ln::detail::RenderingCommand
{
	Driver::ITexture*		m_targetTexture;
	PointI					m_offset;
	DataHandle				m_bmpDataIndex;
	size_t					m_dataSize;
	SizeI					m_bmpSize;
	// ↑エラーチェックは Texture で行い、フォーマットは既に決まっていることを前提とするため、コマンドに乗せるデータはこれだけでOK。

	void create(Driver::ITexture* texture, const PointI& offset, const void* data, size_t dataSize, const SizeI& bmpSize)
	{
		m_targetTexture = texture;
		m_offset = offset;
		m_bmpDataIndex = allocExtData(dataSize, data);
		m_dataSize = dataSize;
		m_bmpSize = bmpSize;
		markGC(texture);
	}

	void execute()
	{
		m_targetTexture->setSubData(m_offset, getExtData(m_bmpDataIndex), m_dataSize, m_bmpSize);
	}
};

//==============================================================================
struct ReadLockTextureCommand : public ln::detail::RenderingCommand
{
	Texture*	m_targetTexture;
	void create(Texture* texture)
	{
		m_targetTexture = texture;
		markGC(texture);
	}
	void execute()
	{
		m_targetTexture->m_primarySurface = m_targetTexture->m_deviceObj->lock();
		// Texture::lock() はこの後コマンドリストが空になるまで待機する
		// (実際のところ、このコマンドが最後のコマンドのはず)
	}
};

//==============================================================================
struct ReadUnlockTextureCommand : public ln::detail::RenderingCommand
{
	Texture*	m_targetTexture;
	void create(Texture* texture)
	{
		m_targetTexture = texture;
		markGC(texture);
	}
	void execute()
	{
		m_targetTexture->m_deviceObj->unlock();
		m_targetTexture->m_primarySurface = NULL;
		// ReadLockTextureCommand と同じように、Texture::unlock() で待機している。
		// (でも、ここまで待機することも無いかも？)
	}
};

LN_NAMESPACE_END
