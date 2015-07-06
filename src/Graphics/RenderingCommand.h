
#pragma once

#include <vector>
#include <Lumino/Base/Delegate.h>
#include <Lumino/Base/Rect.h>
#include "Device/DeviceInterface.h"
#include <Lumino/Graphics/RenderState.h>
#include <Lumino/Graphics/SamplerState.h>

#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Utils.h>

#define LN_RC_TRACE /*printf*/

namespace Lumino
{
namespace Graphics
{
class SwapChain;
class Texture;
class VertexBuffer;
class IndexBuffer;
class ShaderPass;
class RenderingCommandList;

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

	対策として、初期化は static 関数である Create() で行うようにした。
	ユーティリティ関数を用意したものの、キャストなどで冗長になってしまうのはやむなし。
	(急いで作ったから、もう少し改善の余地はあるかも)

	必ず守らなければならないのは、HandleCast() と Alloc() を同一式の中に書いてはならないこと。
	例えば以下のようにしてはならない。

		HandleCast<Command>(cmd)->m_data = Alloc(cmd, 10, ...);

	Alloc() で再配置の可能性があるので、格納先である m_data が不正なメモリ位置になってしまう。
*/
class RenderingCommand
{
protected:

	struct CmdInfo
	{
		RenderingCommandList*	m_commandList;
		size_t					m_dataHandle;
	};


	RenderingCommandList*	m_commandList;
	size_t					m_dataHandle;

	RenderingCommand() {}
	virtual ~RenderingCommand() {}

	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer) = 0;
	virtual void Release(RenderingCommandList* commandList) {}	// 削除予定。デストラクタに任せる

	template<class CommandT>
	static CommandT* HandleCast(CmdInfo& cmd)
	{
		return reinterpret_cast<CommandT*>(cmd.m_commandList->GetBuffer(cmd.m_dataHandle));
	}

	inline static size_t Alloc(CmdInfo& cmd, size_t byteCount, const void* copyData);

	//inline void MarkGC(RefObject* obj);

private:
	friend class RenderingCommandList;
	//static void* operator new(size_t size, RenderingCommandList* cmmandList);
	//static void operator delete(void* ptr, RenderingCommandList* cmmandList);	// 一応書いているが、直接 delete しようとするとコンパイルエラー「非置換 delete が定義されていません」になるはず
};




class RenderingCommandList
	: public RefObject
{
public:
	RenderingCommandList();
	virtual ~RenderingCommandList();

public:
	void ClearCommands();

	/// すべてのコマンドを実行する (描画スレッドから呼ばれる)
	void Execute(Device::IRenderer* renderer);

	/// 後処理 (描画スレッドから呼ばれる)
	void PostExecute();

	/// 描画キューに入っているか
	bool IsRunning() { return m_running.IsTrue(); }

	/// アイドル状態になるまで待つ
	void WaitForIdle() { m_idling.Wait(); }


private:

	static const size_t DataBufferReserve = 20;


private:

	template<typename T>
	T*  CreateCommand()
	{
		size_t dataHandle = Alloc(sizeof(T), NULL);
		T* t = new (GetBuffer(dataHandle))T();
		t->m_commandList = this;
		t->m_dataHandle = dataHandle;
		return t;
	}

public:


	template<typename T, typename A1>
	void AddCommand(const A1& a1)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1);
		LN_RC_TRACE("RenderingCommandList::AddCommand 0() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2>
	void AddCommand(const A1& a1, const A2& a2)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2);
		LN_RC_TRACE("RenderingCommandList::AddCommand 1() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2, typename A3>
	void AddCommand(const A1& a1, const A2& a2, const A3& a3)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2, a3);
		LN_RC_TRACE("RenderingCommandList::AddCommand 2() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4>
	void AddCommand(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2, a3, a4);
		LN_RC_TRACE("RenderingCommandList::AddCommand 3() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	void AddCommand(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2, a3, a4, a5);
		LN_RC_TRACE("RenderingCommandList::AddCommand 4() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void AddCommand(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2, a3, a4, a5, a6);
		LN_RC_TRACE("RenderingCommandList::AddCommand 5() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	void AddCommand(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2, a3, a4, a5, a6, a7);
		LN_RC_TRACE("RenderingCommandList::AddCommand 6() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}
	template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	void AddCommand(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
	{
		T* t = CreateCommand<T>();
		RenderingCommand::CmdInfo cmd;
		cmd.m_commandList = this;
		cmd.m_dataHandle = t->m_dataHandle;
		T::Create(cmd, a1, a2, a3, a4, a5, a6, a7, a8);
		LN_RC_TRACE("RenderingCommandList::AddCommand 8() s %p\n", this);
		m_commandList.Add(cmd.m_dataHandle);
	}

public:
	size_t Alloc(size_t byteCount, const void* copyData);	// 返すのは m_commandDataBuffer の要素番号 (バッファ拡張時の再配置に備える)
	void* GetBuffer(size_t bufferIndex);
	void MarkGC(RefObject* obj) { obj->AddRef();  m_markGCList.Add(obj); }

private:
	Array<size_t>			m_commandList;
	ByteBuffer				m_commandDataBuffer;
	size_t					m_commandDataBufferUsed;
	Array<RefObject*>		m_markGCList;

	friend class RenderingThread;
	friend class UserRenderingCommand;
	friend class RenderingCommand;
	Threading::EventFlag	m_running;	///< 描画キューに入っているか
	Threading::EventFlag	m_idling;
};

inline size_t RenderingCommand::Alloc(CmdInfo& cmd, size_t byteCount, const void* copyData) { return HandleCast<RenderingCommand>(cmd)->m_commandList->Alloc(byteCount, copyData); }



//=============================================================================
class SetRenderStateCommand : public RenderingCommand
{
	RenderState	m_state;

public:
	static void Create(CmdInfo& cmd, const RenderState& state)
	{
		HandleCast<SetRenderStateCommand>(cmd)->m_state = state;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->SetRenderState(m_state);
	}
};

//=============================================================================
class SetRenderTargetCommand : public RenderingCommand
{
	int m_index;
	Texture* m_sourceTexture;

public:
	static void Create(CmdInfo& cmd, int index, Texture* texture)
	{
		HandleCast<SetRenderTargetCommand>(cmd)->m_index = index;
		HandleCast<SetRenderTargetCommand>(cmd)->m_sourceTexture = texture;
		LN_SAFE_ADDREF(texture);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->SetRenderTarget(m_index, (m_sourceTexture != NULL) ? m_sourceTexture->m_deviceObj : NULL);
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_sourceTexture);
	}
};

//=============================================================================
class SetDepthBufferCommand : public RenderingCommand
{
	Texture* m_sourceTexture;

public:
	static void Create(CmdInfo& cmd, Texture* texture)
	{
		HandleCast<SetDepthBufferCommand>(cmd)->m_sourceTexture = texture;
		LN_SAFE_ADDREF(texture);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->SetDepthBuffer((m_sourceTexture != NULL) ? m_sourceTexture->m_deviceObj : NULL);
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_sourceTexture);
	}
};

//=============================================================================
class SetViewportCommand : public RenderingCommand
{
	Rect m_viewportRect;

public:
	static void Create(CmdInfo& cmd, const Rect& rect)
	{
		HandleCast<SetViewportCommand>(cmd)->m_viewportRect = rect;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->SetViewport(m_viewportRect);
	}
};

//=============================================================================
class SetVertexBufferCommand : public RenderingCommand
{
	VertexBuffer* m_sourceVertexBuffer;

public:
	static void Create(CmdInfo& cmd, VertexBuffer* vertexBuffer)
	{
		HandleCast<SetVertexBufferCommand>(cmd)->m_sourceVertexBuffer = vertexBuffer;
		LN_SAFE_ADDREF(vertexBuffer);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->SetVertexBuffer((m_sourceVertexBuffer != NULL) ? m_sourceVertexBuffer->m_deviceObj : NULL);
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_sourceVertexBuffer);
	}
};

//=============================================================================
class SetIndexBufferCommand : public RenderingCommand
{
	IndexBuffer* m_sourceIndexBuffer;

public:
	static void Create(CmdInfo& cmd, IndexBuffer* indexBuffer)
	{
		HandleCast<SetIndexBufferCommand>(cmd)->m_sourceIndexBuffer = indexBuffer;
		LN_SAFE_ADDREF(indexBuffer);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->SetIndexBuffer((m_sourceIndexBuffer != NULL) ? m_sourceIndexBuffer->m_deviceObj : NULL);
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_sourceIndexBuffer);
	}
};

//=============================================================================
class ClearCommand : public RenderingCommand
{
	bool m_target;
	bool m_depth;
	ColorF m_clearColor;
	float m_z;

public:
	static void Create(CmdInfo& cmd, bool target, bool depth, const ColorF& color, float z)
	{
		HandleCast<ClearCommand>(cmd)->m_target = target;
		HandleCast<ClearCommand>(cmd)->m_depth = depth;
		HandleCast<ClearCommand>(cmd)->m_clearColor = color;
		HandleCast<ClearCommand>(cmd)->m_z = z;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->Clear(m_target, m_depth, m_clearColor, m_z);
	}
};

//=============================================================================
class DrawPrimitiveCommand : public RenderingCommand
{
	PrimitiveType m_primitive;
	int m_startVertex;
	int m_primitiveCount;

public:
	static void Create(CmdInfo& cmd, PrimitiveType primitive, int startVertex, int primitiveCount)
	{
		HandleCast<DrawPrimitiveCommand>(cmd)->m_primitive = primitive;
		HandleCast<DrawPrimitiveCommand>(cmd)->m_startVertex = startVertex;
		HandleCast<DrawPrimitiveCommand>(cmd)->m_primitiveCount = primitiveCount;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->DrawPrimitive(m_primitive, m_startVertex, m_primitiveCount);
	}
};

//=============================================================================
class DrawPrimitiveIndexedCommand : public RenderingCommand
{
	PrimitiveType m_primitive;
	int m_startIndex;
	int m_primitiveCount;

public:
	static void Create(CmdInfo& cmd, PrimitiveType primitive, int startIndex, int primitiveCount)
	{
		HandleCast<DrawPrimitiveIndexedCommand>(cmd)->m_primitive = primitive;
		HandleCast<DrawPrimitiveIndexedCommand>(cmd)->m_startIndex = startIndex;
		HandleCast<DrawPrimitiveIndexedCommand>(cmd)->m_primitiveCount = primitiveCount;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		renderer->DrawPrimitiveIndexed(m_primitive, m_startIndex, m_primitiveCount);
	}
};

//=============================================================================
class SetSamplerStateCommand : public RenderingCommand
{
	Device::ITexture* m_targetTexture;
	SamplerState m_state;

public:
	static void Create(CmdInfo& cmd, Device::ITexture* texture, const SamplerState& state)
	{
		HandleCast<SetSamplerStateCommand>(cmd)->m_targetTexture = texture;
		HandleCast<SetSamplerStateCommand>(cmd)->m_state = state;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		m_targetTexture->SetSamplerState(m_state);
	}
};

//=============================================================================
//class SetTextureSubDataCommand : public RenderingCommand
//{
//	Device::ITexture*		m_targetTexture;
//	size_t					m_sourceBitmapDataHandle;
//	Size					m_size;
//	Imaging::PixelFormat	m_format;
//
//public:
//	static void Create(CmdInfo& cmd, Device::ITexture* texture, Imaging::Bitmap* bitmap)
//	{
//		// メモリ確保は一度ハンドルをローカル変数に置く。1つの式の中で複数回 HandleCast を使用してはならないため。
//		size_t tmpData = Alloc(cmd, bitmap->GetBitmapBuffer()->GetSize(), bitmap->GetBitmapBuffer()->GetData());
//
//		HandleCast<SetTextureSubDataCommand>(cmd)->m_targetTexture = texture;
//		HandleCast<SetTextureSubDataCommand>(cmd)->m_sourceBitmapDataHandle = tmpData;
//		HandleCast<SetTextureSubDataCommand>(cmd)->m_size = bitmap->GetSize();
//		HandleCast<SetTextureSubDataCommand>(cmd)->m_format = bitmap->GetPixelFormat();
//	}
//
//private:
//	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
//	{
//		// 参照モードで一時メモリを Bitmap 化する (メモリコピーを行わない)
//		ByteBuffer refData(commandList->GetBuffer(m_sourceBitmapDataHandle), Imaging::Bitmap::GetPixelFormatByteCount(m_format, m_size), true);
//		Imaging::Bitmap lockedBmp(&refData, m_size, m_format);
//
//		m_targetTexture->SetSubData(&lockedBmp);
//	}
//};

//=============================================================================
class SetShaderVariableCommand : public RenderingCommand
{
	union
	{
		bool				BoolVal;	// X11 では "Bool" が定義済みマクロなので Val を付けて逃げる
		int					Int;
		float				Float;
		size_t				VectorsBufferIndex;
		//Vector4*			Vector;		///< 単一 Vector または VectorArray
		//Lumino::Matrix*		Matrix;		///< 単一 Matrix または MatrixArray
		Device::ITexture*	Texture;
	};
	size_t						m_arrayLength;
	ShaderVariableType			m_variableType;
	Device::IShaderVariable*	m_target;

public:
	static void Create(CmdInfo& cmd, Device::IShaderVariable* target, bool value)
	{
		HandleCast<SetShaderVariableCommand>(cmd)->m_target = target;
		HandleCast<SetShaderVariableCommand>(cmd)->m_variableType = ShaderVariableType_Bool;
		HandleCast<SetShaderVariableCommand>(cmd)->BoolVal = value;
		LN_SAFE_ADDREF(target);
	}
	static void Create(CmdInfo& cmd, Device::IShaderVariable* target, int value)
	{
		HandleCast<SetShaderVariableCommand>(cmd)->m_target = target;
		HandleCast<SetShaderVariableCommand>(cmd)->m_variableType = ShaderVariableType_Int;
		HandleCast<SetShaderVariableCommand>(cmd)->Int = value;
		LN_SAFE_ADDREF(target);
	}
	static void Create(CmdInfo& cmd, Device::IShaderVariable* target, float value)
	{
		HandleCast<SetShaderVariableCommand>(cmd)->m_target = target;
		HandleCast<SetShaderVariableCommand>(cmd)->m_variableType = ShaderVariableType_Float;
		HandleCast<SetShaderVariableCommand>(cmd)->Float = value;
		LN_SAFE_ADDREF(target);
	}
	static void Create(CmdInfo& cmd, Device::IShaderVariable* target, const Lumino::Vector4* vectors, size_t count)
	{
		size_t tmpData = Alloc(cmd, sizeof(Vector4) * count, vectors);
		HandleCast<SetShaderVariableCommand>(cmd)->m_target = target;
		HandleCast<SetShaderVariableCommand>(cmd)->m_arrayLength = count;
		HandleCast<SetShaderVariableCommand>(cmd)->m_variableType = (count == 1) ? ShaderVariableType_Vector : ShaderVariableType_VectorArray;
		HandleCast<SetShaderVariableCommand>(cmd)->VectorsBufferIndex = tmpData;
		LN_SAFE_ADDREF(target);
	}
	static void Create(CmdInfo& cmd, Device::IShaderVariable* target, const Lumino::Matrix* matrices, size_t count)
	{
		size_t tmpData = Alloc(cmd, sizeof(Matrix) * count, matrices);
		HandleCast<SetShaderVariableCommand>(cmd)->m_target = target;
		HandleCast<SetShaderVariableCommand>(cmd)->m_arrayLength = count;
		HandleCast<SetShaderVariableCommand>(cmd)->m_variableType = (count == 1) ? ShaderVariableType_Matrix : ShaderVariableType_MatrixArray;
		HandleCast<SetShaderVariableCommand>(cmd)->VectorsBufferIndex = tmpData;
		LN_SAFE_ADDREF(target);
	}
	static void Create(CmdInfo& cmd, Device::IShaderVariable* target, Device::ITexture* value)
	{
		HandleCast<SetShaderVariableCommand>(cmd)->m_target = target;
		HandleCast<SetShaderVariableCommand>(cmd)->m_variableType = ShaderVariableType_Texture;
		HandleCast<SetShaderVariableCommand>(cmd)->Texture = value;
		LN_SAFE_ADDREF(target);
		LN_SAFE_ADDREF(value);
	}


private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		switch (m_variableType)
		{
		case ShaderVariableType_Bool:
			m_target->SetBool(BoolVal);
			break;
		case ShaderVariableType_Int:
			m_target->SetInt(Int);
			break;
		case ShaderVariableType_Float:
			m_target->SetFloat(Float);
			break;
		case ShaderVariableType_Vector:
			m_target->SetVector(*((Vector4*)commandList->GetBuffer(VectorsBufferIndex)));
			break;
		case ShaderVariableType_VectorArray:
			m_target->SetVectorArray((Vector4*)commandList->GetBuffer(VectorsBufferIndex), m_arrayLength);
			break;
		case ShaderVariableType_Matrix:
			m_target->SetMatrix(*((Matrix*)commandList->GetBuffer(VectorsBufferIndex)));
			break;
		case ShaderVariableType_MatrixArray:
			m_target->SetMatrixArray((Matrix*)commandList->GetBuffer(VectorsBufferIndex), m_arrayLength);
			break;
		case ShaderVariableType_Texture:
			m_target->SetTexture(Texture);
			break;
		//case ShaderVariableType_String:
		default:
			break;
		}
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		if (m_variableType == ShaderVariableType_Texture) {
			LN_SAFE_RELEASE(Texture);
		}
		LN_SAFE_RELEASE(m_target);
	}
};

//=============================================================================
class ApplyShaderPassCommand : public RenderingCommand
{
	Device::IShaderPass* m_pass;

public:
	static void Create(CmdInfo& cmd, Device::IShaderPass* pass)
	{
		HandleCast<ApplyShaderPassCommand>(cmd)->m_pass = pass;
		LN_SAFE_ADDREF(pass);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		m_pass->Apply();
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_pass);
	}
};

//=============================================================================
class PresentCommand : public RenderingCommand
{
	SwapChain* m_targetSwapChain;

public:
	static void Create(CmdInfo& cmd, SwapChain* swapChain)
	{
		HandleCast<PresentCommand>(cmd)->m_targetSwapChain = swapChain;
		LN_SAFE_ADDREF(swapChain);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		m_targetSwapChain->m_deviceObj->Present(m_targetSwapChain->m_backColorBuffer->m_deviceObj);
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		// 実行完了。m_waiting を ture にすることで、メインスレッドからはこのスワップチェインをキューに追加できるようになる。
		// コマンドの成否にかかわらず true にしないと、例外した後にデッドロックが発生する。
		m_targetSwapChain->m_waiting.SetTrue();
		LN_SAFE_RELEASE(m_targetSwapChain);
	}
};
	
//=============================================================================
class SetSubDataTextureCommand : public RenderingCommand	// TODO: ↑似たようなコマンドが残っている
{
	Device::ITexture*		m_targetTexture;
	Point					m_offset;
	size_t					m_bmpDataIndex;
	Size					m_bmpSize;
	// ↑エラーチェックは Texture で行い、フォーマットは既に決まっていることを前提とするため、コマンドに乗せるデータはこれだけでOK。

public:
	static void Create(CmdInfo& cmd, Device::ITexture* texture, const Point& offset, const void* data, size_t dataSize, const Size& bmpSize)
	{
		size_t tmpData = Alloc(cmd, dataSize, data);
		HandleCast<SetSubDataTextureCommand>(cmd)->m_targetTexture = texture;
		HandleCast<SetSubDataTextureCommand>(cmd)->m_offset = offset;
		HandleCast<SetSubDataTextureCommand>(cmd)->m_bmpDataIndex = tmpData;
		HandleCast<SetSubDataTextureCommand>(cmd)->m_bmpSize = bmpSize;
		LN_SAFE_ADDREF(texture);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		m_targetTexture->SetSubData(m_offset, commandList->GetBuffer(m_bmpDataIndex), m_bmpSize);
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_targetTexture);
	}
};

//=============================================================================
class Texture_SetSubDataBitmapCommand : public RenderingCommand
{
	RefPtr<Device::ITexture> m_targetTexture;
	Point m_offset;

	size_t m_bmpDataIndex;
	Size m_size;
	int m_pitch;
	Imaging::PixelFormat m_format;
	bool m_upFlow;

public:
	static void Create(CmdInfo& cmd, Device::ITexture* texture, const Point& offset, Imaging::Bitmap* bmp)
	{
		size_t tmpData = Alloc(cmd, bmp->GetBitmapBuffer()->GetSize(), bmp->GetBitmapBuffer()->GetConstData());
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_targetTexture = texture;
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_offset = offset;
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_bmpDataIndex = tmpData;
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_size = bmp->GetSize();
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_pitch = bmp->GetPitch();
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_format = bmp->GetPixelFormat();
		HandleCast<Texture_SetSubDataBitmapCommand>(cmd)->m_upFlow = bmp->IsUpFlow();
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		if (m_format == Utils::TranslatePixelFormat(m_targetTexture->GetTextureFormat()))
		{
			//Imaging::Bitmap bmp(commandList->GetBuffer(m_bmpDataIndex), m_size, m_format, m_pitch, m_upFlow);
			m_targetTexture->SetSubData(m_offset, commandList->GetBuffer(m_bmpDataIndex), m_size);
		}
		else {
			LN_THROW(0, NotImplementedException);
		}
	}
};


//=============================================================================
class ReadLockTextureCommand : public RenderingCommand
{
	Texture*	m_targetTexture;

public:
	static void Create(CmdInfo& cmd, Texture* texture)
	{
		HandleCast<ReadLockTextureCommand>(cmd)->m_targetTexture = texture;
		LN_SAFE_ADDREF(texture);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		m_targetTexture->m_primarySurface = m_targetTexture->m_deviceObj->Lock();
		// Texture::Lock() はこの後コマンドリストが空になるまで待機する
		// (実際のところ、このコマンドが最後のコマンドのはず)
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_targetTexture);
	}
};

//=============================================================================
class ReadUnlockTextureCommand : public RenderingCommand
{
	Texture*	m_targetTexture;

public:
	static void Create(CmdInfo& cmd, Texture* texture)
	{
		HandleCast<ReadUnlockTextureCommand>(cmd)->m_targetTexture = texture;
		LN_SAFE_ADDREF(texture);
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		m_targetTexture->m_deviceObj->Unlock();
		m_targetTexture->m_primarySurface = NULL;
		// ReadLockTextureCommand と同じように、Texture::Unlock() で待機している。
		// (でも、ここまで待機することも無いかも？)
	}
	virtual void Release(RenderingCommandList* commandList)
	{
		LN_SAFE_RELEASE(m_targetTexture);
	}
};

} // namespace Graphics
} // namespace Lumino
