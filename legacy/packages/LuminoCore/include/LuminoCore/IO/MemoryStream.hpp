
#pragma once

#include "../Base/Buffer.hpp"
#include "Stream.hpp"

namespace ln {

/**
	@brief	データの読み書き先としてメモリを使用するストリームです。
*/
class MemoryStream
	: public Stream
{
public:
	MemoryStream();
	
	/**
		@brief		既存のバッファを参照する、サイズ変更できない MemoryStream を構築します。
		@param		buffer	: バッファの先頭アドレス
		@param		size	: サイズ(バイト数)
		@details	buffer に指定したバッファは、このクラスのインスタンスが存在する間は開放しないでください。
	*/
	MemoryStream(void* buffer, size_t size);

	
	/**
		@brief		既存のバッファを参照する、サイズ変更できない読み取り専用の MemoryStream を構築します。
		@param		buffer	: バッファの先頭アドレス
		@param		size	: サイズ(バイト数)
		@details	buffer に指定したバッファは、このクラスのインスタンスが存在する間は開放しないでください。
	*/
	MemoryStream(const void* buffer, size_t size, bool copy = false);


public:

	///**
	//	@brief		データの読み書き先バイト配列の先頭アドレスを取得します。
	//*/
	//void* getBuffer(size_t index = 0);

	void* data() const;
	const void* head() const;

public:
	// override Stream
	virtual bool canRead() const override;
	virtual bool canWrite() const override;
	virtual int64_t length() const override;
	virtual int64_t position() const override;
	virtual size_t read(void* buffer, size_t byteCount) override;
	virtual void write(const void* data, size_t byteCount) override;
	virtual void seek(int64_t offset, SeekOrigin origin) override;
	virtual void flush() override;

public:	// TODO: スタックでも使いたい
	~MemoryStream();
	void initialize(size_t size);	// TODO: Resize 
	void reset(void* buffer, size_t size);
	void reset(const void* buffer, size_t size, bool copy = false);

private:
	//std::vector<byte_t>	m_buffer;			///< 可変長の場合はこのバッファを使う
	ByteBuffer m_buffer;
	size_t				m_seekPos;

	void*				m_fixedBuffer;		///< 固定長の場合はこのバッファを使う
	const void*			m_constfixedBuffer;	///< 固定長の場合はこのバッファを使う
	size_t				m_fixedBufferSize;
	bool				m_autoDelete;
};

} // namespace ln
