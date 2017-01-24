
#pragma once

#include <vector>
#include "../Base/Exception.h"
#include "Stream.h"

LN_NAMESPACE_BEGIN
class MemoryStream;
typedef RefPtr<MemoryStream>	MemoryStreamPtr;

/**
	@brief	データの読み書き先としてメモリを使用するストリームです。
*/
class MemoryStream
	: public Stream
{
public:

	static MemoryStreamPtr Create();

	/**
		@brief		既存のバッファを指定して、サイズ変更できないメモリストリームを作成します。
		@param		buffer	: バッファの先頭アドレス
		@param		size	: サイズ(バイト数)
		@details	buffer に指定したバッファは、このクラスのインスタンスが存在する間は開放しないでください。
	*/
	static MemoryStreamPtr Create(void* buffer, size_t size);
	
	/**
		@brief		既存のバッファを指定して、サイズ変更できない読み取り専用のメモリストリームを作成します。
		@param		buffer	: バッファの先頭アドレス
		@param		size	: サイズ(バイト数)
		@details	buffer に指定したバッファは、このクラスのインスタンスが存在する間は開放しないでください。
	*/
	static MemoryStreamPtr Create(const void* buffer, size_t size, bool copy = false);

public:

	/**
		@brief		データの読み書き先バイト配列の先頭アドレスを取得します。
	*/
	void* GetBuffer(size_t index = 0);

public:
	// override Stream
	virtual bool CanRead() const;
	virtual bool CanWrite() const;
	virtual int64_t GetLength() const;
	virtual int64_t GetPosition() const;
	virtual size_t Read(void* buffer, size_t byteCount);
	virtual void Write(const void* data, size_t byteCount);
	virtual void Seek(int64_t offset, SeekOrigin origin);
	virtual void Flush() {}		// Write が直接メモリに書きこむので不要

public:	// TODO: スタックでも使いたい
	MemoryStream();
	MemoryStream(void* buffer, size_t size);
	MemoryStream(const void* buffer, size_t size, bool copy = false);
	~MemoryStream();
	void Initialize(size_t size);	// TODO: Resize 
	void Initialize(void* buffer, size_t size);
	void Initialize(const void* buffer, size_t size, bool copy = false);

private:
	std::vector<byte_t>	m_buffer;			///< 可変長の場合はこのバッファを使う
	size_t				m_seekPos;

	void*				m_fixedBuffer;		///< 固定長の場合はこのバッファを使う
	const void*			m_constfixedBuffer;	///< 固定長の場合はこのバッファを使う
	size_t				m_fixedBufferSize;
	bool				m_autoDelete;
};

LN_NAMESPACE_END
