
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		バイト配列を表すクラスです。
	@details	インスタンスは = 演算子等でコピーすることができます。
				内部的にはメモリ使用量を削減するため、コピーオンライトの共有が行われます。
*/
class ByteBuffer
{
public:

	/**
		@brief		サイズ 0 として初期化します。
		@details	実際にメモリ確保は行いません。
					別途、Resize() 等により確保する必要があります。
	*/
	ByteBuffer();

	/**
		@brief		指定されたバイト数でバッファを確保します。
		@param[in]	size		: バイト数
		@param[in]	zeroClear	: true の場合、確保したバッファ全体を 0 でクリアする
	*/
	ByteBuffer(size_t size, bool zeroClear = true);

	/**
		@brief		指定されたバッファの内容をコピーし、バッファを構築します。
		@param[in]	data		: コピー元のバッファ
		@param[in]	size		: コピーするバイト数
	*/
	ByteBuffer(const void* data, size_t size);

	/**
		@brief		指定されたバッファの内容をコピーし、バッファを構築します。
		@param[in]	data		: コピー元のバッファ
		@param[in]	size		: コピーするバイト数
		@param[in]	refMode		: (解説参照)
		@details	size 分のメモリを確保し、data の示すバッファからデータをコピーします。
					ただし、refMode が true の場合はメモリ確保やコピーを行わず、
					data のバッファを参照するだけになり、Clear() や Copy() 等の書き込み操作は
					このバッファに直接書き込まれます。
					また、このクラスは data のバッファを開放しません。
		@code
					// refMode の使用例
					char* data = new char[5];
					ByteBuffer buf(data, 5, true);
					buf.Clear();
					buf.Copy("abc", 3);
					printf(data);		// => "abc"
					delete[] data;
		@endcode
	*/
	ByteBuffer(void* data, size_t size, bool refMode);

	/**
		@brief		指定された NULL 終端文字列をコピーしてバッファを構築します。
		@param[in]	str			: NULL 終端文字列
		@details	終端の NULL 文字はコピーされません。
	*/
	ByteBuffer(const char* str);
	ByteBuffer(const wchar_t* str);

	/**
		@brief		他のバッファのコピーを作成します。
		@details	内部的にはコピーオンライトの共有が行われます。
	*/
	ByteBuffer(const ByteBuffer& buffer);

	virtual ~ByteBuffer();

public:

	/**
		@brief		バッファの再割り当てを行います。
		@param[in]	size		: バッファのバイト数
	*/
	void alloc(size_t size, bool zeroClear = true);

	/**
		@brief		バッファの再割り当てを行い、指定されたデータをコピーします。
		@param[in]	data		: コピー元のデータ
		@param[in]	size		: コピーするバイト数
	*/
	void alloc(const void* data, size_t size);

	/**
		@brief		バッファのサイズを変更します。
		@param[in]	size		: 新しいサイズ (バイト数)
		@param[in]	zeroClear	: 伸長する場合、 0 でクリアを行うか
		@details	再割り当て時には元の領域から新しい領域へ内容をコピーします。
					その際、縮退する場合は末尾の余剰データは切り捨てられ、
					伸長する場合は zeroClear 引数に従い 0 クリアを行います。
	*/
	void resize(size_t size, bool zeroClear = true);

	/**
		@brief		指定したメモリアドレスをデータストアとして参照します。
		@param[in]	buffer		: 参照先アドレス
		@param[in]	size		: 参照先データのバイト数
		@details	メモリ確保は行いません。
					data のバッファを参照するだけになり、Clear() や Copy() 等の書き込み操作は
					このバッファに直接書き込まれます。
					また、このクラスは data のバッファを開放しません。
		@code
					// refMode の使用例
					char* data = new char[5];
					ByteBuffer buf;
					buf.Attach(data, 5);
					buf.Clear();
					buf.Copy("abc", 3);
					printf(data);		// => "abc"
					delete[] data;
		@endcode
	*/
	void attach(void* buffer, size_t size);

	/**
		@brief		指定したバッファからこのバッファにデータをコピーします。
		@param[in]	data		: コピー元バッファ
		@param[in]	size		: コピーするバイト数
	*/
	void copy(const void* data, size_t size);

	/**
		@brief		指定したコピー先位置に、指定したバッファからこのバッファにデータをコピーします。
		@param[in]	offset		: コピー先の先頭インデックス
		@param[in]	data		: コピー元バッファ
		@param[in]	size		: コピーするバイト数
	*/
	void copy(size_t offset, const void* data, size_t size);

	/**
		@brief		バッファ全体を 0 でクリアします。
	*/
	void clear();

	/**
		@brief		バッファの先頭アドレスを取得します。
	*/
	byte_t* getData() { checkDetachShared(); return m_core->m_buffer; }
	const byte_t* getData() const { return m_core->m_buffer; }			/** @overload GetData */

	/**
		@brief		バッファの先頭アドレスを取得します。メモリの再割り当ては行われません。
	*/
	const byte_t* getConstData() const { return m_core->m_buffer; }

	/**
		@brief		バッファのサイズ (バイト数) を取得します。
	*/
	size_t getSize() const { return m_size; }
	
	/**
		@brief		メモリを解放します。
		@details	コピーオンライト用にメモリを共有している場合は参照を外します。
					Clear() とは異なり、メモリそのものを解放します。
	*/
	void free();

	bool equals(const ByteBuffer& buffer) const;

public:

	/**
		@brief		ByteBuffer と指定したサイズのバイト列を比較します。
		@return		負の値: buf1<buf2  0:buf1==buf2  正の値:buf1>buf2
		@details	部分比較はできません。buf1.GetSize() と buf2Size が一致しなければ不一致となります。
	*/
	static int compare(const ByteBuffer& buf1, const void* buf2, size_t buf2Size, int count = -1);

public:
	ByteBuffer& operator=(const ByteBuffer& right);
	byte_t& operator[] (size_t index);
	const byte_t& operator[] (size_t index) const;

private:
	void checkDetachShared();

private:
	class ByteBufferCore
	{
	public:
		ByteBufferCore(size_t size);
		ByteBufferCore(byte_t* sharedBuffer);
		~ByteBufferCore();
		inline bool isShared() const;
		inline void addRef();
		inline void release();

		byte_t*		m_buffer;

	private:
		int			m_refCount;
		bool		m_refMode;
	};
	static ByteBufferCore SharedCoreEmpty;

	ByteBufferCore*	m_core;
	size_t			m_capacity;
	size_t			m_size;
	// TODO: capacity と size も共有しても良いかも。
};

LN_NAMESPACE_END
